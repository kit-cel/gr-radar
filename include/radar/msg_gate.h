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


#ifndef INCLUDED_RADAR_MSG_GATE_H
#define INCLUDED_RADAR_MSG_GATE_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block blocks messages whose data is not in range of val_min to val_max. All parameters are given as vectors. Each index represents a dataset with given identifier (symbol) which should be tested on valid data in given range. All other data is pushed through.
     * 
     * \param keys Identifiers (symbols) of the data as vector
     * \param val_min Lower limit of the gate as vector
     * \param val_max Higher limit of the gate as vector
     * 
     * \ingroup radar
     *
     */
    class RADAR_API msg_gate : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<msg_gate> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::msg_gate.
       *
       * To avoid accidental use of raw pointers, radar::msg_gate's
       * constructor is in a private implementation
       * class. radar::msg_gate::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::vector<std::string> keys, std::vector<float> val_min, std::vector<float> val_max);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_MSG_GATE_H */

