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
#include "estimator_fmcw_impl.h"
#include <iostream>

namespace gr {
  namespace radar {

    estimator_fmcw::sptr
    estimator_fmcw::make(int samp_rate, float center_freq, float sweep_freq, int samp_up, int samp_down, bool push_power)
    {
      return gnuradio::get_initial_sptr
        (new estimator_fmcw_impl(samp_rate, center_freq, sweep_freq, samp_up, samp_down, push_power));
    }

    /*
     * The private constructor
     */
    estimator_fmcw_impl::estimator_fmcw_impl(int samp_rate, float center_freq, float sweep_freq, int samp_up, int samp_down, bool push_power)
      : gr::block("estimator_fmcw",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_samp_rate = samp_rate;
		d_center_freq = center_freq;
		d_sweep_freq = sweep_freq;
		d_samp_up = samp_up;
		d_samp_down = samp_down;
		d_push_power = push_power;
		
		d_msg_cw_in = false;
		d_msg_up_in = false;
		d_msg_down_in = false;
		
		// Setup estimation constants
		d_const_doppler = 2*d_center_freq/c_light;
		d_const_up = 2*d_sweep_freq/c_light*(float)d_samp_rate/(float)d_samp_up;
		d_const_down = 2*d_sweep_freq/c_light*(float)d_samp_rate/(float)d_samp_down;
		
		// Register input message ports
		d_port_id_in_cw = pmt::mp("Msg in CW");
		message_port_register_in(d_port_id_in_cw);
		set_msg_handler(d_port_id_in_cw, boost::bind(&estimator_fmcw_impl::handle_msg_cw, this, _1));
		
		d_port_id_in_up = pmt::mp("Msg in UP");
		message_port_register_in(d_port_id_in_up);
		set_msg_handler(d_port_id_in_up, boost::bind(&estimator_fmcw_impl::handle_msg_up, this, _1));
		
		d_port_id_in_down = pmt::mp("Msg in DOWN");
		message_port_register_in(d_port_id_in_down);
		set_msg_handler(d_port_id_in_down, boost::bind(&estimator_fmcw_impl::handle_msg_down, this, _1));
		
		// Register output message port
		d_port_id_out = pmt::mp("Msg out");
		message_port_register_out(d_port_id_out);
	}
	
	void
    estimator_fmcw_impl::handle_msg_cw(pmt::pmt_t msg)
    {
		// Handle CW msg and call estimate if all msgs are available
		d_msg_cw = msg;
		d_msg_cw_in = true;
		if(d_msg_cw_in&&d_msg_up_in&&d_msg_down_in){
			d_msg_cw_in = false;
			d_msg_up_in = false;
			d_msg_down_in = false;
			estimate();
		}
	}
	
	void
    estimator_fmcw_impl::handle_msg_up(pmt::pmt_t msg)
    {
		// Handle UP msg and call estimate if all msgs are available
		d_msg_up = msg;
		d_msg_up_in = true;
		if(d_msg_cw_in&&d_msg_up_in&&d_msg_down_in){
			d_msg_cw_in = false;
			d_msg_up_in = false;
			d_msg_down_in = false;
			estimate();
		}
	}
	
	void
    estimator_fmcw_impl::handle_msg_down(pmt::pmt_t msg)
    {
		// Handle DOWN msg and call estimate if all msgs are available
		d_msg_down = msg;
		d_msg_down_in = true;
		if(d_msg_cw_in&&d_msg_up_in&&d_msg_down_in){
			d_msg_cw_in = false;
			d_msg_up_in = false;
			d_msg_down_in = false;
			estimate();
		}
	}
	
