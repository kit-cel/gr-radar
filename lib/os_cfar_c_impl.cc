//* -*- c++ -*- */
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
#include "os_cfar_c_impl.h"

namespace gr {
  namespace radar {

    os_cfar_c::sptr
    os_cfar_c::make(int samp_rate, int samp_compare, int samp_protect, float rel_threshold, float mult_threshold, bool merge_consecutive, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new os_cfar_c_impl(samp_rate, samp_compare, samp_protect, rel_threshold, mult_threshold, merge_consecutive, len_key));
    }

    /*
     * The private constructor
     */
    os_cfar_c_impl::os_cfar_c_impl(int samp_rate, int samp_compare, int samp_protect, float rel_threshold, float mult_threshold, bool merge_consecutive, const std::string& len_key)
      : gr::tagged_stream_block("os_cfar_c",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0), len_key)
    {
		d_samp_rate = samp_rate;
		d_samp_compare = samp_compare;
		d_samp_protect = samp_protect;
		d_rel_threshold = rel_threshold;
		d_mult_threshold = mult_threshold;
		d_consecutive = false;
		d_merge_consecutive = merge_consecutive;
		
		// Register message port
		d_port_id = pmt::mp("Msg out");
		message_port_register_out(d_port_id);
	}
	
	void os_cfar_c_impl::set_rel_threshold(float inp){ d_rel_threshold = inp; }
	void os_cfar_c_impl::set_mult_threshold(float inp){ d_mult_threshold = inp; }
	void os_cfar_c_impl::set_samp_compare(int inp){ d_samp_compare = inp; }
	void os_cfar_c_impl::set_samp_protect(int inp){ d_samp_protect = inp; }

    /*
     * Our virtual destructor.
     */
    os_cfar_c_impl::~os_cfar_c_impl()
    {
    }

    int
    os_cfar_c_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }

    int
    os_cfar_c_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];

        // Do <+signal processing+>
        
        // OS-CFAR detection
        d_freq.clear();
        d_pks.clear();
        d_angle.clear();
		
        for(int k=0; k<ninput_items[0]; k++){ // go through input
			d_hold_samp.clear();
			for(int l=1; l<d_samp_compare+1; l++){ // go through samples to compare, care: num of samp_compare is doubled! redef if too confusing
				if(k-l-d_samp_protect<0){ // push-back zeros for underflows
					d_hold_samp.push_back(0);
				}
				else{ // push back abs-square
					d_hold_samp.push_back(std::pow(std::abs(in[k-l-d_samp_protect]),2));
				}
				if(k+l+d_samp_protect>=ninput_items[0]){ // push-back zeros for overflows
					d_hold_samp.push_back(0);
				}
				else{ // push back abs-square
					d_hold_samp.push_back(std::pow(std::abs(in[k+l+d_samp_protect]),2));
				}
			}
			std::sort(d_hold_samp.begin(),d_hold_samp.end()); // sort sample vector
			
			if(std::pow(std::abs(in[k]),2)>d_hold_samp[(int)((2*d_samp_compare-1)*d_rel_threshold)]*d_mult_threshold){ // check if in[k] is over dynamic threshold multiplied with mult_threshold
				// Add peaks, frequencies and angle
				if(k<=ninput_items[0]/2){
					d_freq.push_back(k*(d_samp_rate/(float)ninput_items[0])); // add frequency to message vector d_freq
				}
				else{
					d_freq.push_back(-(float)d_samp_rate+k*(d_samp_rate/(float)ninput_items[0]));
				}
				d_pks.push_back(pow(abs(in[k]),2)); // add abs-square to message vector d_pks
				d_angle.push_back(std::arg(in[k])); // add angle to message vector d_angle
				
				// Merge consecutive peaks
				if( (d_merge_consecutive && d_consecutive) && d_freq.size()>1){
					if(d_pks[d_pks.size()-2]<d_pks[d_pks.size()-1]){ // if last peak is lower, remove last one
						d_pks.erase(d_pks.end()-2); // vec.end() points past the end!
						d_freq.erase(d_freq.end()-2);
						d_angle.erase(d_angle.end()-2);
					}
					else{ // if last peak is higher, remove actual one
						d_pks.erase(d_pks.end()-1);
						d_freq.erase(d_freq.end()-1);
						d_angle.erase(d_angle.end()-1);
					}
				}
				
				d_consecutive = true; // tag next loop cycle with check for merging peaks
			}
			else{
				d_consecutive = false; // release consecutive peaks merger if no peak is detected
			}
			
		}
		
		// get rx_time tag
		get_tags_in_range(d_tags,0,nitems_read(0),nitems_read(0)+1,pmt::string_to_symbol("rx_time"));
		
		// setup msg pmt
		if(d_tags.size()>0) d_ptimestamp = d_tags[0].value;
		else d_ptimestamp = pmt::make_tuple(pmt::from_uint64(0),pmt::from_double(-1)); // if no timetag is found, set to 0 and frac_sec to -1
		d_pfreq = pmt::init_f32vector(d_freq.size(),d_freq);
		d_ppks = pmt::init_f32vector(d_pks.size(),d_pks);
		d_pangle = pmt::init_f32vector(d_angle.size(),d_angle);
		d_value = pmt::list4(d_ptimestamp,d_pfreq,d_ppks,d_pangle);
		
		// publish message
		message_port_pub(d_port_id,d_value);

        // Tell runtime system how many output items we produced.
        return 0;
    }

  } /* namespace radar */
} /* namespace gr */

