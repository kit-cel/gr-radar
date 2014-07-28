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

#ifndef INCLUDED_RADAR_OS_CFAR_2D_VC_IMPL_H
#define INCLUDED_RADAR_OS_CFAR_2D_VC_IMPL_H

#include <radar/os_cfar_2d_vc.h>

namespace gr {
  namespace radar {

    class os_cfar_2d_vc_impl : public os_cfar_2d_vc
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      os_cfar_2d_vc_impl(int vlen, std::vector<int> samp_compare, std::vector<int> samp_protect, float rel_threshold, float mult_threshold, const std::string& len_key);
      ~os_cfar_2d_vc_impl();
      void set_rel_threshold(float inp);
      void set_mult_threshold(float inp);
      void set_samp_compare(std::vector<int> inp);
      void set_samp_protect(std::vector<int> inp);
      
      int d_vlen;
      std::vector<int> d_samp_compare, d_samp_protect;
      float d_rel_threshold, d_mult_threshold;
      std::vector<float> d_x, d_y, d_power;
      
      pmt::pmt_t d_port_id;
      std::vector< tag_t > d_tags;
      pmt::pmt_t d_ptimestamp, d_px, d_py, d_ppower, d_value;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OS_CFAR_2D_VC_IMPL_H */

