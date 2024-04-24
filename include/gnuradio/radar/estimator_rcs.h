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


#ifndef INCLUDED_RADAR_ESTIMATOR_RCS_H
#define INCLUDED_RADAR_ESTIMATOR_RCS_H

#include <gnuradio/block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief Basic RCS block for estimating the RCS of a single target. Some hardware specs
 need to be known for calculating RCS values, see parameters for details. The radar
 equation is used to calculate the RCS: RCS = Pr*(4pi)^3*R^4/(Pt*Gt*Gr*lamda^2). The Rx
 power (Pr) and the distance (R) are being estimated, while the other parameters are given
 in the flowgraph. It is possible to average a number of samples by setting the num_mean
 value > 1. The RCS will be 0 until enough samples are collected to calculate the mean
 value (be patient). The Tx power (Pt) needs to be calibrated with external hardware. I
 recommend to calibrate for the wanted power and not to change the parameters in the
 flowgraph on the Tx side after that. The RCS block needs the Rx power, to estimate the
 RCS. For that, the input power of the block needs to be determined analytically and
 altered via the corr_factor and exponent values, to fit the following equation: Pr =
 P_input ^ (exponent) * corr_factor / Pt. In addition, the FFTs need to be normalized for
 correct power calculation.
 *
 * \param num_mean Number of samples taken into account for calculating mean value (1 for
 no mean calculation)
 * \param center_freq Center frequency of radar
 * \param antenna_gain_tx Antenna Gain of the Tx antenna
 * \param antenna_gain_rx Antenna Gain of the Rx antenna
 * \param usrp_gain_rx Rx gain of USRP set in flowgraph
 * \param power_tx Tx power of radar signal. Needs to be measured one time!
 * \param corr_factor Correction factor for the RCS to calibrate system to a known target
 or special signal paths
 * \param exponent Exponent of the input power to calculate Rx power, depends on signal
 path (calculate analytically)
 *
 * \ingroup radar
 *
 */
class RADAR_API estimator_rcs : virtual public gr::block
{
public:
    typedef std::shared_ptr<estimator_rcs> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::estimator_rcs.
     *
     * To avoid accidental use of raw pointers, radar::estimator_rcs's
     * constructor is in a private implementation
     * class. radar::estimator_rcs::make is the public interface for
     * creating new instances.
     */
    static sptr make(int num_mean,
                     float center_freq,
                     float antenna_gain_tx,
                     float antenna_gain_rx,
                     float usrp_gain_rx,
                     float power_tx,
                     float corr_factor,
                     float exponent = 1);
    // callbacks
    virtual void set_num_mean(int val) = 0;
    virtual void set_center_freq(float val) = 0;
    virtual void set_antenna_gain_tx(float val) = 0;
    virtual void set_antenna_gain_rx(float val) = 0;
    virtual void set_usrp_gain_rx(float val) = 0;
    virtual void set_power_tx(float val) = 0;
    virtual void set_corr_factor(float val) = 0;
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_RCS_H */
