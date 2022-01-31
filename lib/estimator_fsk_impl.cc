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

#include "estimator_fsk_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>

namespace gr {
namespace radar {

estimator_fsk::sptr
estimator_fsk::make(float center_freq, float delta_freq, bool push_power)
{
    return gnuradio::make_block_sptr<estimator_fsk_impl>(center_freq, delta_freq, push_power);
}

/*
 * The private constructor
 */
estimator_fsk_impl::estimator_fsk_impl(float center_freq,
                                       float delta_freq,
                                       bool push_power)
    : gr::block("estimator_fsk",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_center_freq = center_freq;
    d_delta_freq = delta_freq;
    d_push_power = push_power;

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in, [this](pmt::pmt_t msg) { this->handle_msg(msg); });

    // Register output message port
    d_port_id_out = pmt::mp("Msg out");
    message_port_register_out(d_port_id_out);
}

/*
 * Our virtual destructor.
 */
estimator_fsk_impl::~estimator_fsk_impl() {}

void estimator_fsk_impl::handle_msg(pmt::pmt_t msg)
{
    // Read msg from peak detector
    pmt::pmt_t msg_part;
    for (int k = 0; k < pmt::length(msg); k++) {
        msg_part = pmt::nth(k, msg);
        if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "frequency") {
            d_pfreq = pmt::nth(1, msg_part);
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "phase") {
            d_pphase = pmt::nth(1, msg_part);
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "power") {
            d_ppower = msg_part;
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "rx_time") {
            d_ptimestamp = msg_part;
        }
    }

    d_freq = pmt::f32vector_elements(d_pfreq);
    d_phase = pmt::f32vector_elements(d_pphase);

    // Check read data
    if (d_phase.size() == 0 && d_freq.size() != 0)
        std::runtime_error("Frequency but no phase found in message");

    // Calc velocities and write to vector
    d_vel.clear();
    for (int k = 0; k < d_freq.size(); k++) {
        d_vel.push_back(d_freq[k] * c_light / 2 /
                        d_center_freq); // calc with doppler formula
    }

    // Calc ranges and write to vector
    d_range.clear();
    for (int k = 0; k < d_phase.size(); k++) {
        if (d_phase[k] >= 0)
            d_range.push_back((d_phase[k]) * c_light / 4 / GR_M_PI /
                              d_delta_freq); // calc with fsk range formula
        else if (d_phase[k] < 0)
            d_range.push_back((2 * GR_M_PI + d_phase[k]) * c_light / 4 / GR_M_PI /
                              d_delta_freq); // phase jumps from pi to -pi
    }

    // Push pmt to output msg port
    d_vel_key = pmt::string_to_symbol("velocity");          // identifier velocity
    d_vel_value = pmt::init_f32vector(d_vel.size(), d_vel); // vector to pmt
    d_vel_pack = pmt::list2(d_vel_key, d_vel_value); // make list for velocity information

    d_range_key = pmt::string_to_symbol("range");                 // identifier range
    d_range_value = pmt::init_f32vector(d_range.size(), d_range); // vector to pmt
    d_range_pack =
        pmt::list2(d_range_key, d_range_value); // make list for range information

    if (d_push_power) { // if power of peaks shall be pushed through
        d_value = pmt::list4(d_ptimestamp,
                             d_vel_pack,
                             d_range_pack,
                             d_ppower); // all information to one pmt list
    } else {
        d_value = pmt::list3(d_ptimestamp, d_vel_pack, d_range_pack);
    }

    message_port_pub(d_port_id_out, d_value); // publish message
}

} /* namespace radar */
} /* namespace gr */
