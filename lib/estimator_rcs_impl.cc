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

#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <boost/circular_buffer.hpp>
#include "estimator_rcs_impl.h"
#include <numeric>

namespace gr {
namespace radar {

estimator_rcs::sptr estimator_rcs::make(int num_mean,
                                        float center_freq,
                                        float antenna_gain_tx,
                                        float antenna_gain_rx,
                                        float usrp_gain_rx,
                                        float power_tx,
                                        float corr_factor,
                                        float exponent)
{
    return gnuradio::get_initial_sptr(new estimator_rcs_impl(num_mean,
                                                             center_freq,
                                                             antenna_gain_tx,
                                                             antenna_gain_rx,
                                                             usrp_gain_rx,
                                                             power_tx,
                                                             corr_factor,
                                                             exponent));
}

/*
 * The private constructor
 */
estimator_rcs_impl::estimator_rcs_impl(int num_mean,
                                       float center_freq,
                                       float antenna_gain_tx,
                                       float antenna_gain_rx,
                                       float usrp_gain_rx,
                                       float power_tx,
                                       float corr_factor,
                                       float exponent)
    : gr::block("estimator_rcs",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_num_mean = num_mean;
    d_center_freq = center_freq;
    d_antenna_gain_tx = antenna_gain_tx;
    d_antenna_gain_rx = antenna_gain_rx;
    d_usrp_gain_rx = usrp_gain_rx;
    d_power_tx = power_tx; // needs to be calibrated for every usage
    d_corr_factor = corr_factor;
    d_exponent = exponent;

    d_rcs_vals.resize(d_num_mean);

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in, boost::bind(&estimator_rcs_impl::handle_msg, this, _1));

    // Register output message port
    d_port_id_out = pmt::mp("Msg out");
    message_port_register_out(d_port_id_out);

    d_loop_counter = 0;

    // constant factors in radar equation
    d_antenna_gain_abs_rx = pow(10, d_antenna_gain_rx / 10);
    d_antenna_gain_abs_tx = pow(10, d_antenna_gain_tx / 10);
    d_lambda = c_light / d_center_freq;
    d_fak = pow(4.0 * GR_M_PI, 3) /
            (d_antenna_gain_abs_rx * d_antenna_gain_abs_tx * pow(d_lambda, 2));
}

/*
 * Our virtual destructor.
 */
estimator_rcs_impl::~estimator_rcs_impl() {}

void estimator_rcs_impl::set_num_mean(int val)
{
    d_num_mean = val;
    d_rcs_vals.clear();
    d_rcs_vals.resize(d_num_mean);
    d_loop_counter = 0;
}

void estimator_rcs_impl::set_center_freq(float val)
{
    d_center_freq = val;
    d_lambda = c_light / d_center_freq;
    d_fak = pow(4.0 * GR_M_PI, 3) /
            (d_antenna_gain_abs_rx * d_antenna_gain_abs_tx * pow(d_lambda, 2));
}

void estimator_rcs_impl::set_antenna_gain_tx(float val)
{
    d_antenna_gain_tx = val;
    d_antenna_gain_abs_tx = pow(10, d_antenna_gain_tx / 10);
    d_fak = pow(4.0 * GR_M_PI, 3) /
            (d_antenna_gain_abs_rx * d_antenna_gain_abs_tx * pow(d_lambda, 2));
}

void estimator_rcs_impl::set_antenna_gain_rx(float val)
{
    d_antenna_gain_rx = val;
    d_antenna_gain_abs_rx = pow(10, d_antenna_gain_rx / 10);
    d_fak = pow(4.0 * GR_M_PI, 3) /
            (d_antenna_gain_abs_rx * d_antenna_gain_abs_tx * pow(d_lambda, 2));
}

void estimator_rcs_impl::set_usrp_gain_rx(float val) { d_usrp_gain_rx = val; }

void estimator_rcs_impl::set_power_tx(float val) { d_power_tx = val; }

void estimator_rcs_impl::set_corr_factor(float val) { d_corr_factor = val; }

float estimator_rcs_impl::calculate_vector_mean(boost::circular_buffer<float>* rcs_vals)
{
    float sum_of_elems = 0;
    for (int k = 0; k < rcs_vals->size(); k++) {
        sum_of_elems += (*rcs_vals)[k];
    }
    return sum_of_elems / rcs_vals->size();
}

float estimator_rcs_impl::calculate_rcs()
{
    // catch errors
    if (d_range.size() == 0)
        throw std::runtime_error("range vector has size zero");
    if (d_power.size() == 0)
        throw std::runtime_error("power vector has size zero");

    // regard usrp gain and signal path
    float power_rx =
        pow(d_power[0], d_exponent) / d_power_tx / pow(10, d_usrp_gain_rx / 10);

    float fak = d_fak * pow(d_range[0], 4);

    // debug output
    // std::cout << "PowerTx: " << d_power_tx << std::endl;
    // std::cout << "PowerRx: " << power_rx << std::endl;
    // std::cout << "Lambda: " << d_lambda << std::endl;
    // std::cout << "GainRx: " << d_antenna_gain_rx << std::endl;
    // std::cout << "GainTx: " << d_antenna_gain_tx << std::endl;
    // std::cout << "fak: " << fak << std::endl;

    return power_rx / d_power_tx * fak * d_corr_factor;
}

void estimator_rcs_impl::handle_msg(pmt::pmt_t msg)
{
    // Read msg from peak detector
    d_msg_hold.clear();
    pmt::pmt_t msg_part;
    bool found_range = false;
    bool found_power = false;
    for (int k = 0; k < pmt::length(msg); k++) {
        msg_part = pmt::nth(k, msg);
        if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "range") {
            d_prange = pmt::nth(1, msg_part);
            d_msg_hold.push_back(msg_part);
            found_range = true;
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "power") {
            d_ppower = pmt::nth(1, msg_part);
            found_power = true;
        } else {
            d_msg_hold.push_back(msg_part);
        }
    }

    if (!(found_range && found_power))
        throw std::runtime_error("range or power identifier (symbol) not found");

    d_range = pmt::f32vector_elements(d_prange);
    d_power = pmt::f32vector_elements(d_ppower);
    d_rcs.clear();

    if (d_range.size() != d_power.size())
        throw std::runtime_error("range and power vectors do not have same size");

    // Calculate RCS
    float rcs_mean = 0.0;
    if (d_range.size() == 0 && d_power.size() == 0) {
        // std::cout << "ERROR: No target detected for RCS calculation" << std::endl;
    } else {
        d_rcs_vals.push_back(calculate_rcs());
    }
    // std::cout << "RCS_TEMP: " << calculate_rcs() << std::endl;

    if (d_loop_counter + 1 >= d_num_mean) {
        rcs_mean = calculate_vector_mean(&d_rcs_vals);
    } else {
        d_loop_counter++;
    }

    for (int k = 0; k < d_range.size(); k++) {
        d_rcs.push_back(rcs_mean);
    }

    // Push pmt to output msg port
    d_rcs_key = pmt::string_to_symbol("rcs");               // identifier velocity
    d_rcs_value = pmt::init_f32vector(d_rcs.size(), d_rcs); // vector to pmt
    d_rcs_pack = pmt::list2(d_rcs_key, d_rcs_value); // make list for velocity information

    d_value = pmt::list1(d_rcs_pack);
    for (int k = 0; k < d_msg_hold.size(); k++) {
        d_value = pmt::list_add(d_value, d_msg_hold[k]);
    }

    message_port_pub(d_port_id_out, d_value); // publish message
}

} /* namespace radar */
} /* namespace gr */
