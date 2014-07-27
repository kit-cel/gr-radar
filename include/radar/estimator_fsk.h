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

#ifndef INCLUDED_RADAR_ESTIMATOR_FSK_H
#define INCLUDED_RADAR_ESTIMATOR_FSK_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief This block estimates the range with peaks given from a FSK spectrum. Needed identifiers (symbols) are 'frequency' and 'phase'. The velocity is calculated with the 'frequency' information and the doppler formula. The phase of the doppler peaks are used to estimate the range. Output identifier are 'range' and 'velocity'. If push_power is true the information about the power of the peaks is pushed through (if available). This can be used for estimating the RCS of an object.
     * 
     * \param center_freq Center frequency
     * \param delta_freq Frequency difference of high and low frequency
     * \param push_power Toggle pushing through information about power of peaks
     * 
     * \ingroup radar
     *
     */
    class RADAR_API estimator_fsk : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<estimator_fsk> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::estimator_fsk.
       *
       * To avoid accidental use of raw pointers, radar::estimator_fsk's
       * constructor is in a private implementation
       * class. radar::estimator_fsk::make is the public interface for
       * creating new instances.
       */
      static sptr make(float center_freq, float delta_freq, bool push_power=false);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_FSK_H */

