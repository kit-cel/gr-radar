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
#include "qtgui_rv_diagram_impl.h"

namespace gr {
  namespace radar {

    qtgui_rv_diagram::sptr
    qtgui_rv_diagram::make(std::vector<float> axis_range, std::vector<float> axis_velocity)
    {
      return gnuradio::get_initial_sptr
        (new qtgui_rv_diagram_impl(axis_range, axis_velocity));
    }

    /*
     * The private constructor
     */
    qtgui_rv_diagram_impl::qtgui_rv_diagram_impl(std::vector<float> axis_range, std::vector<float> axis_velocity)
      : gr::block("qtgui_rv_diagram",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_axis_range = axis_range;
		d_axis_velocity = axis_velocity;
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&qtgui_rv_diagram_impl::handle_msg, this, _1));
	}
	
	void
    qtgui_rv_diagram_impl::handle_msg(pmt::pmt_t msg)
    {
		size_t size_msg;
		d_range.clear();
		d_velocity.clear();
		
		size_msg = pmt::length(msg);
		pmt::pmt_t msg_part;
		// Go through msg and search for key symbols "range" and "velocity" and get data
		for(int k=0; k<size_msg; k++){ // FIXME: errorhandling for wrong input?
			msg_part = pmt::nth(k,msg);
			if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="velocity"){
				d_velocity = pmt::f32vector_elements(pmt::nth(1,msg_part));
			}
			else if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="range"){
				d_range = pmt::f32vector_elements(pmt::nth(1,msg_part));
			}
		}
    }

    /*
     * Our virtual destructor.
     */
    qtgui_rv_diagram_impl::~qtgui_rv_diagram_impl()
    {
    }

  } /* namespace radar */
} /* namespace gr */

