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

#ifndef INCLUDED_RADAR_QTGUI_SPECTROGRAM_PLOT_IMPL_H
#define INCLUDED_RADAR_QTGUI_SPECTROGRAM_PLOT_IMPL_H

#include <radar/qtgui_spectrogram_plot.h>
#include "spectrogram_plot.h"

namespace gr {
  namespace radar {

    class qtgui_spectrogram_plot_impl : public qtgui_spectrogram_plot
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      qtgui_spectrogram_plot_impl(int vlen, int interval, std::string len_key);
      ~qtgui_spectrogram_plot_impl();
      void run_gui();
      
      int d_argc;
      char *d_argv;
      QApplication *d_qApplication;
      
      int d_vlen, d_interval;
      std::vector<float> d_buffer;
      spectrogram_plot* d_main_gui;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTGUI_SPECTROGRAM_PLOT_IMPL_H */

