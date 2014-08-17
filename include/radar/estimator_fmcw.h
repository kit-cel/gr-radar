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
 
#ifndef INCLUDED_RADAR_ESTIMATOR_FMCW_H
#define INCLUDED_RADAR_ESTIMATOR_FMCW_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block estimates range and veloicty from peaks of a FMCW spectrum. Input messages are data with the identifier 'frequency' of the up-chirp, down-chirp and CW part. If data is available on all three message ports the estimation starts. The velocity is estimated with the frequency information of the CW block and the range is estimated with the up and down chirp. If multiple frequencies are given, the velocity is estimated first and associated with the most likely range from the up- and down-chirp. The output identifiers are 'range' and 'velocity'.
     * 
     * \param samp_rate Sample rate
     * \param center_freq Center frequency
     * \param sweep_freq Sweep frequency
     * \param samp_up Samples of up-chirp
     * \param samp_down Samples of down-chirp
     * \param push_power Push through power of peak
     * 
     * \ingroup radar
     *
     */
    class RADAR_API estimator_fmcw : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<estimator_fmcw> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::estimator_fmcw.
       *
       * To avoid accidental use of raw pointers, radar::estimator_fmcw's
       * constructor is in a private implementation
       * class. radar::estimator_fmcw::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_rate, float center_freq, float sweep_freq, int samp_up, int samp_down, bool push_power);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_FMCW_H */

