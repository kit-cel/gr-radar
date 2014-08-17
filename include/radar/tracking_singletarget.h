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


#ifndef INCLUDED_RADAR_TRACKING_SINGLETARGET_H
#define INCLUDED_RADAR_TRACKING_SINGLETARGET_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block tracks a singletarget detection with a particle or kalman filter. As input values with identifiers 'range' and 'velocity' are needed and should hold a f32vector with only one element. All input variables tagged with std gives the standard deviation of the parameter. The threshold_track is a value which decides with the likelihood of the data if the new data is accepted as a track. A good starting value is threshold_track = 0.001. threshold_lost is the number of false tracks unitel the track is lost and the tracker begins with a new one. The string filter decides which tracking kernel should be used. 'kalman' or 'particle' are valid. If 'particle' is chosen num_particle gives the number of particles for the particle filter. If 'kalman' is chosen there is no effect on the tracker.
     * 
     * \param num_particle Number of particles for particle filter. There is no effect if kalman filter is chosen.
     * \param std_range_meas Standard deviation of the range measurement
     * \param std_velocity_meas Standard deviation of the velocity measurement
     * \param std_accel_sys Standard deviation of the system acceleration
     * \param threshold_track Value to decide if data is valid for the track
     * \param theshold_lost Number of false tracks until the current track is lost
     * \param filter Filter kernel to be used. 'kalman' or 'particle' are valid.
     * 
     * \ingroup radar
     *
     */
    class RADAR_API tracking_singletarget : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<tracking_singletarget> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::tracking_singletarget.
       *
       * To avoid accidental use of raw pointers, radar::tracking_singletarget's
       * constructor is in a private implementation
       * class. radar::tracking_singletarget::make is the public interface for
       * creating new instances.
       */
      static sptr make(int num_particle, float std_range_meas, float std_velocity_meas, float std_accel_sys, float threshold_track, int threshold_lost, std::string filter);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TRACKING_SINGLETARGET_H */

