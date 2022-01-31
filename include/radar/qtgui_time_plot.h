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


#ifndef INCLUDED_RADAR_QTGUI_TIME_PLOT_H
#define INCLUDED_RADAR_QTGUI_TIME_PLOT_H

#include <gnuradio/block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block displays a plot of a value with a given interval in milliseconds.
 * label_y represents the identifier (symbol) of the data. The y axis is scaled with
 * axis_y. range_time gives the shown range of the x axis in seconds. label is an
 * additional label for the title.
 *
 * \param interval Update interval in milliseconds
 * \param label_y Lable for y axis and identifier (symbol) for data
 * \param axis_y Display range y axis
 * \param range_time Display range for x axis in seconds
 * \param label Additional label for the title
 *
 * \ingroup radar
 *
 */
class RADAR_API qtgui_time_plot : virtual public gr::block
{
public:
    typedef std::shared_ptr<qtgui_time_plot> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::qtgui_time_plot.
     *
     * To avoid accidental use of raw pointers, radar::qtgui_time_plot's
     * constructor is in a private implementation
     * class. radar::qtgui_time_plot::make is the public interface for
     * creating new instances.
     */
    static sptr make(int interval,
                     std::string label_y,
                     std::vector<float> axis_y,
                     float range_time,
                     std::string label = "");
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTGUI_TIME_PLOT_H */
