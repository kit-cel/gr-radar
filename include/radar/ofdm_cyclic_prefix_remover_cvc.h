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


#ifndef INCLUDED_RADAR_OFDM_CYCLIC_PREFIX_REMOVER_CVC_H
#define INCLUDED_RADAR_OFDM_CYCLIC_PREFIX_REMOVER_CVC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief <+description of block+>
     * \ingroup radar
     *
     */
    class RADAR_API ofdm_cyclic_prefix_remover_cvc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<ofdm_cyclic_prefix_remover_cvc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::ofdm_cyclic_prefix_remover_cvc.
       *
       * To avoid accidental use of raw pointers, radar::ofdm_cyclic_prefix_remover_cvc's
       * constructor is in a private implementation
       * class. radar::ofdm_cyclic_prefix_remover_cvc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fft_len, int cp_len, std::string len_key="packet_len");
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OFDM_CYCLIC_PREFIX_REMOVER_CVC_H */

