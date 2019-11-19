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

#ifndef INCLUDED_RADAR_FIND_MAX_PEAK_C_IMPL_H
#define INCLUDED_RADAR_FIND_MAX_PEAK_C_IMPL_H

#include <radar/find_max_peak_c.h>

namespace gr {
namespace radar {

class find_max_peak_c_impl : public find_max_peak_c
{
private:
    // Nothing to declare in this block.

protected:
    int calculate_output_stream_length(const gr_vector_int& ninput_items);

public:
    find_max_peak_c_impl(int samp_rate,
                         float threshold,
                         int samp_protect,
                         std::vector<float> max_freq,
                         bool cut_max_freq,
                         const std::string& len_key);
    ~find_max_peak_c_impl();
    void set_threshold(float threshold);
    void set_samp_protect(int samp);
    void set_max_freq(std::vector<float> freq);

    int d_samp_rate;
    float d_threshold;
    int d_samp_protect;
    std::vector<float> d_max_freq;
    bool d_cut_max_freq;

    std::vector<float> d_pks, d_freq, d_angle;

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

#endif /* INCLUDED_RADAR_FIND_MAX_PEAK_C_IMPL_H */
