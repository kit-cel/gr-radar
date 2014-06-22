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

#ifndef INCLUDED_RADAR_QTGUI_RV_DIAGRAM_IMPL_H
#define INCLUDED_RADAR_QTGUI_RV_DIAGRAM_IMPL_H

#include <radar/qtgui_rv_diagram.h>
#include "scatter_plot.h"

namespace gr {
  namespace radar {

    class qtgui_rv_diagram_impl : public qtgui_rv_diagram
    {
     private:
      // Nothing to declare in this block.

     public:
      qtgui_rv_diagram_impl(int interval, std::vector<float> axis_range, std::vector<float> axis_velocity);
      ~qtgui_rv_diagram_impl();
      void handle_msg(pmt::pmt_t msg);
      void run_gui();
      
      std::vector<float> d_axis_x, d_axis_y;
      std::vector<float> d_x, d_y;
      std::string d_label_x, d_label_y;
      pmt::pmt_t d_port_id_in;
      
      int d_argc;
      char *d_argv;
      QApplication *d_qApplication;
      
      int d_interval;
      scatter_plot* d_main_gui;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTGUI_RV_DIAGRAM_IMPL_H */

