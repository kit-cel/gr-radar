//* -*- c++ -*- */
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

#ifndef INCLUDED_RADAR_OS_CFAR_C_IMPL_H
#define INCLUDED_RADAR_OS_CFAR_C_IMPL_H

#include <radar/os_cfar_c.h>

namespace gr {
namespace radar {

class os_cfar_c_impl : public os_cfar_c
{
private:
    // Nothing to declare in this block.

protected:
    int calculate_output_stream_length(const gr_vector_int& ninput_items);

public:
    os_cfar_c_impl(int samp_rate,
                   int samp_compare,
                   int samp_protect,
                   float rel_threshold,
                   float mult_threshold,
                   bool merge_consecutive,
                   const std::string& len_key);
    ~os_cfar_c_impl();
    void set_rel_threshold(float inp);
    void set_mult_threshold(float inp);
    void set_samp_compare(int inp);
    void set_samp_protect(int inp);

    int d_samp_compare, d_samp_protect, d_samp_rate;
    float d_rel_threshold, d_mult_threshold;
    bool d_merge_consecutive, d_consecutive;

    std::vector<float> d_pks, d_freq, d_angle, d_hold_samp;

    pmt::pmt_t d_port_id;
    pmt::pmt_t d_ptimestamp, d_pfreq, d_ppks, d_pangle, d_value;
    std::vector<tag_t> d_tags;

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_int& ninput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_OS_CFAR_C_IMPL_H */
