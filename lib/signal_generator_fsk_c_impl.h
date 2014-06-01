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
 
#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_FSK_C_IMPL_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_FSK_C_IMPL_H

#include <radar/signal_generator_fsk_c.h>

namespace gr {
  namespace radar {

    class signal_generator_fsk_c_impl : public signal_generator_fsk_c
    {
     private:
      // Nothing to declare in this block.

     public:
      signal_generator_fsk_c_impl(int samp_rate, int samp_per_freq, int blocks_per_tag, float freq_low, float freq_high, float amplitude, const std::string& len_key);
      ~signal_generator_fsk_c_impl();
      
      int d_samp_rate, d_samp_per_freq, d_blocks_per_tag, d_packet_len;
      float d_freq_low, d_freq_high, d_amplitude;
      
      pmt::pmt_t d_key, d_value, d_srcid;
      gr_complex d_phase_high, d_phase_low;
      
      bool d_state;

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_FSK_C_IMPL_H */

