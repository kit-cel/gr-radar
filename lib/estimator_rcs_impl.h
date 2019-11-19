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

#ifndef INCLUDED_RADAR_ESTIMATOR_RCS_IMPL_H
#define INCLUDED_RADAR_ESTIMATOR_RCS_IMPL_H

#include <radar/estimator_rcs.h>

namespace gr {
namespace radar {

class estimator_rcs_impl : public estimator_rcs
{
private:
    // Nothing to declare in this block

public:
    estimator_rcs_impl(int num_mean,
                       float center_freq,
                       float antenna_gain_tx,
                       float antenna_gain_rx,
                       float usrp_gain_rx,
                       float power_tx,
                       float corr_factor,
                       float exponent);
    ~estimator_rcs_impl();
    void handle_msg(pmt::pmt_t msg);
    float calculate_rcs();
    float calculate_vector_mean(boost::circular_buffer<float>*);
    void set_num_mean(int val);
    void set_center_freq(float val);
    void set_antenna_gain_tx(float val);
    void set_antenna_gain_rx(float val);
    void set_usrp_gain_rx(float val);
    void set_power_tx(float val);
    void set_corr_factor(float val);


    boost::circular_buffer<float> d_rcs_vals;
    int d_num_mean, d_loop_counter;

    float d_center_freq, d_antenna_gain_tx, d_antenna_gain_rx, d_usrp_gain_rx, d_power_tx,
        d_fak, d_lambda, d_antenna_gain_abs_rx, d_antenna_gain_abs_tx, d_corr_factor,
        d_exponent;

    pmt::pmt_t d_port_id_in, d_port_id_out;
    pmt::pmt_t d_prange, d_ppower, d_value;
    pmt::pmt_t d_rcs_value, d_rcs_key, d_rcs_pack;
    std::vector<float> d_range, d_power, d_rcs;
    std::vector<pmt::pmt_t> d_msg_hold;

    constexpr static float c_light = 3e8;
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_RCS_IMPL_H */
