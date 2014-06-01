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
 
#ifndef INCLUDED_RADAR_PRINT_PEAKS_IMPL_H
#define INCLUDED_RADAR_PRINT_PEAKS_IMPL_H

#include <radar/print_peaks.h>

namespace gr {
  namespace radar {

    class print_peaks_impl : public print_peaks
    {
     private:
      // Nothing to declare in this block.

     public:
      print_peaks_impl();
      ~print_peaks_impl();
      void handle_msg(pmt::pmt_t msg);
      
      pmt::pmt_t d_port_id_in;
      pmt::pmt_t d_timestamp, d_freq, d_pks, d_angle;
      size_t d_len_vec;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_PRINT_PEAKS_IMPL_H */

