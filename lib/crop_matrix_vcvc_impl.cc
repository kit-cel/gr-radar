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

#include "crop_matrix_vcvc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

crop_matrix_vcvc::sptr crop_matrix_vcvc::make(int vlen,
                                              std::vector<int> crop_x,
                                              std::vector<int> crop_y,
                                              std::string len_key)
{
    return gnuradio::make_block_sptr<crop_matrix_vcvc_impl>(
        vlen, crop_x, crop_y, len_key);
}

/*
 * The private constructor
 */
crop_matrix_vcvc_impl::crop_matrix_vcvc_impl(int vlen,
                                             std::vector<int> crop_x,
                                             std::vector<int> crop_y,
                                             std::string len_key)
    : gr::tagged_stream_block(
          "crop_matrix_vcvc",
          gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen),
          gr::io_signature::make(1, 1, sizeof(gr_complex) * (crop_x[1] - crop_x[0])),
          len_key)
{
    d_vlen = vlen;
    d_crop_x = crop_x;
    d_crop_y = crop_y;

    set_tag_propagation_policy(TPP_DONT); // does not apply on stream tags!
}

/*
 * Our virtual destructor.
 */
crop_matrix_vcvc_impl::~crop_matrix_vcvc_impl() {}

int crop_matrix_vcvc_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    int noutput_items = d_crop_y[1] - d_crop_y[0];
    return noutput_items;
}

int crop_matrix_vcvc_impl::work(int noutput_items,
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

    // Push cropped data to output
    for (int n = d_crop_y[0]; n < d_crop_y[1]; n++) {
        for (int m = d_crop_x[0]; m < d_crop_x[1]; m++) {
            *out++ = in[d_vlen * n + m];
        }
    }

    // Set noutput items and new tagged stream length
    noutput_items = d_crop_y[1] - d_crop_y[0];
    update_length_tags(noutput_items, 0);

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
