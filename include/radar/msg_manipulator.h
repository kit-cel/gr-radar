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


#ifndef INCLUDED_RADAR_MSG_MANIPULATOR_H
#define INCLUDED_RADAR_MSG_MANIPULATOR_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block manipulates data in a msg with given identifier (symbol). All data are given as vectors and each index represents a dataset with identifier which should be processed. All other data is pushed through. The addition is performed before the multiplication.
     * 
     * \param symbols Identifier (symbols) of data as vector
     * \param const_add Value to add on the data
     * \param const_mult Value to multiply on the data
     * 
     * \ingroup radar
     *
     */
    class RADAR_API msg_manipulator : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<msg_manipulator> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::msg_manipulator.
       *
       * To avoid accidental use of raw pointers, radar::msg_manipulator's
       * constructor is in a private implementation
       * class. radar::msg_manipulator::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::vector<std::string> symbols, std::vector<float> const_add, std::vector<float> const_mult);
      virtual void set_const_add(std::vector<float> val) = 0;
      virtual void set_const_mult(std::vector<float> val) = 0;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_MSG_MANIPULATOR_H */

