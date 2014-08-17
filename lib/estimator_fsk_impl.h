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
 
#ifndef INCLUDED_RADAR_ESTIMATOR_FSK_IMPL_H
#define INCLUDED_RADAR_ESTIMATOR_FSK_IMPL_H

#include <radar/estimator_fsk.h>

namespace gr {
  namespace radar {

    class estimator_fsk_impl : public estimator_fsk
    {
     private:
      // Nothing to declare in this block.

     public:
      estimator_fsk_impl(float center_freq, float delta_freq, bool push_power);
      ~estimator_fsk_impl();
      void handle_msg(pmt::pmt_t msg);
      
      float d_center_freq, d_delta_freq;
      bool d_push_power;
      pmt::pmt_t d_port_id_in, d_port_id_out;
      
      std::vector<float> d_freq, d_phase;
	  pmt::pmt_t d_ptimestamp, d_pfreq, d_pphase, d_ppower;
	  
	  std::vector<float> d_vel;
	  pmt::pmt_t d_value;
	  pmt::pmt_t d_vel_key, d_vel_value, d_vel_pack;
	  std::vector<float> d_range;
	  pmt::pmt_t d_range_key, d_range_value, d_range_pack;
      
      const static float c_light = 3e8;
      
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_FSK_IMPL_H */

