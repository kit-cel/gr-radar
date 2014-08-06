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
#include "signal_generator_cw_c_impl.h"

namespace gr {
  namespace radar {

    signal_generator_cw_c::sptr
    signal_generator_cw_c::make(int packet_len, int samp_rate, std::vector<float> frequency, float amplitude, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new signal_generator_cw_c_impl(packet_len, samp_rate, frequency, amplitude, len_key));
    }

    /*
     * The private constructor
     */
    signal_generator_cw_c_impl::signal_generator_cw_c_impl(int packet_len, int samp_rate, std::vector<float> frequency, float amplitude, const std::string& len_key)
      : gr::sync_block("signal_generator_cw_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		d_packet_len = packet_len; // packet length of 1 cw packet
		d_samp_rate = samp_rate; // samp rate of the signal
		d_frequency = frequency; // frequency of the cw signal in baseband
		d_amplitude = amplitude; // amplitude of the cw signal
		
		d_key = pmt::string_to_symbol(len_key); // set tag identifier for tagged stream
		d_value = pmt::from_long(packet_len); // set length of 1 cw packet as tagged stream
		d_srcid = pmt::string_to_symbol("sig_gen_cw"); // set block identifier
		d_num_freq = d_frequency.size();
		d_phase.resize(d_num_freq) ; // set start phase to 0
	}

    /*
     * Our virtual destructor.
     */
    signal_generator_cw_c_impl::~signal_generator_cw_c_impl()
    {
    }

    int
    signal_generator_cw_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex *) output_items[0];
        
        // Set output to zero
        std::memset(out, 0, noutput_items*sizeof(gr_complex));
        
        // Integrate phase for iq signal
        for(int i=0; i<noutput_items; i++){
			// Set tag on every packet_len-th item
			if((nitems_written(0)+i)%d_packet_len==0) add_item_tag(0, nitems_written(0)+i, d_key, d_value, d_srcid);
			
			// Write sample
			for(int k=0; k<d_num_freq; k++){ // Go through frequencies
				out[i] += d_amplitude/(float)d_num_freq*exp(d_phase[k]); // output amplitude is normed on d_amplitude
				d_phase[k] = 1j*std::fmod(imag(d_phase[k])+2*M_PI*d_frequency[k]/(float)d_samp_rate,2*M_PI);
			}
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

