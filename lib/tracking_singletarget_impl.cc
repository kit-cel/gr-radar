/* -*- c++ -*- */
/*
 * Copyright 2014 Communications Engineering Lab, KIT.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tracking_singletarget_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <time.h>

namespace gr {
namespace radar {

tracking_singletarget::sptr tracking_singletarget::make(int num_particle,
                                                        float std_range_meas,
                                                        float std_velocity_meas,
                                                        float std_accel_sys,
                                                        float threshold_track,
                                                        int threshold_lost,
                                                        std::string filter)
{
    return gnuradio::make_block_sptr<tracking_singletarget_impl>(num_particle,
                                                                 std_range_meas,
                                                                 std_velocity_meas,
                                                                 std_accel_sys,
                                                                 threshold_track,
                                                                 threshold_lost,
                                                                 filter);
}

/*
 * The private constructor
 */
tracking_singletarget_impl::tracking_singletarget_impl(int num_particle,
                                                       float std_range_meas,
                                                       float std_velocity_meas,
                                                       float std_accel_sys,
                                                       float threshold_track,
                                                       int threshold_lost,
                                                       std::string filter)
    : gr::block("tracking_singletarget",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_num_particle = num_particle;
    d_std_range_meas = std_range_meas;
    d_std_velocity_meas = std_velocity_meas;
    d_std_accel_sys = std_accel_sys;
    d_threshold_track = threshold_track;
    d_threshold_lost = threshold_lost;
    d_filter = filter;

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in, [this](pmt::pmt_t msg) { this->handle_msg(msg); });

    // Register output message port
    d_port_id_out = pmt::mp("Msg out");
    message_port_register_out(d_port_id_out);

    // Setup random numbers (seed)
    std::srand(time(NULL));

    // Setup tracker
    d_particle_range.resize(num_particle); // num particles
    d_particle_velocity.resize(num_particle);
    d_particle_weight.resize(num_particle);

    int matrix_size = 2; // setup matrix
    Q.resize(matrix_size);
    R.resize(matrix_size);
    R_inv.resize(matrix_size), P.resize(matrix_size), K.resize(matrix_size);
    for (int k = 0; k < matrix_size; k++) {
        Q[k].resize(matrix_size);
        R[k].resize(matrix_size);
        R_inv[k].resize(matrix_size);
        P[k].resize(matrix_size);
        K[k].resize(matrix_size);
    }

    d_lost = 0;         // number of lost tracks
    d_is_track = false; // is a track active

    // Setup measurement variance
    R[0][0] = d_std_range_meas * d_std_range_meas;
    R[1][1] = std_velocity_meas * std_velocity_meas;

    R_det = R[0][0] * R[1][1];
    R_inv[0][0] = R[1][1] / R_det;
    R_inv[1][1] = R[0][0] / R_det;
}

/*
 * Our virtual destructor.
 */
tracking_singletarget_impl::~tracking_singletarget_impl() {}

