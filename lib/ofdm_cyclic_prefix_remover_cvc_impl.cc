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

#include "ofdm_cyclic_prefix_remover_cvc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

ofdm_cyclic_prefix_remover_cvc::sptr
ofdm_cyclic_prefix_remover_cvc::make(int fft_len, int cp_len, std::string len_key)
{
    return gnuradio::make_block_sptr<ofdm_cyclic_prefix_remover_cvc_impl>(fft_len, cp_len, len_key);
}

/*
 * The private constructor
 */
ofdm_cyclic_prefix_remover_cvc_impl::ofdm_cyclic_prefix_remover_cvc_impl(
    int fft_len, int cp_len, std::string len_key)
    : gr::tagged_stream_block("ofdm_cyclic_prefix_remover_cvc",
                              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              gr::io_signature::make(1, 1, sizeof(gr_complex) * fft_len),
                              len_key)
{
    d_fft_len = fft_len;
    d_cp_len = cp_len;

    // Set propagation policy
    set_tag_propagation_policy(TPP_DONT); // does not apply on stream tags!
}

/*
 * Our virtual destructor.
 */
ofdm_cyclic_prefix_remover_cvc_impl::~ofdm_cyclic_prefix_remover_cvc_impl() {}

int ofdm_cyclic_prefix_remover_cvc_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    int noutput_items = ninput_items[0] / (d_fft_len + d_cp_len);
    return noutput_items;
}

int ofdm_cyclic_prefix_remover_cvc_impl::work(int noutput_items,
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

    // Set noutput_items
    noutput_items = ninput_items[0] / (d_fft_len + d_cp_len);

    // Update len key tag
    update_length_tags(noutput_items, 0);

    // Remove cyclic prefix and push vectors with len fft_len to output
    for (int k = 0; k < noutput_items; k++) {
        memcpy(out + d_fft_len * k,
               in + d_cp_len + k * (d_fft_len + d_cp_len),
               d_fft_len * sizeof(gr_complex));
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
