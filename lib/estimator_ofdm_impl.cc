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

#include "estimator_ofdm_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

estimator_ofdm::sptr estimator_ofdm::make(std::string symbol_x,
                                          int len_x,
                                          std::vector<float> axis_x,
                                          std::string symbol_y,
                                          int len_y,
                                          std::vector<float> axis_y,
                                          bool merge_consecutive)
{
    return gnuradio::get_initial_sptr(new estimator_ofdm_impl(
        symbol_x, len_x, axis_x, symbol_y, len_y, axis_y, merge_consecutive));
}

/*
 * The private constructor
 */
estimator_ofdm_impl::estimator_ofdm_impl(std::string symbol_x,
                                         int len_x,
                                         std::vector<float> axis_x,
                                         std::string symbol_y,
                                         int len_y,
                                         std::vector<float> axis_y,
                                         bool merge_consecutive)
    : gr::block("estimator_ofdm",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_symbol_x = symbol_x;
    d_symbol_y = symbol_y;
    d_len_x = len_x;
    d_len_y = len_y;
    d_axis_x = axis_x;
    d_axis_y = axis_y;
    d_merge_consecutive = merge_consecutive;

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
                    boost::bind(&estimator_ofdm_impl::handle_msg, this, _1));

    // Register output message port
    d_port_id_out = pmt::mp("Msg out");
    message_port_register_out(d_port_id_out);
}

/*
 * Our virtual destructor.
 */
estimator_ofdm_impl::~estimator_ofdm_impl() {}

void estimator_ofdm_impl::handle_msg(pmt::pmt_t msg)
{
    // Read msg from peak detector
    pmt::pmt_t msg_part;
    for (int k = 0; k < pmt::length(msg); k++) {
        msg_part = pmt::nth(k, msg);
        if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "axis_x") {
            d_paxis_x = pmt::nth(1, msg_part);
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "axis_y") {
            d_paxis_y = pmt::nth(1, msg_part);
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "power") {
            d_ppower = pmt::nth(1, msg_part);
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "rx_time") {
            d_ptimestamp = msg_part;
        }
    }

    d_val_x = pmt::f32vector_elements(d_paxis_x);
    d_val_y = pmt::f32vector_elements(d_paxis_y);

    // Merge consecutive peaks
    std::vector<float> val_x_hold, val_y_hold;
    val_x_hold = d_val_x;
    val_y_hold = d_val_y;
    if (d_merge_consecutive) {
        d_val_power = pmt::f32vector_elements(d_ppower);
        for (int k = 0; k < d_val_x.size();
             k++) { // Go through points and check bins in range one (also diagonal!) if
                    // the power is greater then the CUT
            for (int l = 0; l < d_val_x.size(); l++) {
                if (std::abs(d_val_x[k] - d_val_x[l]) <= 1 &&
                    std::abs(d_val_y[k] - d_val_y[l]) <= 1 &&
                    d_val_power[k] < d_val_power[l]) {
                    val_x_hold[k] = -1;
                    val_y_hold[k] = -1;
                }
            }
        }
        d_val_x.clear();
        d_val_y.clear();
        for (int k = 0; k < val_x_hold.size(); k++) {
            if (val_x_hold[k] != -1 && val_y_hold[k] != -1) {
                d_val_x.push_back(val_x_hold[k]);
                d_val_y.push_back(val_y_hold[k]);
            }
        }
    }

    // Map bins on given axis
    // 4 values given splits in the middle of the axis and interpolates linear in between
    // 2 values given represents min and max with linear progression in between
    d_map_x.clear();
    d_map_y.clear();
    for (int k = 0; k < d_val_x.size(); k++) { // x axis
        if (d_axis_x.size() == 2) {
            d_map_x.push_back(d_axis_x[0] +
                              d_val_x[k] / d_len_x * (d_axis_x[1] - d_axis_x[0]));
        } else if (d_axis_x.size() == 4) {
            if (d_val_x[k] < d_len_x / 2) { // lower part
                d_map_x.push_back(d_axis_x[0] + d_val_x[k] / (float)d_len_x * 2.0 *
                                                    (d_axis_x[1] - d_axis_x[0]));
            } else { // upper part
                d_map_x.push_back(d_axis_x[2] + (d_val_x[k] - d_len_x / 2.0) /
                                                    (float)d_len_x * 2.0 *
                                                    (d_axis_x[3] - d_axis_x[2]));
            }
        }
    }

    for (int k = 0; k < d_val_y.size(); k++) { // y axis
        if (d_axis_y.size() == 2) {
            d_map_y.push_back(d_axis_y[0] +
                              d_val_y[k] / d_len_y * (d_axis_y[1] - d_axis_y[0]));
        } else if (d_axis_y.size() == 4) {
            if (d_val_y[k] < d_len_y / 2) { // lower part
                d_map_y.push_back(d_axis_y[0] + d_val_y[k] / (float)d_len_y * 2.0 *
                                                    (d_axis_y[1] - d_axis_y[0]));
            } else { // upper part
                d_map_y.push_back(d_axis_y[2] + (d_val_y[k] - d_len_y / 2.0) /
                                                    (float)d_len_y * 2.0 *
                                                    (d_axis_y[3] - d_axis_y[2]));
            }
        }
    }

    // Push pmt to output msg port
    d_x_key = pmt::string_to_symbol(d_symbol_x);              // identifier x axis
    d_x_value = pmt::init_f32vector(d_map_x.size(), d_map_x); // vector to pmt
    d_x_pack = pmt::list2(d_x_key, d_x_value); // make list for x axis information

    d_y_key = pmt::string_to_symbol(d_symbol_y);              // identifier y axis
    d_y_value = pmt::init_f32vector(d_map_y.size(), d_map_y); // vector to pmt
    d_y_pack = pmt::list2(d_y_key, d_y_value); // make list for y axis information

    d_value = pmt::list3(d_ptimestamp, d_x_pack, d_y_pack);

    message_port_pub(d_port_id_out, d_value); // publish message
}

} /* namespace radar */
} /* namespace gr */
