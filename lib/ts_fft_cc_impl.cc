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

#include "ts_fft_cc_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>

namespace gr {
namespace radar {

ts_fft_cc::sptr ts_fft_cc::make(int packet_len, const std::string& len_key)
{
    return gnuradio::make_block_sptr<ts_fft_cc_impl>(packet_len, len_key);
}

/*
 * The private constructor
 */
ts_fft_cc_impl::ts_fft_cc_impl(int packet_len, const std::string& len_key)
    : gr::tagged_stream_block("ts_fft_cc",
                              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              len_key)
{
    d_packet_len = packet_len;
    d_buffer = (fftwf_complex*)fftwf_malloc(sizeof(fftw_complex) * d_packet_len);

    // Check datatype
    if (sizeof(gr_complex) != sizeof(fftw_complex))
        std::runtime_error("sizeof(gr_complex)!=sizeof(fftw_complex)");

    // Setup plan
    // d_fft_plan = fftwf_plan_dft_1d(d_packet_len, reinterpret_cast<fftwf_complex
    // *>(&d_buffer[0]), reinterpret_cast<fftwf_complex *>(&d_buffer[0]), FFTW_FORWARD,
    // FFTW_ESTIMATE);
    d_fft_plan =
        fftwf_plan_dft_1d(d_packet_len, d_buffer, d_buffer, FFTW_FORWARD, FFTW_ESTIMATE);
}

/*
 * Our virtual destructor.
 */
ts_fft_cc_impl::~ts_fft_cc_impl()
{
    fftwf_destroy_plan(d_fft_plan);
    fftwf_free(d_buffer);
}

int ts_fft_cc_impl::calculate_output_stream_length(const gr_vector_int& ninput_items)
{
    int noutput_items = ninput_items[0];
    return noutput_items;
}

int ts_fft_cc_impl::work(int noutput_items,
                         gr_vector_int& ninput_items,
                         gr_vector_const_void_star& input_items,
                         gr_vector_void_star& output_items)
{
    gr_complex* in = (gr_complex*)input_items[0];
    gr_complex* out = (gr_complex*)output_items[0];

    // Set output to one packet (defined with tagged stream)
    noutput_items = ninput_items[0];

    // Execute fft plan
    memcpy(d_buffer, in, d_packet_len * sizeof(gr_complex));
    fftwf_execute(d_fft_plan);
    memcpy(out, d_buffer, d_packet_len * sizeof(gr_complex));

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
