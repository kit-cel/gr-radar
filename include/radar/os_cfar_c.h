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
 
#ifndef INCLUDED_RADAR_OS_CFAR_C_H
#define INCLUDED_RADAR_OS_CFAR_C_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block estimates peaks of a given FFT spectrum as tagged stream. Multi peak detection is implemented with the OS-CFAR algorithm. The algorithm uses around the cell under test (CUT) on each side samp_compare samples to estimate the noise floor. This relative threshold is defined by the bin of the vector within the sorted samp_compare samples. A standard value is rel_threshold = 0.78. The value of this bin is multiplied by mult_threshold and compared with the CUT. samp_protect samples are a protected are which is not used for acquiring compare samples. If consecutive bins are detected as valid peaks it is possible to merge these detections with merge_consecutive = true. Output data are f32vectors with the information of frequency, power and phase. The identifiers (symbols) are 'frequency', 'power' and 'phase'.
     * 
     * \param samp_rate Sample rate
     * \param samp_compare Sample to be compared with each other
     * \param samp_protect Samples which are protected and not used for peak detection
     * \param rel_threshold Relative threshold
     * \param mult_threshold Multiplier threshold
     * \param merge_consectuive Toggle merging consecutive detected peaks
     * \param len_key Packet length key for tagged stream
     * 
     * \ingroup radar
     *
     */
    class RADAR_API os_cfar_c : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<os_cfar_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::os_cfar_c.
       *
       * To avoid accidental use of raw pointers, radar::os_cfar_c's
       * constructor is in a private implementation
       * class. radar::os_cfar_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_rate, int samp_compare, int samp_protect, float rel_threshold, float mult_threshold, bool merge_consecutive=true, const std::string& len_key="packet_len");
      virtual void set_rel_threshold(float inp) = 0;
      virtual void set_mult_threshold(float inp) = 0;
      virtual void set_samp_compare(int inp) = 0;
      virtual void set_samp_protect(int inp) = 0;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OS_CFAR_C_H */