	void
    estimator_fmcw_impl::estimate()
    {
		// Get timestamp and frequencies (cw, up-chirp, down-chirp)
		std::vector<float> freq_cw, freq_up, freq_down;
		pmt::pmt_t timestamp;
		pmt::pmt_t msg_part;
		pmt::pmt_t power;
		
		for(int k=0; k<pmt::length(d_msg_cw); k++){ // search freq CW
			msg_part = pmt::nth(k,d_msg_cw);
			if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="frequency"){
				freq_cw = pmt::f32vector_elements(pmt::nth(1,msg_part));
			}
			else if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="rx_time"){
				timestamp = pmt::nth(1,msg_part);
			}
			else if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="power"){
				power = msg_part;
			}
		}
		
		for(int k=0; k<pmt::length(d_msg_up); k++){ // search freq UP
			msg_part = pmt::nth(k,d_msg_up);
			if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="frequency"){
				freq_up = pmt::f32vector_elements(pmt::nth(1,msg_part));
			}
		}
		
		for(int k=0; k<pmt::length(d_msg_down); k++){ // search freq DOWN
			msg_part = pmt::nth(k,d_msg_down);
			if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="frequency"){
				freq_down = pmt::f32vector_elements(pmt::nth(1,msg_part));
			}
		}
		
		// Get velocities out of CW frequencies
		std::vector<float> velocity_cw;
		for(int k=0; k<freq_cw.size(); k++){
			velocity_cw.push_back(-c_light/2/d_center_freq*freq_cw[k]); // with minus! for same sign as all_velocities
		}
		
		// Get all possible range/velocity pairs
		std::vector<float> all_ranges, all_velocities;
		float v1, v2, r;
		for(int m=0; m<freq_up.size(); m++){
			for(int n=0; n<freq_down.size(); n++){
				r = (freq_up[m]-freq_down[n])/(d_const_up+d_const_down); // range from up- and down-chirp
				all_ranges.push_back(r);
				v1 = (d_const_up*r-freq_up[m])/d_const_doppler; // velocity from up-chirp
				v2 = (-d_const_down*r-freq_down[n])/d_const_doppler; // velocity from down-chirp
				all_velocities.push_back((v1+v2)/2.0); // median of up- and down-chirp
			}
		}
		
		// Minimize velocity from CW with all possible velocities from up-/down-chirp
		std::vector<float> velocity, range;
		int min_vel_index;
		float min_vel;
		for(int k=0; k<velocity_cw.size(); k++){
			min_vel = 1e20;
			min_vel_index = -1;
			
			for(int l=0; l<all_velocities.size(); l++){
				if(min_vel>std::abs(velocity_cw[k]-all_velocities[l])){
					min_vel = std::abs(velocity_cw[k]-all_velocities[l]);
					min_vel_index = l;
				}
			}
			
			velocity.push_back(velocity_cw[k]); // push back cw velocity because of (probably) better resolution
			range.push_back(all_ranges[min_vel_index]);
		}
		
		// Pack output msg and push to output
		pmt::pmt_t time_pack;
		time_pack = pmt::list2(pmt::string_to_symbol("rx_time"), timestamp); // make list for timestamp information
		
		pmt::pmt_t vel_value, vel_pack;
		vel_value = pmt::init_f32vector(velocity.size(), velocity); // vector to pmt
		vel_pack = pmt::list2(pmt::string_to_symbol("velocity"), vel_value); // make list for velocity information
		
		pmt::pmt_t range_value, range_pack;
		range_value = pmt::init_f32vector(range.size(), range); // vector to pmt
		range_pack = pmt::list2(pmt::string_to_symbol("range"), range_value); // make list for range information
		
		pmt::pmt_t value;
		if(d_push_power){ // if power of peaks shall be pushed through
			value = pmt::list4(time_pack, vel_pack, range_pack, power); // all information to one pmt list
		}
		else{
			value = pmt::list3(time_pack, vel_pack, range_pack); // all information to one pmt list
		}
		message_port_pub(d_port_id_out,value);
	}

    /*
     * Our virtual destructor.
     */
    estimator_fmcw_impl::~estimator_fmcw_impl()
    {
    }

  } /* namespace radar */
} /* namespace gr */

