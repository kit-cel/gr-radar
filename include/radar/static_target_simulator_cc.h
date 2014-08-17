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
 
#ifndef INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_H
#define INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block simulates the backscattering of a given signal on point targets. TX signal is given on the input and is tuned in the simulation on the given center frequency. Target parameters are given as f32vectors and each index refers to a target. The vectors range, velocity, rcs, and azimuth must have the same length. Velocity adds the doppler shift on the input signal and range adds a timeshift. Azimuth adds a second timeshift which refers to the distance between TX and RX antenna. All antennas are set on a straight line. The TX antenna takes place on the zero point as reference. The RX antennas are set on the straight line in a distance of position_rx to the TX antenna. The azimuth is measured from the vertical of this line. rndm_phaseshift adds on every target RX signal a random phase. self_coupling toggles the self coupling of the TX signal on the RX signal with self_coupling_db power.
     * 
     * 
     * \param range Target ranges as vector
     * \param velocity Target velocities as vector
     * \param rcs Target RCS as vector
     * \param azimuth Target azimuth as vector
     * \param position_rx Position RX antennas
     * \param samp_rate Sample rate
     * \param center_freq Center frequency
     * \param self_coupling_db Self coupling in dB
     * \param rndm_phaseshift Toggle random phaseshift on targets
     * \param self_coupling Toggle self coupling
     * \param packet_len Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API static_target_simulator_cc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<static_target_simulator_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::static_target_simulator_cc.
       *
       * To avoid accidental use of raw pointers, radar::static_target_simulator_cc's
       * constructor is in a private implementation
       * class. radar::static_target_simulator_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::vector<float> range, std::vector<float> velocity, std::vector<float> rcs, std::vector<float> azimuth, std::vector<float> position_rx,
						int samp_rate, float center_freq, float self_coupling_db, bool rndm_phaseshift=true, bool self_coupling=true, const std::string& len_key="packet_len");
						
	  virtual void setup_targets(std::vector<float> range, std::vector<float> velocity, std::vector<float> rcs, std::vector<float> azimuth, std::vector<float> position_rx,
													int samp_rate, float center_freq, float self_coupling_db, bool rndm_phaseshift, bool self_coupling) = 0;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_H */

