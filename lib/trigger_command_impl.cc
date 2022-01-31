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

#include "trigger_command_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

trigger_command::sptr trigger_command::make(std::string command,
                                            std::vector<std::string> identifiers,
                                            std::vector<float> vals_min,
                                            std::vector<float> vals_max,
                                            int block_time)
{
    return gnuradio::make_block_sptr<trigger_command_impl>(command, identifiers, vals_min, vals_max, block_time);
}

/*
 * The private constructor
 */
trigger_command_impl::trigger_command_impl(std::string command,
                                           std::vector<std::string> identifiers,
                                           std::vector<float> vals_min,
                                           std::vector<float> vals_max,
                                           int block_time)
    : gr::block("trigger_command",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_command = command;
    d_identifiers = identifiers;
    d_vals_min = vals_min;
    d_vals_max = vals_max;
    d_block_time = block_time;

    d_last_time = boost::posix_time::microsec_clock::local_time();

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
                    boost::bind(&trigger_command_impl::handle_msg, this, _1));
}

void trigger_command_impl::handle_msg(pmt::pmt_t msg)
{
    // Check if execution has to be blocked
    d_actual_time = boost::posix_time::microsec_clock::local_time();
    d_time_duration = d_actual_time - d_last_time;
    if (d_time_duration.total_milliseconds() > d_block_time) {
        d_last_time = boost::posix_time::microsec_clock::local_time();
    } else {
        return;
    }

    // Go through msg parts and check symbols
    pmt::pmt_t msg_part_symbol, msg_part_value;
    bool execute_command = true;
    for (int k = 0; k < pmt::length(msg); k++) {
        msg_part_symbol = pmt::nth(0, pmt::nth(k, msg));
        msg_part_value = pmt::nth(1, pmt::nth(k, msg));
        // Go through given symbols
        for (int p = 0; p < d_identifiers.size(); p++) {
            if (pmt::symbol_to_string(msg_part_symbol) == d_identifiers[p]) {
                // Go through input if values are a f32vector, else set executing command
                // on false
                if (pmt::is_f32vector(msg_part_value)) {
                    // Go through values and check boundries
                    std::vector<float> values = pmt::f32vector_elements(msg_part_value);
                    for (int m = 0; m < values.size(); m++) {
                        if (values[m] < d_vals_min[p] || values[m] > d_vals_max[p]) {
                            execute_command = false;
                        }
                    }
                }
                // add here more types as else if
                else {
                    execute_command = false;
                }
            }
        }
    }

    // Execute command if values are correct
    if (execute_command) {
        int sys_return = std::system(d_command.c_str());
    }
}

/*
 * Our virtual destructor.
 */
trigger_command_impl::~trigger_command_impl() {}

} /* namespace radar */
} /* namespace gr */
