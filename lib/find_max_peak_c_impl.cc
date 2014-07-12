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
#include "find_max_peak_c_impl.h"

namespace gr {
  namespace radar {

    find_max_peak_c::sptr
    find_max_peak_c::make(int samp_rate, float threshold, int samp_protect, std::vector<float> max_freq, bool cut_max_freq, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new find_max_peak_c_impl(samp_rate, threshold, samp_protect, max_freq, cut_max_freq, len_key));
    }

    /*
     * The private constructor
     */
    find_max_peak_c_impl::find_max_peak_c_impl(int samp_rate, float threshold, int samp_protect, std::vector<float> max_freq, bool cut_max_freq, const std::string& len_key)
      : gr::tagged_stream_block("find_max_peak_c",
              gr::io_signature::make(1,1,sizeof(gr_complex)),
              gr::io_signature::make(0,0,0),len_key)
    {
		d_samp_rate = samp_rate;
		d_threshold = threshold;
		d_samp_protect = samp_protect;
		d_max_freq = max_freq;
		d_cut_max_freq = cut_max_freq;
		
		// Register message port
		d_port_id = pmt::mp("Msg out");
		message_port_register_out(d_port_id);
	}

    /*
     * Our virtual destructor.
     */
    find_max_peak_c_impl::~find_max_peak_c_impl()
    {
    }

    int
    find_max_peak_c_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }
    
    void
    find_max_peak_c_impl::set_threshold(float threshold)
    {
      d_threshold = threshold;
    }
    
    void
    find_max_peak_c_impl::set_samp_protect(int samp)
    {
      d_samp_protect = samp;
    }
    
    void
    find_max_peak_c_impl::set_max_freq(std::vector<float> freq){
		d_max_freq = freq;
	}

    int
    find_max_peak_c_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];

        // Do <+signal processing+>
        
        // Find max peak detection
        d_freq.clear();
        d_pks.clear();
        d_angle.clear();
        
        int k_max_pos, k_max_neg;
        if(d_cut_max_freq){
			k_max_pos = (ninput_items[0]/float(d_samp_rate))*d_max_freq[1];
			k_max_neg = ninput_items[0]+(ninput_items[0]/float(d_samp_rate))*d_max_freq[0];
		}
        int k = -1;
        float hold = -1;
        for(int p=d_samp_protect; p<ninput_items[0]-d_samp_protect; p++){ // implementation of protected samples
			if(d_cut_max_freq){
				if( p>k_max_pos && p<k_max_neg ){ // skip frequency over abs(max_freq)
					continue;
				}
			}
			if(std::pow(std::abs(in[p]),2)>hold && std::pow(std::abs(in[p]),2)>std::pow(10,d_threshold/10.0)){
				hold = std::pow(std::abs(in[p]),2);
				k = p;
			}
		}
        
        if(k!=-1){
			if(k<=ninput_items[0]/2) d_freq.push_back(k*(d_samp_rate/(float)ninput_items[0])); // add frequency to message vector d_freq
			else d_freq.push_back(-(float)d_samp_rate+k*(d_samp_rate/(float)ninput_items[0]));
			d_pks.push_back(std::pow(std::abs(in[k]),2));
			d_angle.push_back(std::arg(in[k]));
		}

        // get rx_time tag
		get_tags_in_range(d_tags,0,nitems_read(0),nitems_read(0)+1,pmt::string_to_symbol("rx_time"));
		
		// setup msg pmt
		if(d_tags.size()>0) d_ptimestamp = pmt::list2(pmt::string_to_symbol("rx_time"),d_tags[0].value);
		else d_ptimestamp = pmt::list2(pmt::string_to_symbol("rx_time"),pmt::make_tuple(pmt::from_uint64(0),pmt::from_double(-1))); // if no timetag is found, set to 0 and frac_sec to -1
		d_pfreq = pmt::list2(pmt::string_to_symbol("frequency"),pmt::init_f32vector(d_freq.size(),d_freq));
		d_ppks = pmt::list2(pmt::string_to_symbol("peak"),pmt::init_f32vector(d_pks.size(),d_pks));
		d_pangle = pmt::list2(pmt::string_to_symbol("phase"),pmt::init_f32vector(d_angle.size(),d_angle));
		d_value = pmt::list4(d_ptimestamp,d_pfreq,d_ppks,d_pangle);
		
		// publish message
		message_port_pub(d_port_id,d_value);

        // Tell runtime system how many output items we produced.
        return 0;
    }

  } /* namespace radar */
} /* namespace gr */

