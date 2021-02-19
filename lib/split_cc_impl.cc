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

#include "split_cc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

split_cc::sptr split_cc::make(int packet_num,
                              const std::vector<int> packet_parts,
                              const std::string& len_key)
{
    return gnuradio::get_initial_sptr(
        new split_cc_impl(packet_num, packet_parts, len_key));
}

/*
 * The private constructor
 */
split_cc_impl::split_cc_impl(int packet_num,
                             const std::vector<int> packet_parts,
                             const std::string& len_key)
    : gr::tagged_stream_block("split_cc",
                              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              len_key)
{
    // Set key for info pmt and store packet_part identifier
    d_packet_parts = packet_parts;
    d_packet_num = packet_num;

    set_tag_propagation_policy(TPP_DONT); // does not apply on stream tags!
}

/*
 * Our virtual destructor.
 */
split_cc_impl::~split_cc_impl() {}

int split_cc_impl::calculate_output_stream_length(const gr_vector_int& ninput_items)
{
    int noutput_items = ninput_items[0];
    return noutput_items;
}

int split_cc_impl::work(int noutput_items,
                        gr_vector_int& ninput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items)
{
    const gr_complex* in = (const gr_complex*)input_items[0];
    gr_complex* out = (gr_complex*)output_items[0];

    // Get all tags, reset offset and push to output
    get_tags_in_range(d_tags, 0, nitems_read(0), nitems_read(0) + 1);
    for (int k = 0; k < d_tags.size(); k++) {
        add_item_tag(
            0, nitems_written(0), d_tags[k].key, d_tags[k].value, d_tags[k].srcid);
    }

    // Push only part of input stream to output and resize length tag value
    update_length_tags(d_packet_parts[d_packet_num], 0); // update length tag
    noutput_items = d_packet_parts[d_packet_num];        // get num output items

    d_offset = 0; // calc offset in stream
    for (int k = 0; k < d_packet_num; k++)
        d_offset += d_packet_parts[k];
    for (int k = 0; k < noutput_items; k++)
        out[k] = in[k + d_offset]; // push items to output

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
