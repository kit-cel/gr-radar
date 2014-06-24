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
#include "tracking_particle_singletarget_impl.h"

namespace gr {
  namespace radar {

    tracking_particle_singletarget::sptr
    tracking_particle_singletarget::make(int num_particle, float std_freq_meas, float std_accel)
    {
      return gnuradio::get_initial_sptr
        (new tracking_particle_singletarget_impl(num_particle, std_freq_meas, std_accel));
    }

    /*
     * The private constructor
     */
    tracking_particle_singletarget_impl::tracking_particle_singletarget_impl(int num_particle, float std_freq_meas, float std_accel)
      : gr::block("tracking_particle_singletarget",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_num_particle = num_particle;
		d_std_freq_meas = std_freq_meas;
		d_std_accel = std_accel;
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&tracking_particle_singletarget_impl::handle_msg, this, _1));
		
		// Register output message port
		d_port_id_out = pmt::mp("Msg out");
		message_port_register_out(d_port_id_out);
	}

    /*
     * Our virtual destructor.
     */
    tracking_particle_singletarget_impl::~tracking_particle_singletarget_impl()
    {
    }
    
    void
    tracking_particle_singletarget_impl::handle_msg(pmt::pmt_t msg)
    {
		size_t size_msg;
		bool range_found, velocity_found;
		range_found = false;
		velocity_found = false;
		std::vector<pmt::pmt_t> store_msg;
		
		size_msg = pmt::length(msg);
		pmt::pmt_t msg_part;
		// Go through msg and search for key symbols "range" and "velocity" (or other keys!) and get data
		// This is singletarget tracker! Taken only first item of vector
		// FIXME: error handling for false input? e.g. multi target input (throw warning)
		for(int k=0; k<size_msg; k++){ // FIXME: errorhandling for wrong input?
			msg_part = pmt::nth(k,msg);
			if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="range"){
				d_range = (pmt::f32vector_elements(pmt::nth(1,msg_part)))[0];
				range_found = true;
			}
			else if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="velocity"){
				d_velocity = (pmt::f32vector_elements(pmt::nth(1,msg_part)))[0];
				velocity_found = true;
			}
			else{
				// Store msg part for repacking if no symbol is found
				store_msg.push_back(msg_part);
			}
		}
		
		// Do tracking if both variables are found, else throw error
		if(range_found&&velocity_found) tracking();
		else throw std::runtime_error("range or velocity not found");
		
		// Repack msg
		pmt::pmt_t msg_out;
		if(store_msg.size()!=0){ // if msgs are stored add them
			msg_out = pmt::list1(store_msg[0]);
			for(int k=1; k<store_msg.size(); k++){
				msg_out = pmt::list_add(msg_out, store_msg[k]);
			}
		}
		pmt::pmt_t msg_val;
		std::vector<float> f32_val;
		// Add velocity
		f32_val.push_back(d_velocity);
		msg_val = pmt::list2(pmt::string_to_symbol("velocity"),pmt::init_f32vector(f32_val.size(), f32_val));
		if(store_msg.size()!=0){ // if a pmt list is created before
			msg_out = pmt::list_add(msg_out, msg_val);
		}else{ // if msg_out is not a pmt list
			msg_out = pmt::list1(msg_val);
		}
		// Add range
		f32_val.clear();
		f32_val.push_back(d_range);
		msg_val = pmt::list2(pmt::string_to_symbol("range"),pmt::init_f32vector(f32_val.size(), f32_val));
		msg_out = pmt::list_add(msg_out, msg_val);
		
		// Publish msg
		message_port_pub(d_port_id_out,msg_out);
    }
    
    void
    tracking_particle_singletarget_impl::tracking(){
		// Overwrite d_range and d_velocity with tracked values
	}

  } /* namespace radar */
} /* namespace gr */

