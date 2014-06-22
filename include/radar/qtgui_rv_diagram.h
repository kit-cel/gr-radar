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


#ifndef INCLUDED_RADAR_QTGUI_RV_DIAGRAM_H
#define INCLUDED_RADAR_QTGUI_RV_DIAGRAM_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief <+description of block+>
     * \ingroup radar
     *
     */
    class RADAR_API qtgui_rv_diagram : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<qtgui_rv_diagram> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::qtgui_rv_diagram.
       *
       * To avoid accidental use of raw pointers, radar::qtgui_rv_diagram's
       * constructor is in a private implementation
       * class. radar::qtgui_rv_diagram::make is the public interface for
       * creating new instances.
       */
      static sptr make(int interval, std::vector<float> axis_range, std::vector<float> axis_velocity);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_QTGUI_RV_DIAGRAM_H */

