/* -*- c++ -*- */
/* 
 * Copyright 2014 Communications Engineering Lab, KIT.
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
 
#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_IMPL_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_IMPL_H

#include <radar/signal_generator_fmcw_c.h>

namespace gr {
  namespace radar {

    class signal_generator_fmcw_c_impl : public signal_generator_fmcw_c
    {
     private:
      // Nothing to declare in this block.

     public:
      signal_generator_fmcw_c_impl(int samp_rate, int samp_up, int samp_down, int samp_cw, float freq_cw, float freq_sweep, float amplitude, const std::string& len_key);
      ~signal_generator_fmcw_c_impl();
      
      int d_samp_rate, d_samp_up, d_samp_down, d_samp_cw, d_packet_len;
      float d_freq_cw, d_freq_sweep, d_amplitude;
      
      pmt::pmt_t d_key_len, d_value_len, d_srcid;
      std::complex<float> d_phase;
      std::vector<float> d_waveform;
      int d_wv_counter;

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_IMPL_H */

