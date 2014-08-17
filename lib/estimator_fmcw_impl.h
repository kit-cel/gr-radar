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
 
#ifndef INCLUDED_RADAR_ESTIMATOR_FMCW_IMPL_H
#define INCLUDED_RADAR_ESTIMATOR_FMCW_IMPL_H

#include <radar/estimator_fmcw.h>

namespace gr {
  namespace radar {

    class estimator_fmcw_impl : public estimator_fmcw
    {
     private:
      // Nothing to declare in this block.

     public:
      estimator_fmcw_impl(int samp_rate, float center_freq, float sweep_freq, int samp_up, int samp_down, bool push_power);
      ~estimator_fmcw_impl();
      
      void handle_msg_cw(pmt::pmt_t msg);
      void handle_msg_up(pmt::pmt_t msg);
      void handle_msg_down(pmt::pmt_t msg);
      void estimate();
      
      int d_samp_rate;
      float d_center_freq, d_sweep_freq;
      int d_samp_up, d_samp_down;
      bool d_push_power;
      
      float d_const_up, d_const_down, d_const_doppler;
      
      bool d_msg_cw_in, d_msg_up_in, d_msg_down_in;
      
      pmt::pmt_t d_port_id_in_cw, d_port_id_in_up, d_port_id_in_down, d_port_id_out;
      pmt::pmt_t d_msg_cw, d_msg_up, d_msg_down;
      
      const static float c_light = 3e8;

    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_FMCW_IMPL_H */

