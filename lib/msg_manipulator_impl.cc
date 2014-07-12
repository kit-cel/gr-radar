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
#include "msg_manipulator_impl.h"

namespace gr {
  namespace radar {

    msg_manipulator::sptr
    msg_manipulator::make(std::vector<std::string> symbols, std::vector<float> const_add, std::vector<float> const_mult)
    {
      return gnuradio::get_initial_sptr
        (new msg_manipulator_impl(symbols, const_add, const_mult));
    }

    /*
     * The private constructor
     */
    msg_manipulator_impl::msg_manipulator_impl(std::vector<std::string> symbols, std::vector<float> const_add, std::vector<float> const_mult)
      : gr::block("msg_manipulator",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_symbols = symbols;
		d_const_add = const_add;
		d_const_mult = const_mult;
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&msg_manipulator_impl::handle_msg, this, _1));
		
		// Register output message port
		d_port_id_out = pmt::mp("Msg out");
		message_port_register_out(d_port_id_out);
	}
	
	void
	msg_manipulator_impl::set_const_add(std::vector<float> val){
		d_const_add = val;
	}
	
	void
	msg_manipulator_impl::set_const_mult(std::vector<float> val){
		d_const_mult = val;
	}
	
	void
	msg_manipulator_impl::handle_msg(pmt::pmt_t msg){
		// Go through msg parts and check symbols
		pmt::pmt_t msg_part_symbol, msg_part_value;
		bool symbol_found;
		std::vector<float> value_new;
		std::vector<pmt::pmt_t> store_msg;
		int index;
		for(int k=0; k<pmt::length(msg); k++){
			symbol_found = false;
			msg_part_symbol = pmt::nth(0,pmt::nth(k,msg));
			msg_part_value = pmt::nth(1,pmt::nth(k,msg));
			// Go through given symbols
			for(int p=0; p<d_symbols.size(); p++){
				if(pmt::symbol_to_string(msg_part_symbol)==d_symbols[p]){
					index = p;
					symbol_found = true;
				}
			}
			// Store msg part
			if(symbol_found && pmt::is_f32vector(msg_part_value)){ // use const_add and const_mult if symbol found and value is f32 vector, then store new msg
				value_new = pmt::f32vector_elements(msg_part_value);
				for(int m=0; m<value_new.size(); m++){ // manipulate vector
					value_new[m] = (value_new[m]+d_const_add[index])*d_const_mult[index];
				}
				store_msg.push_back(pmt::list2(msg_part_symbol,pmt::init_f32vector(value_new.size(), value_new)));
			}
			else{ // just store msg if not found
				store_msg.push_back(pmt::nth(k,msg));
			}
		}
		
		// Repack msg
		pmt::pmt_t msg_out;
		if(store_msg.size()==0) throw std::runtime_error("Store msg vector has unvalid length (size=0)");
		msg_out = pmt::list1(store_msg[0]); // init pmt list
		for(int k=1; k<store_msg.size(); k++){ // add pmt list
			msg_out = pmt::list_add(msg_out, store_msg[k]);
		}
		
		// Publish msg
		message_port_pub(d_port_id_out,msg_out);
	}

    /*
     * Our virtual destructor.
     */
    msg_manipulator_impl::~msg_manipulator_impl()
    {
    }

  } /* namespace radar */
} /* namespace gr */

