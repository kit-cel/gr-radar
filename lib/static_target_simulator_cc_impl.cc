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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "static_target_simulator_cc_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <volk/volk.h>

namespace gr {
namespace radar {
namespace {
//! \f$ \sqrt{(4\pi)^3)}\f$
const double FOUR_PI_CUBED_SQRT = 44.54662397465366;
} // namespace

static_target_simulator_cc::sptr
static_target_simulator_cc::make(std::vector<float> range,
                                 std::vector<float> velocity,
                                 std::vector<float> rcs,
                                 std::vector<float> azimuth,
                                 std::vector<float> position_rx,
                                 int samp_rate,
                                 float center_freq,
                                 float self_coupling_db,
                                 bool rndm_phaseshift,
                                 bool self_coupling,
                                 const std::string& len_key)
{
    return gnuradio::get_initial_sptr(
        new static_target_simulator_cc_impl(range,
                                            velocity,
                                            rcs,
                                            azimuth,
                                            position_rx,
                                            samp_rate,
                                            center_freq,
                                            self_coupling_db,
                                            rndm_phaseshift,
                                            self_coupling,
                                            len_key));
}

static_target_simulator_cc_impl::static_target_simulator_cc_impl(
    std::vector<float> range,
    std::vector<float> velocity,
    std::vector<float> rcs,
    std::vector<float> azimuth,
    std::vector<float> position_rx,
    int samp_rate,
    float center_freq,
    float self_coupling_db,
    bool rndm_phaseshift,
    bool self_coupling,
    const std::string& len_key)
    : gr::tagged_stream_block("static_target_simulator_cc",
                              gr::io_signature::make(1, 1, sizeof(gr_complex)),
                              gr::io_signature::make(position_rx.size(),
                                                     position_rx.size(),
                                                     sizeof(gr_complex)),
                              len_key)
{
    setup_targets(range,
                  velocity,
                  rcs,
                  azimuth,
                  position_rx,
                  samp_rate,
                  center_freq,
                  self_coupling_db,
                  rndm_phaseshift,
                  self_coupling);
}

static_target_simulator_cc_impl::~static_target_simulator_cc_impl() {}

void static_target_simulator_cc_impl::setup_targets(std::vector<float> range,
                                                    std::vector<float> velocity,
                                                    std::vector<float> rcs,
                                                    std::vector<float> azimuth,
                                                    std::vector<float> position_rx,
                                                    int samp_rate,
                                                    float center_freq,
                                                    float self_coupling_db,
                                                    bool rndm_phaseshift,
                                                    bool self_coupling)
{
    d_range = range;
    d_velocity = velocity;
    d_rcs = rcs;
    d_azimuth = azimuth;
    d_position_rx = position_rx;
    d_center_freq =
        center_freq; // center frequency of simulated hardware for doppler estimation
    d_samp_rate = samp_rate;
    d_hold_noutput = -1;
    d_rndm_phaseshift = rndm_phaseshift;
    d_self_coupling = self_coupling;
    d_self_coupling_db = self_coupling_db;

    // Setup rx_time tag
    d_key = pmt::string_to_symbol("rx_time");
    d_srcid = pmt::string_to_symbol("stat_targ_sim");

    // Get num targets
    d_num_targets =
        range.size(); // FIXME: throw exceptions for len(range)!=len(velocity)!=...

    // Get doppler frequencies
    d_doppler.resize(d_num_targets);
    d_filt_doppler.resize(d_num_targets);
    for (int k = 0; k < d_num_targets; k++)
        d_doppler[k] = 2 * d_velocity[k] * d_center_freq / c_light;

    // Get timeshifts
    d_timeshift.resize(d_num_targets);
    d_timeshift_azimuth.resize(d_position_rx.size());
    d_filt_time.resize(d_num_targets);
    d_filt_time_azimuth.resize(d_position_rx.size());
    for (int l = 0; l < d_position_rx.size(); l++) {
        d_filt_time_azimuth[l].resize(d_num_targets);
        d_timeshift_azimuth[l].resize(d_num_targets);
        for (int k = 0; k < d_num_targets; k++) {
            d_timeshift_azimuth[l][k] =
                d_position_rx[l] * std::sin(d_azimuth[k] * GR_M_PI / 180.0);
        }
    }
    for (int k = 0; k < d_num_targets; k++)
        d_timeshift[k] = 2.0 * d_range[k] / c_light;

    // Get signal amplitude of reflection with free space path loss and rcs (radar
    // equation)
    d_scale_ampl.resize(d_num_targets);
    for (int k = 0; k < d_num_targets; k++) {
        // Factor out all terms out of the sqrt except the RCS:
        d_scale_ampl[k] = c_light * std::sqrt(d_rcs[k]) / FOUR_PI_CUBED_SQRT /
                          (d_range[k] * d_range[k]) / d_center_freq;
    }

    if (d_rndm_phaseshift) {
        // Resize phase shift filter
        d_filt_phase.resize(d_num_targets);

        // Setup random numbers
        std::srand(std::time(NULL)); // initial with time
    }
}

int static_target_simulator_cc_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    int noutput_items = ninput_items[0];
    return noutput_items;
}

int static_target_simulator_cc_impl::work(int noutput_items,
                                          gr_vector_int& ninput_items,
                                          gr_vector_const_void_star& input_items,
                                          gr_vector_void_star& output_items)
{
    const gr_complex* in = (const gr_complex*)input_items[0];
    gr_complex* out;

    // Set output items to tagged stream length
    noutput_items = ninput_items[0];

    // Check if new filter, buffer or fft plan is necessary
    if (d_hold_noutput != noutput_items) {
        // Set length buffer in loop
        d_hold_in.resize(noutput_items);

        // Setup fft and ifft
        d_in_fft.resize(noutput_items);
        d_fft_plan = fftwf_plan_dft_1d(noutput_items,
                                       reinterpret_cast<fftwf_complex*>(&d_hold_in[0]),
                                       reinterpret_cast<fftwf_complex*>(&d_in_fft[0]),
                                       FFTW_FORWARD,
                                       FFTW_ESTIMATE);
        d_ifft_plan = fftwf_plan_dft_1d(noutput_items,
                                        reinterpret_cast<fftwf_complex*>(&d_in_fft[0]),
                                        reinterpret_cast<fftwf_complex*>(&d_hold_in[0]),
                                        FFTW_BACKWARD,
                                        FFTW_ESTIMATE);

        // Setup frequency vector for shift in frequency domain
        d_freq.resize(noutput_items);
        for (int i = 0; i < noutput_items; i++) {
            if (i < noutput_items / 2)
                d_freq[i] =
                    i * (float)d_samp_rate / (float)noutput_items; // zero to samp_rate/2
            else
                d_freq[i] = i * (float)d_samp_rate / (float)noutput_items -
                            (float)d_samp_rate; // -samp_rate/2 to zero
        }

        // Setup freq and time shift filter, resize phase shift filter
        for (int k = 0; k < d_num_targets; k++) {
            d_filt_doppler[k].resize(noutput_items);
            if (d_rndm_phaseshift)
                d_filt_phase[k].resize(noutput_items);

            d_phase_doppler = 0;
            for (int i = 0; i < noutput_items; i++) {
                // Doppler shift filter and rescaling amplitude with rcs
                d_filt_doppler[k][i] = std::exp(d_phase_doppler) * d_scale_ampl[k];
                d_phase_doppler =
                    gr_complex(0, std::fmod(std::imag(d_phase_doppler) +
                                       2 * GR_M_PI * d_doppler[k] / (float)d_samp_rate,
                                   2 * GR_M_PI)); // integrate phase (with plus!)
            }

            d_filt_time[k].resize(noutput_items);
            d_phase_time = 0;
            for (int i = 0; i < noutput_items; i++) {
                // Time shift filter, uses target range
                d_phase_time =
                    gr_complex(0, std::fmod(2 * GR_M_PI * (d_timeshift[k]) // range time shift
                                       * d_freq[i],
                                   2 * GR_M_PI)); // integrate phase (with minus!)
                d_filt_time[k][i] =
                    std::exp(-d_phase_time) /
                    (float)noutput_items; // div with noutput_item to correct amplitude
                                          // after fft->ifft
            }

            for (int l = 0; l < d_position_rx.size();
                 l++) { // Do time shift filter with azimuth and position, there are two
                        // time shift filters to avoid problems with significant digits of
                        // float
                d_filt_time_azimuth[l][k].resize(noutput_items);
                d_phase_time = 0;
                for (int i = 0; i < noutput_items; i++) {
                    // Time shift filter, uses azimuth and RX position
                    d_phase_time =
                        gr_complex(0, std::fmod(2 * GR_M_PI *
                                      (d_timeshift_azimuth[l][k]) // azimuth time shift
                                      * d_freq[i],
                                  2 * GR_M_PI)); // integrate phase (with minus!)
                    d_filt_time_azimuth[l][k][i] =
                        std::exp(-d_phase_time); // do not div with noutput_items, is done
                                                 // with range timeshift filter
                }
            }
        }

        // Resize hold of noutput_items
        d_hold_noutput = noutput_items;
    }

    // Setup random phase shift
    if (d_rndm_phaseshift) {
        gr_complex phase_random_hold;
        for (int k = 0; k < d_num_targets; k++) {
            phase_random_hold = gr_complex(0, 2 * GR_M_PI * float((std::rand() % 1000 + 1) / 1000.0));
            d_phase_random = std::exp(phase_random_hold);
            std::fill_n(&d_filt_phase[k][0], noutput_items, d_phase_random);
        }
    }

    // Go through RXs
    for (int l = 0; l < d_position_rx.size(); l++) {
        // Setup pointer on output buffer
        out = (gr_complex*)output_items[l];

        // Set rx_time tag
        d_time_sec = nitems_written(l) / d_samp_rate;
        d_time_frac_sec = nitems_written(l) / (float)d_samp_rate - d_time_sec;
        d_val = pmt::make_tuple(
            pmt::from_uint64(d_time_sec),
            pmt::from_double(d_time_frac_sec)); // FIXME: correct implementation?
        add_item_tag(l, nitems_written(l), d_key, d_val, d_srcid);

        // Set output to zero
        std::memset(out, 0, noutput_items * sizeof(gr_complex));

        // Go through targets and apply filters
        for (int k = 0; k < d_num_targets; k++) {
            // Add doppler shift
            volk_32fc_x2_multiply_32fc(
                &d_hold_in[0],
                in,
                &d_filt_doppler[k][0],
                noutput_items); // add doppler shift with rescaled amplitude

            // Add time shift
            fftwf_execute(d_fft_plan); // go to freq domain
            volk_32fc_x2_multiply_32fc(&d_in_fft[0],
                                       &d_in_fft[0],
                                       &d_filt_time[k][0],
                                       noutput_items); // add timeshift with multiply
                                                       // exp-func in freq domain (range)
            volk_32fc_x2_multiply_32fc(
                &d_in_fft[0],
                &d_in_fft[0],
                &d_filt_time_azimuth[l][k][0],
                noutput_items); // add timeshift with multiply exp-func in freq domain (rx
                                // position with azimuth and range)
            fftwf_execute(d_ifft_plan); // back in time domain

            if (d_rndm_phaseshift) {
                // Add random phase shift
                volk_32fc_x2_multiply_32fc(&d_hold_in[0],
                                           &d_hold_in[0],
                                           &d_filt_phase[k][0],
                                           noutput_items); // add random phase shift
            }

            // Add to output
            for (int i = 0; i < noutput_items; i++)
                out[i] += d_hold_in[i];
        }

        // Add self coupling
        if (d_self_coupling) {
            for (int i = 0; i < noutput_items; i++)
                out[i] += (gr_complex)pow(10, d_self_coupling_db / 20.0) *
                          in[i]; // d_self_coupling_db gives scaling of power
        }
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
