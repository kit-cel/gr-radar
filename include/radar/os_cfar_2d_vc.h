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


#ifndef INCLUDED_RADAR_OS_CFAR_2D_VC_H
#define INCLUDED_RADAR_OS_CFAR_2D_VC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block estimates peaks of a given matrix. A matrix can be represented as a combination of vectors and tagged streams. Input has to be a matrix with linear scaled values (NOT logarithmic scaled!). Used algorithm is a 2D OS-CFAR algorithm. The algorithm uses around the cell under test (CUT) on each side samp_compare samples to estimate the noise floor. samp_protect is a protected are around the CUT which is not used for acquiring compare samples. Index 0 of a input vector refers to x axis properties and index 1 refers to y axis properties. The relative threshold is defined by the bin of the vector within the sorted samp_compare samples. A standard value is rel_threshold = 0.78. The value of this bin is multiplied by mult_threshold and compared with the CUT. If the magnitude square of the CUT is greater than the threshold the matrix item is accepted. Used identifiers (symbols) for data are 'axis_x', 'axis_y' and 'power'.
     * 
     * \param vlen Input vector length
     * \param samp_compare Compare samples (vector index refers to axis)
     * \param samp_protect Protected samples (vector index refers to axis)
     * \param rel_threshold Relative threshold
     * \param mult_threshold Multiplier threshold
     * \param len_key Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API os_cfar_2d_vc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<os_cfar_2d_vc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::os_cfar_2d_vc.
       *
       * To avoid accidental use of raw pointers, radar::os_cfar_2d_vc's
       * constructor is in a private implementation
       * class. radar::os_cfar_2d_vc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int vlen, std::vector<int> samp_compare, std::vector<int> samp_protect, float rel_threshold, float mult_threshold, const std::string& len_key="packet_len");
      virtual void set_rel_threshold(float inp) = 0;
      virtual void set_mult_threshold(float inp) = 0;
      virtual void set_samp_compare(std::vector<int> inp) = 0;
      virtual void set_samp_protect(std::vector<int> inp) = 0;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OS_CFAR_2D_VC_H */

