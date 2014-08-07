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


#ifndef INCLUDED_RADAR_OFDM_DIVIDE_VCVC_H
#define INCLUDED_RADAR_OFDM_DIVIDE_VCVC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block performs a complex complex division with in0/in1. If vlen_out > vlen_in the additional space is filled with zeros. This can be used for zeropadding. discarded_carriers is a vector of the carriers which should be not used and set zero as division result. num_sync_words gives the number of sync words on which the discarded_carriers rule is not applied.
     * 
     * \param vlen_in Input vector length
     * \param vlen_out Output vector length
     * \param discarded_carriers Discarded carriers
     * \param num_sync_words Number of sync words
     * \param len_key Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API ofdm_divide_vcvc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<ofdm_divide_vcvc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::ofdm_divide_vcvc.
       *
       * To avoid accidental use of raw pointers, radar::ofdm_divide_vcvc's
       * constructor is in a private implementation
       * class. radar::ofdm_divide_vcvc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int vlen_in, int vlen_out, std::vector<int> discarded_carriers, int num_sync_words, std::string len_key="packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OFDM_DIVIDE_VCVC_H */