void tracking_singletarget_impl::handle_msg(pmt::pmt_t msg)
{
    size_t size_msg;
    bool range_found, velocity_found, time_found;
    range_found = false;
    velocity_found = false;
    time_found = false;
    std::vector<pmt::pmt_t> store_msg;

    size_msg = pmt::length(msg);
    pmt::pmt_t msg_part;
    // Go through msg and search for key symbols "range" and "velocity" (or other keys!)
    // and get data This is singletarget tracker! Taken only first item of vector
    // FIXME: error handling for false input? e.g. multi target input (throw warning)
    std::vector<float> vec_pmt;
    d_is_empty = false;                  // check if range or velocity is empty
    for (int k = 0; k < size_msg; k++) { // FIXME: errorhandling for wrong input?
        msg_part = pmt::nth(k, msg);
        if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "range") {
            vec_pmt = (pmt::f32vector_elements(pmt::nth(1, msg_part)));
            if (vec_pmt.size() != 0)
                d_range_meas = vec_pmt[0];
            else
                d_is_empty = true;
            range_found = true;
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "velocity") {
            vec_pmt = (pmt::f32vector_elements(pmt::nth(1, msg_part)));
            if (vec_pmt.size() != 0)
                d_velocity_meas = vec_pmt[0];
            else
                d_is_empty = true;
            velocity_found = true;
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "rx_time") {
            d_time = (float)pmt::to_uint64(pmt::tuple_ref(pmt::nth(1, msg_part), 0)) +
                     pmt::to_double(pmt::tuple_ref(pmt::nth(1, msg_part), 1));
            time_found = true;
            store_msg.push_back(msg_part); // store msg, will not be changed
        } else {
            // Store msg part for repacking if no symbol is found
            store_msg.push_back(msg_part);
        }
    }

    // Do tracking if both variables are found, else throw error
    bool state_tracking;
    if (range_found && velocity_found && time_found)
        state_tracking = tracking();
    else
        throw std::runtime_error("range or velocity or time not found");

    if (state_tracking) {
        // Repack msg
        pmt::pmt_t msg_out;
        if (store_msg.size() != 0) { // if msgs are stored add them
            msg_out = pmt::list1(store_msg[0]);
            for (int k = 1; k < store_msg.size(); k++) {
                msg_out = pmt::list_add(msg_out, store_msg[k]);
            }
        }
        pmt::pmt_t msg_val;
        std::vector<float> f32_val;
        // Add velocity
        f32_val.push_back(d_velocity_est);
        msg_val = pmt::list2(pmt::string_to_symbol("velocity"),
                             pmt::init_f32vector(f32_val.size(), f32_val));
        if (store_msg.size() != 0) { // if a pmt list is created before
            msg_out = pmt::list_add(msg_out, msg_val);
        } else { // if msg_out is not a pmt list
            msg_out = pmt::list1(msg_val);
        }
        // Add range
        f32_val.clear();
        f32_val.push_back(d_range_est);
        msg_val = pmt::list2(pmt::string_to_symbol("range"),
                             pmt::init_f32vector(f32_val.size(), f32_val));
        msg_out = pmt::list_add(msg_out, msg_val);

        // Publish msg
        message_port_pub(d_port_id_out, msg_out);
    }
}

float tracking_singletarget_impl::random_normal(float mean, float std)
{
    int num_range = 1000000;
    float v1 = (rand() % num_range) / (float)num_range;
    float v2 = (rand() % num_range) / (float)num_range;
    return mean + std * std::sqrt(-2 * std::log(v1)) * std::cos(2 * GR_M_PI * v2);
}

void // begin kalman-filter
tracking_singletarget_impl::filter_kalman()
{
    float a, b, c, d, factor;
    /* predection based on system-model */
    d_range_est =
        d_range_est -
        d_delta_t * d_velocity_est; // r_k = r_k-1 + delta_T * v_k-1 // v_k = v_k-1
    // calc state-covariance matrix
    P[0][0] = P[0][0] + P[1][0] * d_delta_t +
              d_delta_t * (P[0][1] + d_delta_t * P[1][1]) + Q[0][0];
    P[0][1] = P[0][1] + d_delta_t * P[1][1] + Q[0][1];
    P[1][0] = P[1][0] + d_delta_t * P[1][1] + Q[1][0];
    P[1][1] = P[1][1] + Q[1][1];

    /*  filtering based on new measurement */
    // calc Kalman-Gain: K = P*H'/(H*P*H'+R) here: K = P/(P+R)
    a = P[0][0] + R[0][0];
    b = P[0][1] + R[0][1];
    c = P[1][0] + R[1][0];
    d = P[1][1] + R[1][1];
    factor = 1 / (a * d - b * c); // for 2x2 matrix invers
    K[0][0] = factor * (d * P[0][0] - c * P[0][1]);
    K[0][1] = factor * (a * P[0][1] - b * P[0][0]);
    K[1][0] = factor * (d * P[1][0] - c * P[1][1]);
    K[1][1] = factor * (a * P[1][1] - b * P[1][0]);
    // updated state
    d_range_est = d_range_est + K[0][0] * (d_range_meas - d_range_est) +
                  K[0][1] * (d_velocity_meas - d_velocity_est);
    d_velocity_est = d_velocity_est + K[1][0] * (d_range_meas - d_range_est) +
                     K[1][1] * (d_velocity_meas - d_velocity_est);
    // update state-covariance matrix
    P[0][0] = (1 - K[0][0]) * P[0][0] - P[1][0] * K[0][1];
    P[0][1] = (1 - K[0][0]) * P[0][1] - P[1][1] * K[0][1];
    P[1][0] = (1 - K[1][1]) * P[1][0] - P[0][0] * K[1][0];
    P[1][1] = (1 - K[1][1]) * P[1][1] - P[0][1] * K[1][0];
} // end kalman-filter

