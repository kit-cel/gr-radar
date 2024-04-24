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


#ifndef INCLUDED_RADAR_ESTIMATOR_OFDM_H
#define INCLUDED_RADAR_ESTIMATOR_OFDM_H

#include <gnuradio/block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block evaluates the peaks given from a OFDM matrix. Input are the bins of
 * the peaks with the identifiers 'axis_x' and 'axis_y'. The parameters of the block
 * axis_x and axis_y are vectors which define the mapping of the axis. If two values are
 * given it is assumed a linear progression in between. If four values are given the
 * middle values are set on half of the axis and it is interpolated linear in between.
 * len_x and len_y gives the length of the axis in number of bins. symbol_x and symbol_y
 * defines the identifier (symbols) for the output data. merge_consecutive toggles merging
 * consecutive peaks. Each peak is compared with peaks in a range of one bin. If there is
 * a peak with a higher power the actual bin is not used for evaluations. If
 * merge_consecutive is true data with identifier 'power' is needed.
 *
 * \param symbol_x Identifier (symbol) for information on x axis
 * \param len_x Length of x axis in bins
 * \param axis_x Axis x
 * \param symbol_y Identifier (symbol) for information on y axis
 * \param len_y Length of y axis in bins
 * \param axis_y Axis y
 * \param merge_consecutive Merge consecutive peaks
 *
 * \ingroup radar
 *
 */
class RADAR_API estimator_ofdm : virtual public gr::block
{
public:
    typedef std::shared_ptr<estimator_ofdm> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::estimator_ofdm.
     *
     * To avoid accidental use of raw pointers, radar::estimator_ofdm's
     * constructor is in a private implementation
     * class. radar::estimator_ofdm::make is the public interface for
     * creating new instances.
     */
    static sptr make(std::string symbol_x,
                     int len_x,
                     std::vector<float> axis_x,
                     std::string symbol_y,
                     int len_y,
                     std::vector<float> axis_y,
                     bool merge_consecutive = true);
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_OFDM_H */
