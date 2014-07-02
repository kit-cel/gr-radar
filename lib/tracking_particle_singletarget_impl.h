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

#ifndef INCLUDED_RADAR_TRACKING_PARTICLE_SINGLETARGET_IMPL_H
#define INCLUDED_RADAR_TRACKING_PARTICLE_SINGLETARGET_IMPL_H

#include <radar/tracking_particle_singletarget.h>

namespace gr {
  namespace radar {

    class tracking_particle_singletarget_impl : public tracking_particle_singletarget
    {
     private:
      // Nothing to declare in this block.

     public:
      tracking_particle_singletarget_impl(int num_particle, float std_range_meas, float std_velocity_meas, float std_accel_sys, float threshold_track, int threshold_lost);
      ~tracking_particle_singletarget_impl();
      void handle_msg(pmt::pmt_t msg);
      bool tracking();
      void filter();
      float random_normal(float mean, float std);
      
      int d_num_particle;
      float d_std_range_meas, d_std_velocity_meas, d_std_accel_sys;
      float d_range, d_velocity, d_time;
      
      pmt::pmt_t d_port_id_in, d_port_id_out;
      
      float d_range_last, d_velocity_last, d_time_last;
      float d_delta_t;
      int d_lost;
      bool d_is_track, d_is_empty;
      std::vector<std::vector<float> > Q, R, R_inv;
      std::vector<float> d_particle_range, d_particle_velocity, d_particle_weight;
      float R_det;
      float d_threshold_track;
      int d_threshold_lost;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TRACKING_PARTICLE_SINGLETARGET_IMPL_H */