void // begin particle filter
tracking_singletarget_impl::filter_particle()
{
    float lh, range_dif, velocity_dif;
    float sum_weight, sum_weight_square;
    sum_weight = 0;
    sum_weight_square = 0;
    for (int k = 0; k < d_num_particle; k++) {
        // sample particles from time prosecution
        d_particle_range[k] = random_normal(
            d_particle_range[k] - d_particle_velocity[k] * d_delta_t, std::sqrt(Q[0][0]));
        d_particle_velocity[k] =
            random_normal(d_particle_velocity[k], std::sqrt(Q[1][1]));
        range_dif = d_range_meas - d_particle_range[k];
        velocity_dif = d_velocity_meas - d_particle_velocity[k];
        lh = std::exp(-0.5 * (range_dif * range_dif * R_inv[0][0] +
                              velocity_dif * velocity_dif * R_inv[1][1])) /
             2 / GR_M_PI / std::sqrt(R_det); // get likelihood
        d_particle_weight[k] = d_particle_weight[k] * lh;
        // calc sum of weights and sum of weight square
        sum_weight = sum_weight + d_particle_weight[k];
        sum_weight_square =
            sum_weight_square + d_particle_weight[k] * d_particle_weight[k];
    }
    // normalize particle-weight ( sum(particle_weight =! 1))
    for (int k = 0; k < d_num_particle; k++)
        d_particle_weight[k] = d_particle_weight[k] / sum_weight;
    float n_eff = 1 / (float)sum_weight_square; // measure of degeneracy
    if (n_eff <
        d_num_particle / (float)2) { // if degeneracy is to high --> systematic resampling
        int num_range = 1000000;
        float u1 =
            (rand() % num_range) / (float)num_range; // sample random starting index ...
        u1 = u1 / d_num_particle;                    // ... and normalize
        std::vector<float> cum_weight;
        cum_weight.resize(d_num_particle);
        cum_weight[0] = d_particle_weight[0];
        for (int k = 1; k < d_num_particle; k++) { // calc cummulativ sum of all particles
            cum_weight[k] = cum_weight[k - 1] + d_particle_weight[k];
        }
        int i = 0;
        std::vector<float> u;
        u.resize(d_num_particle);
        std::vector<int> index;
        index.resize(d_num_particle);
        for (int k = 0; k < d_num_particle; k++) {
            u[k] = u1 + k / (float)d_num_particle;
            while (u[k] > cum_weight[i]) {
                i++;
            }
            index[k] = i; // chose particle indexes with high weight
        }
        for (int k = 0; k < d_num_particle; k++) {
            d_particle_range[k] =
                d_particle_range[index[k]]; // resample only those particles with high
                                            // weight ...
            d_particle_velocity[k] = d_particle_velocity[index[k]];
            d_particle_weight[k] =
                1 /
                (float)d_num_particle; // ... and set particle_weight to the same value
        }
    }

    d_range_est = 0;
    d_velocity_est = 0;
    // calc mean of all weighted particles to get the estimated state
    for (int k = 0; k < d_num_particle; k++) {
        d_range_est += d_particle_range[k] * d_particle_weight[k];
        d_velocity_est += d_particle_velocity[k] * d_particle_weight[k];
    }
} // end particle filter

