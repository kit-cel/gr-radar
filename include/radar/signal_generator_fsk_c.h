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
 
#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_FSK_C_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_FSK_C_H

#include <radar/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block generates a signal for FSK radar in baseband. The waveform consists of a signal with an alternating frequency.
     * The packet length for subsequent tagged streams is calculated with two times the samples per single frequency multiplied by the blocks per tag.
     * 
     * samp_rate		:	Signal sample rate \n
     * samp_per_freq	:	Number of samples per frequency until the frequency shifts \n
     * blocks_per_tag	:	A block contains samp_per_freq samples of the low frequency and samp_per_freq samples of the high frequency. The packet length for subsequent tagged stream blocks is calculated with 2*samp_per_freq*blocks_per_tag. \n
     * freq_low			:	Lower frequency in baseband \n
     * freq_high		:	Higher frequency in baseband \n
     * amplitude		:	Signal amplitude \n
     * len_key			:	Packet length key for tagged stream \n
     * 
     * \ingroup radar
     *
     */
    class RADAR_API signal_generator_fsk_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<signal_generator_fsk_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::signal_generator_fsk_c.
       *
       * To avoid accidental use of raw pointers, radar::signal_generator_fsk_c's
       * constructor is in a private implementation
       * class. radar::signal_generator_fsk_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_rate, int samp_per_freq, int blocks_per_tag, float freq_low, float freq_high, float amplitude, const std::string& len_key="packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_FSK_C_H */

