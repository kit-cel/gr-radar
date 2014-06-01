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
#include "print_peaks_impl.h"
#include <iostream>

namespace gr {
  namespace radar {

    print_peaks::sptr
    print_peaks::make()
    {
      return gnuradio::get_initial_sptr
        (new print_peaks_impl());
    }

    /*
     * The private constructor
     */
    print_peaks_impl::print_peaks_impl()
      : gr::block("print_peaks",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&print_peaks_impl::handle_msg, this, _1));
	}

    /*
     * Our virtual destructor.
     */
    print_peaks_impl::~print_peaks_impl()
    {
    }
    
    void
    print_peaks_impl::handle_msg(pmt::pmt_t msg)
    {
		std::cout << "// Print peaks" << std::endl;
		d_timestamp = pmt::nth(0,msg);
		d_freq = pmt::nth(1,msg);
		d_pks = pmt::nth(2,msg);
		d_angle = pmt::nth(3,msg);
		d_len_vec = pmt::length(d_freq);
		
		std::cout << "Timestamp: " << pmt::to_uint64(pmt::tuple_ref(d_timestamp,0)) << ":" << pmt::to_double(pmt::tuple_ref(d_timestamp,1)) << std::endl;
		
		std::cout << "Frequencies: ";
		for(int k=0; k<d_len_vec; k++) std::cout << pmt::f32vector_elements(d_freq,d_len_vec)[k] << " ";
		std::cout << std::endl;
		
		std::cout << "Power: ";
		for(int k=0; k<d_len_vec; k++) std::cout << pmt::f32vector_elements(d_pks,d_len_vec)[k] << " ";
		std::cout << std::endl;
		
		std::cout << "Angle: ";
		for(int k=0; k<d_len_vec; k++) std::cout << pmt::f32vector_elements(d_angle,d_len_vec)[k] << " ";
		std::cout << std::endl;
		
		std::cout << std::endl;
	}

  } /* namespace radar */
} /* namespace gr */