bool tracking_singletarget_impl::tracking()
{
    bool is_valid = !(d_is_empty); // measurement is valid if vectors are not empty

    // Calc time difference and matrix Q
    d_delta_t = d_time - d_time_last; // time difference
    d_time_last = d_time;

    Q[0][0] = 0.25 * std::pow(d_delta_t, 4) * d_std_accel_sys * d_std_accel_sys;
    Q[0][1] = 0.5 * std::pow(d_delta_t, 3) * d_std_accel_sys * d_std_accel_sys;
    Q[1][0] = 0.5 * std::pow(d_delta_t, 3) * d_std_accel_sys * d_std_accel_sys;
    Q[1][1] = std::pow(d_delta_t, 2) * d_std_accel_sys * d_std_accel_sys;

    // Do tracking
    if (d_is_track) {   // check if track is available
        if (is_valid) { // check if measurement is valid
            float range_dif, velocity_dif;
            range_dif = d_range_meas - d_range_est;
            velocity_dif = d_velocity_meas - d_velocity_est;
            float lh = std::exp(-0.5 * (range_dif * range_dif * R_inv[0][0] +
                                        velocity_dif * velocity_dif * R_inv[1][1])) /
                       2 / GR_M_PI / std::sqrt(R_det); // calc likelihood

            if (d_threshold_track < lh) { // if new sample is accepted as track
                if (d_filter == "particle")
                    filter_particle();
                else if (d_filter == "kalman")
                    filter_kalman();
                else
                    throw std::runtime_error(
                        "No suitable filter found for given identifier.");
                d_lost = 0;
            } else { // if sample is rejected, do simple estimation based on system model
                     // (v=const, R=v*dt)
                d_range_est =
                    d_range_est -
                    d_velocity_est *
                        d_delta_t; // velocity < 0 for movement to radar; v=const.
                d_lost++;
                if (d_lost > d_threshold_lost) {
                    d_is_track = false;
                    return false; // tracking is not successfull
                }
            }
        } else { // if sample is rejected, do simple estimation based on system model
                 // (v=const, R=v*dt)
            d_range_est = d_range_est -
                          d_velocity_est *
                              d_delta_t; // velocity < 0 for movement to radar; v=const.
            d_lost++;
            if (d_lost > d_threshold_lost) {
                d_is_track = false;
                return false; // tracking is not successfull
            }
        }
    } else {            // if track is not available
        if (is_valid) { // if measurement is valid, do initializiation; else do nothing
            if (d_filter == "particle") { // initialize particle filter
                for (int k = 0; k < d_num_particle; k++) {
                    // samle particles from importance density based on first measurement
                    d_particle_range[k] = random_normal(d_range_meas, d_std_range_meas);
                    d_particle_velocity[k] =
                        random_normal(d_velocity_meas, d_std_velocity_meas);
                    d_particle_weight[k] =
                        1 /
                        float(
                            d_num_particle); // set all particle_weights to the same value
                }
            } else if (d_filter == "kalman") { // initialize state-covariance matrix for
                                               // the kalman filter
                P[0][0] = d_std_range_meas * d_std_range_meas;
                P[0][1] = 0;
                P[1][0] = 0;
                P[1][1] = d_std_velocity_meas * d_std_velocity_meas;
            } else
                throw std::runtime_error(
                    "No suitable filter found for given identifier.");
            // use first range & velocity measurement to intialize the state
            d_range_est = d_range_meas;
            d_velocity_est = d_velocity_meas;
            d_is_track = true;
            d_lost = 0;
        } else {
            d_is_track = false;
            d_lost = 0;
            return false;
        }
    }
    return true; // tracking is successfull
}

} /* namespace radar */
} /* namespace gr */
