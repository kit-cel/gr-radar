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

#ifndef INCLUDED_RADAR_OFDM_DIVIDE_VCVC_IMPL_H
#define INCLUDED_RADAR_OFDM_DIVIDE_VCVC_IMPL_H

#include <radar/ofdm_divide_vcvc.h>

namespace gr {
  namespace radar {

    class ofdm_divide_vcvc_impl : public ofdm_divide_vcvc
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      ofdm_divide_vcvc_impl(int vlen_in, int vlen_out, std::vector<int> discarded_carriers, std::string len_key);
      ~ofdm_divide_vcvc_impl();
      
      int d_vlen_in, d_vlen_out;
      std::vector<int> d_discarded_carriers;
      std::string d_len_key;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OFDM_DIVIDE_VCVC_IMPL_H */

