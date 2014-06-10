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
#include "qtqui_range_velocity_impl.h"

#include <QApplication>

namespace gr {
  namespace radar {

    qtqui_range_velocity::sptr
    qtqui_range_velocity::make(std::vector<float> axis_range, std::vector<float> axis_velocity)
    {
      return gnuradio::get_initial_sptr
        (new qtqui_range_velocity_impl(axis_range, axis_velocity));
    }

    /*
     * The private constructor
     */
    qtqui_range_velocity_impl::qtqui_range_velocity_impl(std::vector<float> axis_range, std::vector<float> axis_velocity)
      : gr::block("qtqui_range_velocity",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
		d_axis_range = d_axis_range;
		d_axis_velocity = d_axis_velocity;
		pmt::pmt_t d_port_id_in;
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&qtqui_range_velocity_impl::handle_msg, this, _1));
		
		// Required now for Qt; argc must be greater than 0 and argv
		// must have at least one valid character. Must be valid through
		// life of the qApplication:
		// http://harmattan-dev.nokia.com/docs/library/html/qt4/qapplication.html
		d_argc = 1;
		d_argv = new char;
		d_argv[0] = '\0';
		
		QApplication *qapp;
		qapp = new QApplication(d_argc, &d_argv);
		qapp->exec();
	}

    /*
     * Our virtual destructor.
     */
    qtqui_range_velocity_impl::~qtqui_range_velocity_impl()
    {
    }
    
    void
    qtqui_range_velocity_impl::handle_msg(pmt::pmt_t msg){
		
	}

  } /* namespace radar */
} /* namespace gr */

