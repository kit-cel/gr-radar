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
 
#ifndef INCLUDED_RADAR_TS_FFT_CC_H
#define INCLUDED_RADAR_TS_FFT_CC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This blocks takes a tagged stream and returns a FFT of the length of the input stream. The output is NOT normalized on the number of input items and no window is used.
     * 
     * \param packet_len Packet length of tagged stream
     * \param len_key Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API ts_fft_cc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<ts_fft_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::ts_fft_cc.
       *
       * To avoid accidental use of raw pointers, radar::ts_fft_cc's
       * constructor is in a private implementation
       * class. radar::ts_fft_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int packet_len, const std::string& len_key = "packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TS_FFT_CC_H */

