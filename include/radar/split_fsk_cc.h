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
 
#ifndef INCLUDED_RADAR_SPLIT_FSK_CC_H
#define INCLUDED_RADAR_SPLIT_FSK_CC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block splits a FSK signal consisting of two separate signals. samp_per_freq items are taken and pushed alternating to the outputs. Discarded samples are thrown away at the beginning of samp_per_freq samples and only samp_per_freq-samp_discard are pushed to output.
     * 
     * \param samp_per_freq Samples per frequency
     * \param samp_discard Discarded samples
     * \param len_key Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API split_fsk_cc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<split_fsk_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::split_fsk_cc.
       *
       * To avoid accidental use of raw pointers, radar::split_fsk_cc's
       * constructor is in a private implementation
       * class. radar::split_fsk_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_per_freq, int samp_discard, const std::string& len_key="packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SPLIT_FSK_CC_H */

