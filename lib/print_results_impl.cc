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

#include "print_results_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>

namespace gr {
namespace radar {

print_results::sptr print_results::make(bool store_msg, const std::string filename)
{
    return gnuradio::make_block_sptr<print_results_impl>(store_msg, filename);
}

/*
 * The private constructor
 */
print_results_impl::print_results_impl(bool store_msg, const std::string filename)
    : gr::block("print_results",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_filename = filename;
    d_store_msg = store_msg;

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in, [this](pmt::pmt_t msg) { handle_msg(msg); });
}

/*
 * Our virtual destructor.
 */
print_results_impl::~print_results_impl() {}

void print_results_impl::handle_msg(pmt::pmt_t msg)
{
    if (d_store_msg)
        d_file.open(d_filename.c_str(), std::ios::app);

    std::cout << "// Print results" << std::endl;
    if (d_store_msg)
        d_file << "// Print results" << std::endl;
    // Get size of pmt list
    d_size_msg = pmt::length(msg);
    for (int k = 0; k < d_size_msg; k++) {
        d_msg_part = pmt::nth(k, msg);
        // Print identifier (which information)
        std::cout << pmt::symbol_to_string(pmt::nth(0, d_msg_part)) << ": ";
        if (d_store_msg)
            d_file << pmt::symbol_to_string(pmt::nth(0, d_msg_part)) << ": ";
        // Print information, every datatype needs an if-statement!
        if (pmt::is_f32vector(pmt::nth(1, d_msg_part))) {
            d_size_part = pmt::length(pmt::nth(1, d_msg_part));
            for (int l = 0; l < d_size_part; l++) {
                std::cout << pmt::f32vector_elements(pmt::nth(1, d_msg_part),
                                                     d_size_part)[l]
                          << " ";
                if (d_store_msg)
                    d_file << pmt::f32vector_elements(pmt::nth(1, d_msg_part),
                                                      d_size_part)[l]
                           << " ";
            }
        } else if (pmt::is_integer(pmt::nth(1, d_msg_part))) {
            std::cout << pmt::to_long(pmt::nth(1, d_msg_part)) << " ";
            if (d_store_msg)
                d_file << pmt::to_long(pmt::nth(1, d_msg_part)) << " ";
        } else if (pmt::is_tuple(pmt::nth(1, d_msg_part))) {
            std::cout << pmt::to_uint64(pmt::tuple_ref(pmt::nth(1, d_msg_part), 0)) << ":"
                      << pmt::to_double(pmt::tuple_ref(pmt::nth(1, d_msg_part), 1))
                      << " ";
            if (d_store_msg)
                d_file << pmt::to_uint64(pmt::tuple_ref(pmt::nth(1, d_msg_part), 0))
                       << ":"
                       << pmt::to_double(pmt::tuple_ref(pmt::nth(1, d_msg_part), 1))
                       << " ";
        } else {
            std::cout << "Can not identify data type.";
            if (d_store_msg)
                d_file << "Can not identify data type.";
        }
        std::cout << std::endl;
        if (d_store_msg)
            d_file << std::endl;
    }
    std::cout << std::endl;
    if (d_store_msg)
        d_file << std::endl;

    if (d_store_msg)
        d_file.close();
}

} /* namespace radar */
} /* namespace gr */
