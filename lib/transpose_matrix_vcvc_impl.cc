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

#include "transpose_matrix_vcvc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

transpose_matrix_vcvc::sptr
transpose_matrix_vcvc::make(int vlen_in, int vlen_out, std::string len_key)
{
    return gnuradio::make_block_sptr<transpose_matrix_vcvc_impl>(
        vlen_in, vlen_out, len_key);
}

/*
 * The private constructor
 */
transpose_matrix_vcvc_impl::transpose_matrix_vcvc_impl(int vlen_in,
                                                       int vlen_out,
                                                       std::string len_key)
    : gr::tagged_stream_block("transpose_matrix_vcvc",
                              gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen_in),
                              gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen_out),
                              len_key)
{
    d_vlen_in = vlen_in;
    d_vlen_out = vlen_out;

    // Set propagation policy
    set_tag_propagation_policy(TPP_DONT); // does not apply on stream tags!
}

/*
 * Our virtual destructor.
 */
transpose_matrix_vcvc_impl::~transpose_matrix_vcvc_impl() {}

int transpose_matrix_vcvc_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    int noutput_items = ninput_items[0] * d_vlen_in / d_vlen_out;
    return noutput_items;
}

int transpose_matrix_vcvc_impl::work(int noutput_items,
                                     gr_vector_int& ninput_items,
                                     gr_vector_const_void_star& input_items,
                                     gr_vector_void_star& output_items)
{
    const gr_complex* in = (const gr_complex*)input_items[0];
    gr_complex* out = (gr_complex*)output_items[0];

    // Error handling
    if (ninput_items[0] * float(d_vlen_in) / float(d_vlen_out) -
            ninput_items[0] * d_vlen_in / d_vlen_out !=
        0)
        throw std::runtime_error("vlen_in and vlen_out do not match to packet length");

    // Get all tags, reset offset and push to output
    get_tags_in_range(d_tags, 0, nitems_read(0), nitems_read(0) + 1);
    for (int k = 0; k < d_tags.size(); k++) {
        add_item_tag(
            0, nitems_written(0), d_tags[k].key, d_tags[k].value, d_tags[k].srcid);
    }

    // Set noutput items
    noutput_items = ninput_items[0] * d_vlen_in / d_vlen_out;

    // Update len key tag
    update_length_tags(noutput_items, 0);

    // Reorganize samples
    for (int l = 0; l < d_vlen_in; l++) {           // go through single input vector
        for (int k = 0; k < ninput_items[0]; k++) { // go through all input vectors
            *out++ = in[k * d_vlen_in + l];
        }
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
