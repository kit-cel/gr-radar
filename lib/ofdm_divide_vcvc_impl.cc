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
#include "ofdm_divide_vcvc_impl.h"

namespace gr {
  namespace radar {

    ofdm_divide_vcvc::sptr
    ofdm_divide_vcvc::make(int vlen_in, int vlen_out, std::vector<int> discarded_carriers, std::string len_key)
    {
      return gnuradio::get_initial_sptr
        (new ofdm_divide_vcvc_impl(vlen_in, vlen_out, discarded_carriers, len_key));
    }

    /*
     * The private constructor
     */
    ofdm_divide_vcvc_impl::ofdm_divide_vcvc_impl(int vlen_in, int vlen_out, std::vector<int> discarded_carriers, std::string len_key)
      : gr::tagged_stream_block("ofdm_divide_vcvc",
              gr::io_signature::make(2, 2, sizeof(gr_complex)*vlen_in),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*vlen_out), len_key)
    {
		d_vlen_in = vlen_in;
		d_vlen_out = vlen_out;
		d_discarded_carriers = discarded_carriers;
		
		// Error handling
		if(d_vlen_out<d_vlen_in) throw std::runtime_error("Input vector length is greater than output vector length");
	}

    /*
     * Our virtual destructor.
     */
    ofdm_divide_vcvc_impl::~ofdm_divide_vcvc_impl()
    {
    }

    int
    ofdm_divide_vcvc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0];
      return noutput_items ;
    }

    int
    ofdm_divide_vcvc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in0 = (const gr_complex *) input_items[0];
        const gr_complex *in1 = (const gr_complex *) input_items[1];
        gr_complex *out = (gr_complex *) output_items[0];

        // Set noutput_items
        noutput_items = ninput_items[0];
        
        // FIXME: Implement discarded carriers
        
        // Set output buffer to zero -> is zeropadding
		std::memset(out,0,sizeof(gr_complex)*noutput_items*d_vlen_out);
		
		// Do division and keep spaces between packets if vlen_out>vlen_in
		for(int k=0; k<noutput_items; k++){
			for(int l=0; l<d_vlen_in; l++){
				out[k*d_vlen_out+l] = (in0[k*d_vlen_in+l])/(in1[k*d_vlen_in+l]);
			}
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

