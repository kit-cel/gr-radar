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

#ifndef INCLUDED_RADAR_TS_FFT_CC_IMPL_H
#define INCLUDED_RADAR_TS_FFT_CC_IMPL_H

#include <fftw3.h>
#include <radar/ts_fft_cc.h>

namespace gr {
namespace radar {

class ts_fft_cc_impl : public ts_fft_cc
{
private:
    // Nothing to declare in this block.

protected:
    int calculate_output_stream_length(const gr_vector_int& ninput_items);

public:
    ts_fft_cc_impl(int packet_len, const std::string& len_key);
    ~ts_fft_cc_impl();

    fftwf_plan d_fft_plan;
    fftwf_complex* d_buffer;
    int d_packet_len;

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_int& ninput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TS_FFT_CC_IMPL_H */
