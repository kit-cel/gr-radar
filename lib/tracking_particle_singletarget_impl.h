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
      tracking_particle_singletarget_impl(int num_particle, float std_freq_meas, float std_accel);
      ~tracking_particle_singletarget_impl();
      void handle_msg(pmt::pmt_t msg);
      void tracking();
      
      int d_num_particle;
      float d_std_freq_meas, d_std_accel;
      float d_range, d_velocity;
      
      pmt::pmt_t d_port_id_in, d_port_id_out;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TRACKING_PARTICLE_SINGLETARGET_IMPL_H */

