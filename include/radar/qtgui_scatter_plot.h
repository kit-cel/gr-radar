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


#ifndef INCLUDED_RADAR_QTGUI_SCATTER_PLOT_H
#define INCLUDED_RADAR_QTGUI_SCATTER_PLOT_H

#include <gnuradio/block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block displays a scatter plot of two data sets. The identifiers (symbols)
 * are given with label_x and label_y. The display range is given with axis_x and axis_y.
 * Points that are not in range are simply not displayed (there are no errors or
 * warnings). label gives an additional lable to differ multiple scatter plots on the
 * screen by the titles. The update interval is given in milliseconds.
 *
 * \param interval Update interval in milliseconds
 * \param label_x Identifier (symbol) of first data set
 * \param label_y Identifier (symbol) of second data set
 * \param axis_x Display range on x axis
 * \param axis_y Display range on y axis
 * \param label Optional label for the title
 *
 * \ingroup radar
 *
 */
class RADAR_API qtgui_scatter_plot : virtual public gr::block
{
public:
    typedef boost::shared_ptr<qtgui_scatter_plot> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::qtgui_scatter_plot.
     *
     * To avoid accidental use of raw pointers, radar::qtgui_scatter_plot's
     * constructor is in a private implementation
     * class. radar::qtgui_scatter_plot::make is the public interface for
     * creating new instances.
     */
    static sptr make(int interval,
                     std::string label_x,
                     std::string label_y,
                     std::vector<float> axis_x,
                     std::vector<float> axis_y,
                     std::string label = "");
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTGUI_SCATTER_PLOT_H */
