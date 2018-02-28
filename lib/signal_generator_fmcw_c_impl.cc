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

#include <gnuradio/io_signature.h>
#include "signal_generator_fmcw_c_impl.h"

namespace gr {
  namespace radar {

    signal_generator_fmcw_c::sptr
    signal_generator_fmcw_c::make(
            const int samp_rate,
            const int samp_up,
            const int samp_down,
            const int samp_cw,
            const float freq_cw,
            const float freq_sweep,
            const float amplitude,
            const std::string& len_key
    ) {
      return gnuradio::get_initial_sptr(new signal_generator_fmcw_c_impl(
          samp_rate,
          samp_up,
          samp_down,
          samp_cw,
          freq_cw,
          freq_sweep,
          amplitude,
          len_key
      ));
    }

    signal_generator_fmcw_c_impl::signal_generator_fmcw_c_impl(
            const int samp_rate,
            const int samp_up,
            const int samp_down,
            const int samp_cw,
            const float freq_cw,
            const float freq_sweep,
            const float amplitude,
            const std::string& len_key)
      : gr::sync_block("signal_generator_fmcw_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
      , d_samp_rate(samp_rate)
      , d_samp_up(samp_up)
      , d_samp_down(samp_down)
      , d_samp_cw(samp_cw)
      , d_freq_cw(freq_cw)
      , d_freq_sweep(freq_sweep)
      , d_amplitude(amplitude)
      , d_packet_len(samp_up+samp_down+samp_cw)
      , d_key_len(pmt::string_to_symbol(len_key))
      , d_value_len(pmt::from_long(d_packet_len))
      , d_srcid(pmt::string_to_symbol("sig_gen_fmcw"))
      , d_wv_counter(0)
      , d_waveform(d_packet_len, freq_cw)
    {
      // Setup waveform vector: Contains cw, up-chirp, down-chirp. CW is
      // already set above. Note this vector contains the phase increment of
      // the waveform, not the IQ samples itself.
      for(int k=0; k<d_samp_up; k++) {
        d_waveform[k+d_samp_cw] =
          d_freq_cw + (d_freq_sweep/(float)d_samp_up) * k;
      }
      for(int k=0; k<d_samp_down; k++) {
        d_waveform[k+d_samp_cw+d_samp_up] =
          d_freq_cw + d_freq_sweep - (d_freq_sweep/(float)d_samp_down) * k;
      }
    }

    signal_generator_fmcw_c_impl::~signal_generator_fmcw_c_impl()
    {
    }

    int
    signal_generator_fmcw_c_impl::work(
        int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items
    ) {
      gr_complex *out = (gr_complex *) output_items[0];

      // Integrate phase for iq signal
      for (int i=0; i<noutput_items; i++) {
        // Set tag on every packet_len-th item
        if ((nitems_written(0)+i) % d_packet_len == 0) {
          add_item_tag(0, nitems_written(0)+i, d_key_len, d_value_len, d_srcid);
          d_wv_counter = 0;
        }

        // Write sample
        *out++ = d_amplitude*exp(d_phase);
        d_phase = 1j*std::fmod(
            imag(d_phase)+2*M_PI*d_waveform[d_wv_counter]/(float)d_samp_rate,
            2*M_PI
        );
        d_wv_counter++;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

