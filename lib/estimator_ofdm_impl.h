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

#ifndef INCLUDED_RADAR_ESTIMATOR_OFDM_IMPL_H
#define INCLUDED_RADAR_ESTIMATOR_OFDM_IMPL_H

#include <radar/estimator_ofdm.h>

namespace gr {
namespace radar {

class estimator_ofdm_impl : public estimator_ofdm
{
private:
    // Nothing to declare in this block.

public:
    estimator_ofdm_impl(std::string symbol_x,
                        int len_x,
                        std::vector<float> axis_x,
                        std::string symbol_y,
                        int len_y,
                        std::vector<float> axis_y,
                        bool merge_consecutive);
    ~estimator_ofdm_impl();
    void handle_msg(pmt::pmt_t msg);

    int d_len_x, d_len_y;
    std::string d_symbol_x, d_symbol_y;
    std::vector<float> d_axis_x, d_axis_y;
    bool d_merge_consecutive;

    pmt::pmt_t d_port_id_in, d_port_id_out;
    pmt::pmt_t d_paxis_x, d_paxis_y, d_ppower;
    std::vector<float> d_val_power, d_val_x, d_val_y, d_map_x, d_map_y;
    pmt::pmt_t d_x_key, d_x_value, d_x_pack, d_y_key, d_y_value, d_y_pack, d_value,
        d_ptimestamp;
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_OFDM_IMPL_H */
