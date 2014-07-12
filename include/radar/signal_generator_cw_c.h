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
 
#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_CW_C_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_CW_C_H

#include <radar/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block generates a signal for CW radar in baseband. The waveform consists of one (or sometimes multiple) constant frequency.
     * 
     * \param packet_len packet_len is the length of a single tagged stream package which will be processed in subsequent tagged stream blocks.
     * \param samp_rate Signal sample rate
     * \param frequency This parameter holds a vector of all constant frequencies in baseband.
     * \param amplitude Signal amplitude
     * \param len_key Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API signal_generator_cw_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<signal_generator_cw_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::signal_generator_cw_c.
       *
       * To avoid accidental use of raw pointers, radar::signal_generator_cw_c's
       * constructor is in a private implementation
       * class. radar::signal_generator_cw_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(int packet_len, int samp_rate, std::vector<float> frequency, float amplitude, const std::string& len_key="packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_CW_C_H */

