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

#ifndef INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_H
#define INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_H

#include <gnuradio/tagged_stream_block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*! Simulates the backscattering of a given signal on point targets.
 *
 * Implements the point scatter model. The input signal is the transmitted
 * signal. The output signal is the received, backscattered signal, and
 * contains one output signal per receive antenna.
 *
 * \section radar_target_modeling Target modeling
 *
 * The targets are modeled by the vectors \p range, \p velocity, \p rcs,
 * and \p azimuth. All these vectors need to be of length H, where H
 * describes the number of reflecting targets. The backscattered signal
 * can either have a zero phase, or a random phase (controlled by
 * \p rndm_phaseshift).  The received signal will be the linear
 * superposition of H signals, each of which are derived from the input
 * signal by the following equation:
 *
 * \f[
 *     r_h(t) = b_h s(t - \tau_h) e^{j2\pi f_{D,h}}
 * \f]
 *
 * The attenuation depends on the center frequency \f$f_C\f$, the radar
 * cross section \f$\sigma_{\text{RCS},h}\f$, the distance of the target
 * \f$d_h\f$ and the speed of light \f$c_0\f$:
 * \f[
 *     b_h = \sqrt{\frac{c_0^2 \sigma_{\text{RCS},h}}{(4\pi)^3 d_h^4 f_C^2}}
 * \f]
 *
 * The delay \f$\tau_h\f$ depends on the distance of the target:
 * \f[
 *     \tau_h = 2\frac{d_h}{c_0}
 * \f]
 *
 * The Doppler shift \f$f_{D,h}\f$ depends on the relative velocity and
 * the center frequency:
 * \f[
 *     f_{D,h} = 2\frac{v_{\text{rel},h}{c_0} f_C
 * \f]
 *
 * The signals are added up to produce the total sum signal:
 * \f[
 *     r(t) = \sum_{h=0}^{H-1} r_h(t)
 * \f]
 *
 * \section radar_mimo_processing MIMO processing simulation
 *
 * This block has a limited capability of simulating multi-antenna
 * reception. The \p position_rx vector determines the distance of every
 * RX antenna from the origin (Note: The TX antenna is always in the
 * origin). The length of the \p position_rx vector is thus also the number
 * of output signals this block produces. For a simple, mono-static SISO
 * radar, simply set position_rx to [0].
 * The RX antennas are always laid out in a straight line. When multiple
 * antennas are given, the target azimuth plays a role; an azimuth of zero
 * is perpendicular to the line in which the RX antennas are placed.
 *
 * \section radar_hw_impairments Self-coupling
 *
 * Self-coupling describes the amount of the TX signal that is directly
 * coupled into the RX path. When \p self_coupling is set to true,
 * effectively, a target with zero velocity and range is added. The
 * parameter \p self_coupling_db describes the attenuation of the
 * self-coupling, a value of -10 means that the transmit signal is
 * attenuated by 10 dB on the receive signal.
 *
 * \ingroup radar
 */
class RADAR_API static_target_simulator_cc : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<static_target_simulator_cc> sptr;

    /*!
     * \param range Target ranges as vector (length H)
     * \param velocity Target velocities as vector (length H)
     * \param rcs Target RCS as vector (length H)
     * \param azimuth Target azimuth as vector (length H)
     * \param position_rx Position RX antennas. A value of [0] means
     *                    there is one antenna, located in the origin
     *                    (simple monostatic case).
     * \param samp_rate Sample rate (samples per second)
     * \param center_freq Center frequency (Hz)
     * \param self_coupling_db Self coupling attenuation (dB)
     * \param rndm_phaseshift Toggle random phaseshift on targets
     * \param self_coupling Toggle self coupling
     * \param packet_len Packet length key for tagged stream
     */
    static sptr make(std::vector<float> range,
                     std::vector<float> velocity,
                     std::vector<float> rcs,
                     std::vector<float> azimuth,
                     std::vector<float> position_rx,
                     int samp_rate,
                     float center_freq,
                     float self_coupling_db,
                     bool rndm_phaseshift = true,
                     bool self_coupling = true,
                     const std::string& len_key = "packet_len");

    virtual void setup_targets(std::vector<float> range,
                               std::vector<float> velocity,
                               std::vector<float> rcs,
                               std::vector<float> azimuth,
                               std::vector<float> position_rx,
                               int samp_rate,
                               float center_freq,
                               float self_coupling_db,
                               bool rndm_phaseshift,
                               bool self_coupling) = 0;
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_H */
