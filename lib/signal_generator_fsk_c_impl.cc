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

#include "signal_generator_fsk_c_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

signal_generator_fsk_c::sptr signal_generator_fsk_c::make(int samp_rate,
                                                          int samp_per_freq,
                                                          int blocks_per_tag,
                                                          float freq_low,
                                                          float freq_high,
                                                          float amplitude,
                                                          const std::string& len_key)
{
    return gnuradio::get_initial_sptr(new signal_generator_fsk_c_impl(samp_rate,
                                                                      samp_per_freq,
                                                                      blocks_per_tag,
                                                                      freq_low,
                                                                      freq_high,
                                                                      amplitude,
                                                                      len_key));
}

/*
 * The private constructor
 */
signal_generator_fsk_c_impl::signal_generator_fsk_c_impl(int samp_rate,
                                                         int samp_per_freq,
                                                         int blocks_per_tag,
                                                         float freq_low,
                                                         float freq_high,
                                                         float amplitude,
                                                         const std::string& len_key)
    : gr::sync_block("signal_generator_fsk_c",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(gr_complex)))
{
    d_samp_rate = samp_rate;
    d_samp_per_freq = samp_per_freq;   // samples on one frequency
    d_blocks_per_tag = blocks_per_tag; // block = 2*samp_per_freq
    d_freq_low = freq_low;
    d_freq_high = freq_high;
    d_amplitude = amplitude;

    d_packet_len = d_samp_per_freq * 2 * d_blocks_per_tag;
    d_key = pmt::string_to_symbol(len_key); // set tag identifier for tagged stream
    d_value = pmt::from_long(d_packet_len); // set length of 1 fsk packet as tagged stream
    d_srcid = pmt::string_to_symbol("sig_gen_fsk"); // set block identifier

    d_phase_high = 0;
    d_phase_low = 0;

    d_state = 1; // 0: low_freq, 1: high_freq
}

/*
 * Our virtual destructor.
 */
signal_generator_fsk_c_impl::~signal_generator_fsk_c_impl() {}

int signal_generator_fsk_c_impl::work(int noutput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items)
{
    gr_complex* out = (gr_complex*)output_items[0];

    // Integrate phase for iq signal
    for (int i = 0; i < noutput_items; i++) {
        // Set tag on every packet_len-th item
        if ((nitems_written(0) + i) % d_packet_len == 0)
            add_item_tag(0, nitems_written(0) + i, d_key, d_value, d_srcid);

        // Swap states every samp_per_freq item
        if ((nitems_written(0) + i) % d_samp_per_freq == 0) {
            if (d_state)
                d_state = 0;
            else
                d_state = 1;
        }

        // Write sample
        if (d_state)
            out[i] = d_amplitude * exp(d_phase_high);
        else
            out[i] = d_amplitude * exp(d_phase_low); // start with low_freq (state=0)

        d_phase_low =
            1j * std::fmod(imag(d_phase_low) + 2 * M_PI * d_freq_low / (float)d_samp_rate,
                           2 * M_PI);
        d_phase_high = 1j * std::fmod(imag(d_phase_high) +
                                          2 * M_PI * d_freq_high / (float)d_samp_rate,
                                      2 * M_PI);
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace radar */
} /* namespace gr */
