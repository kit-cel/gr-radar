/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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

#include <gnuradio/io_signature.h>
#include "ts_fft_cc_impl.h"
#include <volk/volk.h>

namespace gr {
  namespace radar {

    ts_fft_cc::sptr
    ts_fft_cc::make(int apply_filter, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new ts_fft_cc_impl(apply_filter, len_key));
    }

    /*
     * The private constructor
     */
    ts_fft_cc_impl::ts_fft_cc_impl(int apply_filter, const std::string& len_key)
      : gr::tagged_stream_block("ts_fft_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), len_key)
    {
		d_size_fft = apply_filter;
		d_buffer.resize(d_size_fft);
		d_fft_plan = fftwf_plan_dft_1d(d_size_fft, reinterpret_cast<fftwf_complex *>(&d_buffer[0]),
			reinterpret_cast<fftwf_complex *>(&d_buffer[0]), FFTW_FORWARD, FFTW_ESTIMATE);
	}

    /*
     * Our virtual destructor.
     */
    ts_fft_cc_impl::~ts_fft_cc_impl()
    {
    }

    int
    ts_fft_cc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0];
      return noutput_items ;
    }

    int
    ts_fft_cc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        gr_complex *in = (gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
        
        // Set output to one packet (defined with tagged stream)
		noutput_items = ninput_items[0];
			
        // Execute fft plan
        memcpy(&d_buffer[0],in,d_size_fft*sizeof(gr_complex));
		fftwf_execute(d_fft_plan);
		memcpy(out,&d_buffer[0],d_size_fft*sizeof(gr_complex));

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

