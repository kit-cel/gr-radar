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


#ifndef INCLUDED_RADAR_QTGUI_SPECTROGRAM_PLOT_H
#define INCLUDED_RADAR_QTGUI_SPECTROGRAM_PLOT_H

#include <gnuradio/tagged_stream_block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block displays a spectrogram plot of a tagged stream with vectors. Tagged
 * streams combined with vectors represent a matrix. The value of the matrix elements are
 * displayed within a color plot. The colorbar can be scaled automatically or with a
 * manual scale with axis_z. The axis of x and y are set with axis_x and axis_y. The
 * update rate interval is given in milliseconds.
 *
 * \param vlen Vector length
 * \param xlabel Label for x axis
 * \param ylabel Label for y axis
 * \param label Addition label for title
 * \param axis_x Display range for x axis
 * \param axis_y Display range for y axis
 * \param axis_z Display range for z axis. This defines the colorbar if autoscale_z is
 * disabled. \param autoscale_z Toggle automatic scale of the colorbar \param packet_len
 * Packet length key for tagged stream
 *
 * \ingroup radar
 *
 */
class RADAR_API qtgui_spectrogram_plot : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<qtgui_spectrogram_plot> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::qtgui_spectrogram_plot.
     *
     * To avoid accidental use of raw pointers, radar::qtgui_spectrogram_plot's
     * constructor is in a private implementation
     * class. radar::qtgui_spectrogram_plot::make is the public interface for
     * creating new instances.
     */
    static sptr make(int vlen,
                     int interval,
                     std::string xlabel,
                     std::string ylabel,
                     std::string label,
                     std::vector<float> axis_x,
                     std::vector<float> axis_y,
                     std::vector<float> axis_z,
                     bool autoscale_z,
                     std::string len_key = "packet_len");
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTGUI_SPECTROGRAM_PLOT_H */
