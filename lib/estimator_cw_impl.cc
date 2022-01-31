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

#include "estimator_cw_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

estimator_cw::sptr estimator_cw::make(float center_freq)
{
    return gnuradio::make_block_sptr<estimator_cw_impl>(center_freq);
}

/*
 * The private constructor
 */
estimator_cw_impl::estimator_cw_impl(float center_freq)
    : gr::block("estimator_cw",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_center_freq = center_freq;

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
estimator_cw_impl::~estimator_cw_impl() {}

void estimator_cw_impl::handle_msg(pmt::pmt_t msg)
{
    // Read msg from peak detector
    pmt::pmt_t msg_part;
    for (int k = 0; k < pmt::length(msg); k++) {
        msg_part = pmt::nth(k, msg);
        if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "frequency") {
            d_pfreq = pmt::nth(1, msg_part);
        } else if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == "rx_time") {
            d_ptimestamp = pmt::nth(1, msg_part);
        }
    }

    d_freq = pmt::f32vector_elements(d_pfreq); // get freq vector

    // Calc velocities and write to vector
    d_vel.clear();
    for (int k = 0; k < d_freq.size(); k++) {
        d_vel.push_back(d_freq[k] * c_light / 2 /
                        d_center_freq); // calc with doppler formula
    }

    // Push pmt to output msg port
    d_vel_key = pmt::string_to_symbol("velocity");          // identifier velocity
    d_vel_value = pmt::init_f32vector(d_vel.size(), d_vel); // vector to pmt
    d_vel_pack = pmt::list2(d_vel_key, d_vel_value); // make list for velocity information

    d_time_key = pmt::string_to_symbol("rx_time"); // identifier timestamp
    d_time_pack = pmt::list2(
        d_time_key, d_ptimestamp); // make list for timestamp (rx_time) information

    d_value = pmt::list2(d_time_pack, d_vel_pack); // all information to one pmt list
                                                   // (only velocity -> only 1 item list)
    message_port_pub(d_port_id_out, d_value);
}

} /* namespace radar */
} /* namespace gr */
