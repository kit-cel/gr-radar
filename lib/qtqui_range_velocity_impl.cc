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
		d_axis_range = axis_range;
		d_axis_velocity = axis_velocity;
		pmt::pmt_t d_port_id_in;
		
		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&qtqui_range_velocity_impl::handle_msg, this, _1));
		
		// Setup QWT
		d_argc = 1;
		d_argv = new char;
		d_argv[0] = '\0';
		
		if(qApp!=NULL){ // prevents crash if grc uses qt gui
			d_qapp = qApp;
		}
		else{
			d_qapp = new QApplication(d_argc, &d_argv);
		}
		d_plot = new QwtPlot;
		d_symbol = new QwtSymbol(QwtSymbol::Diamond,Qt::red,Qt::NoPen,QSize(20,20));
		d_grid = new QwtPlotGrid;
		
		d_plot->setTitle(QwtText("Range Velocity Diagram")); 
		d_plot->setGeometry(0,0,600,600);
		d_plot->setAxisScale(QwtPlot::xBottom,d_axis_range[0],d_axis_range[1]);
		d_plot->setAxisTitle(QwtPlot::xBottom, "Range");
		d_plot->setAxisScale(QwtPlot::yLeft,d_axis_velocity[0],d_axis_velocity[1]);
		d_plot->setAxisTitle(QwtPlot::yLeft, "Velocity");
		
		d_grid->setPen(QPen(QColor(119,136,153),0.5, Qt::DashLine));
		d_grid->attach(d_plot);
		
		d_plot->show();
		
		// Spawn GUI thread with QThread
		d_thread = gr::thread::thread(boost::bind(&qtqui_range_velocity_impl::gui_thread, this));
		boost::this_thread::sleep(boost::posix_time::milliseconds(100)); // wait for spawning thread
	}

    /*
     * Our virtual destructor.
     */
    qtqui_range_velocity_impl::~qtqui_range_velocity_impl()
    {
    }
    
    void
    qtqui_range_velocity_impl::gui_thread(){
		// Replot with attached markers
		while(1){
			d_plot->replot();
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	}
    
    void
    qtqui_range_velocity_impl::handle_msg(pmt::pmt_t msg){
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
		
		// Clear plot from old markers
		for(int k=0; k<d_marker.size(); k++) d_marker[k]->detach();
		
		// Set new marker
		for(int k=0; k<d_velocity.size(); k++){ // len(velocity)==len(range)
			if(k<d_marker.size()){
				d_marker[k]->setValue(QPointF(d_range[k],d_velocity[k]));
				d_marker[k]->attach(d_plot);
			}
			else{
				d_marker.push_back(new QwtPlotMarker);
				d_marker[k]->setSymbol(d_symbol);
				d_marker[k]->setValue(QPointF(d_range[k],d_velocity[k]));
				d_marker[k]->attach(d_plot);
			}
		}
	}

  } /* namespace radar */
} /* namespace gr */

