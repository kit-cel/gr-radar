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

#ifndef INCLUDED_RADAR_QTQUI_RANGE_VELOCITY_IMPL_H
#define INCLUDED_RADAR_QTQUI_RANGE_VELOCITY_IMPL_H

#include <radar/qtqui_range_velocity.h>

#include <QApplication> 
#include <qwt_plot.h> 
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>

namespace gr {
  namespace radar {

    class qtqui_range_velocity_impl : public qtqui_range_velocity
    {
     private:
      // Nothing to declare in this block.

     public:
      qtqui_range_velocity_impl(std::vector<float> axis_range, std::vector<float> axis_velocity);
      ~qtqui_range_velocity_impl();
      void handle_msg(pmt::pmt_t msg);
      void gui_thread();
      
      std::vector<float> d_axis_range, d_axis_velocity;
      
      int d_argc;
	  char *d_argv;
	  
	  QApplication *d_qapp;
	  QwtPlot *d_plot;
	  QwtSymbol *d_symbol;
	  QwtPlotGrid *d_grid;
	  std::vector<QwtPlotMarker*> d_marker;
	  
	  std::vector<float> d_range, d_velocity;
	  gr::thread::thread d_thread;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTQUI_RANGE_VELOCITY_IMPL_H */

