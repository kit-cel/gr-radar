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
    qtgui_rv_diagram::make(int interval, std::vector<float> axis_range, std::vector<float> axis_velocity)
    {
      return gnuradio::get_initial_sptr
        (new qtgui_rv_diagram_impl(interval, axis_range, axis_velocity));
    }

    /*
     * The private constructor
     */
    qtgui_rv_diagram_impl::qtgui_rv_diagram_impl(int interval, std::vector<float> axis_range, std::vector<float> axis_velocity)
      : gr::block("qtgui_rv_diagram",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_interval = interval;
		d_axis_x = axis_range;
		d_axis_y = axis_velocity;
		d_label_x = "range";
		d_label_y = "velocity";
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&qtgui_rv_diagram_impl::handle_msg, this, _1));
		
		// Setup GUI
		run_gui();
	}
	
	void
    qtgui_rv_diagram_impl::handle_msg(pmt::pmt_t msg)
    {
		size_t size_msg;
		d_x.clear();
		d_y.clear();
		
		size_msg = pmt::length(msg);
		pmt::pmt_t msg_part;
		bool item_found_x, item_found_y;
		// Go through msg and search for key symbols "range" and "velocity" (or other keys) and get data
		for(int k=0; k<size_msg; k++){ // FIXME: errorhandling for wrong input?
			item_found_x = false;
			item_found_y = false;
			msg_part = pmt::nth(k,msg);
			if(pmt::symbol_to_string(pmt::nth(0,msg_part))==d_label_x.c_str()){
				d_x = pmt::f32vector_elements(pmt::nth(1,msg_part));
				item_found_x = true;
			}
			else if(pmt::symbol_to_string(pmt::nth(0,msg_part))==d_label_y.c_str()){
				d_y = pmt::f32vector_elements(pmt::nth(1,msg_part));
				item_found_y = true;
			}
		}
		// Fill zeros in vector if only one label is found
		if(~item_found_x&&item_found_y){
			d_x.resize(d_y.size());
		}
		if(item_found_x&&~item_found_y){
			d_y.resize(d_x.size());
		}
		if(~item_found_x&&~item_found_y){
			// FIXME: throw exception, no label found
		}
    }
    
    void
    qtgui_rv_diagram_impl::run_gui(){
		// Set QT window
		if(qApp != NULL) {
			d_qApplication = qApp;
		}
		else {
			d_qApplication = new QApplication(d_argc, &d_argv);
		}
		
		// Set QWT plot widget
		d_main_gui = new scatter_plot(d_interval, d_axis_x, d_axis_y, &d_x, &d_y, d_label_x, d_label_y);
		d_main_gui->show();
	}

    /*
     * Our virtual destructor.
     */
    qtgui_rv_diagram_impl::~qtgui_rv_diagram_impl()
    {
    }

  } /* namespace radar */
} /* namespace gr */

