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
#include "estimator_msg_gate_impl.h"

namespace gr {
  namespace radar {

    estimator_msg_gate::sptr
    estimator_msg_gate::make(std::vector<std::string> keys, std::vector<float> val_min, std::vector<float> val_max, int verbose)
    {
      return gnuradio::get_initial_sptr
        (new estimator_msg_gate_impl(keys, val_min, val_max, verbose));
    }

    /*
     * The private constructor
     */
    estimator_msg_gate_impl::estimator_msg_gate_impl(std::vector<std::string> keys, std::vector<float> val_min, std::vector<float> val_max, int verbose)
      : gr::block("estimator_msg_gate",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_keys = keys;
		d_val_min = val_min;
		d_val_max = val_max;
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&estimator_msg_gate_impl::handle_msg, this, _1));
		
		// Register output message port
		d_port_id_out = pmt::mp("Msg out");
		message_port_register_out(d_port_id_out);
	}
	
	void
	estimator_msg_gate_impl::handle_msg(pmt::pmt_t msg){
		d_msg_size = pmt::length(msg);
		d_msg_part_hold.clear();
		d_msg_part_hold.resize(0);
		
		for(int k=0; k<d_msg_size; k++){ // go through msg
			d_msg_part = pmt::nth(k,msg);
			d_msg_key = pmt::symbol_to_string(pmt::nth(0,d_msg_part)); // get key
			d_key_found = false;
			for(int l=0; l<d_keys.size(); l++){ // search for key in input keys
				if(d_msg_key==d_keys[l]){
					// If key is found check for vector and repack msg
					d_key_found = true;
					if(pmt::is_f32vector(pmt::nth(1,d_msg_part))){ // only f32 vector works
						d_vec_size = pmt::length(pmt::nth(1,d_msg_part));
						d_val_store.clear();
						d_val_store.resize(d_vec_size);
						d_val_store = pmt::f32vector_elements(pmt::nth(1,d_msg_part));
						// Check if element is in boundries
						d_val_accept.clear();
						d_val_accept.resize(0);
						for(int m=0; m<d_vec_size; m++){
							if((d_val_store[m]>=d_val_min[l])&&(d_val_store[m]<=d_val_max[l])){
								d_val_accept.push_back(d_val_store[m]);
							}
						}
					}
					else{
						1; // FIXME: throw warning for wrong datatype
					}
				} // end repacking
			} // end checking keys
			
			// Store pmt
			if(d_key_found){
				d_msg_part_hold.push_back(pmt::list2(pmt::nth(0,d_msg_part),pmt::init_f32vector(d_val_accept.size(),d_val_accept))); // push back new pmt if key is found
			}
			else{
				d_msg_part_hold.push_back(d_msg_part); // push back old msg part if not
			}
			
		} // end check msg
		
		// Rebuild msg and push to output
		pmt::pmt_t msg_out;
		if(d_msg_part_hold.size()==1){
			msg_out = list1(d_msg_part_hold[0]);
		}
		else if(d_msg_part_hold.size()==2){
			msg_out = pmt::list2(d_msg_part_hold[0],d_msg_part_hold[1]);
		}
		else if(d_msg_part_hold.size()==3){
			msg_out = pmt::list3(d_msg_part_hold[0],d_msg_part_hold[1],d_msg_part_hold[2]);
		}
		message_port_pub(d_port_id_out,msg_out);
	}

    /*
     * Our virtual destructor.
     */
    estimator_msg_gate_impl::~estimator_msg_gate_impl()
    {
    }

  } /* namespace radar */
} /* namespace gr */

