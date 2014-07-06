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
 
#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_H

#include <radar/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block generates a signal for FMCW radar in baseband. The modulation is structured in a CW part with constant frequency and a following up- and down-chirp.
     * The chirp goes from CW frequency to CW frequency plus sweep frequency and back to the CW frequency. The CW part is first, up- and down-chirp is following.
     * The packet length for subsequent tagged streams is calculated by the sum of the number of samples of the single modulations parts.
     * 
     * samp_rate		:	Signal sample rate \n
     * samp_up			:	Number samples of up-chirp part \n
     * samp_down		:	Number samples of down-chirp part \n
     * samp_cw			:	Number samples of CW part \n
     * freq_cw			:	CW signal frequency in baseband \n
     * freq_sweep		:	Sweep frequency of up- and down-chirp \n
     * amplitude		: 	Signal amplitude \n
     * len_key			:	Packet length key for tagged stream \n
     * 
     * \ingroup radar
     *
     */
    class RADAR_API signal_generator_fmcw_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<signal_generator_fmcw_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::signal_generator_fmcw_c.
       *
       * To avoid accidental use of raw pointers, radar::signal_generator_fmcw_c's
       * constructor is in a private implementation
       * class. radar::signal_generator_fmcw_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_rate, int samp_up, int samp_down, int samp_cw, float freq_cw, float freq_sweep, float amplitude, const std::string& len_key="packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_H */

