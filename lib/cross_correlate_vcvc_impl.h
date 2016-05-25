/* -*- c++ -*- */
/*
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_RADAR_CROSS_CORRELATE_VCVC_IMPL_H
#define INCLUDED_RADAR_CROSS_CORRELATE_VCVC_IMPL_H

#include <radar/cross_correlate_vcvc.h>
#include <complex>
#include <fftw3.h>

namespace gr {
  namespace radar {

    class cross_correlate_vcvc_impl : public cross_correlate_vcvc
    {
     private:
      int d_vlen;
      fftwf_plan d_fft_plan1;
      fftwf_plan d_fft_plan2;
      fftwf_plan d_ifft_plan;
      gr_complex* d_fft_buffer1;
      gr_complex* d_fft_buffer2;
      gr_complex* d_out_buffer;

     public:
      cross_correlate_vcvc_impl(int vlen);
      ~cross_correlate_vcvc_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_CROSS_CORRELATE_VCVC_IMPL_H */
