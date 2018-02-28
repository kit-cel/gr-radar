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
 
#ifndef INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_IMPL_H
#define INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_IMPL_H

#include <radar/signal_generator_fmcw_c.h>

namespace gr {
  namespace radar {

    class signal_generator_fmcw_c_impl : public signal_generator_fmcw_c
    {
     public:
      signal_generator_fmcw_c_impl(
        const int samp_rate,
        const int samp_up,
        const int samp_down,
        const int samp_cw,
        const float freq_cw,
        const float freq_sweep,
        const float amplitude,
        const std::string& len_key
      );
      ~signal_generator_fmcw_c_impl();

      int work(int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items
      );

     private:
      const int d_samp_rate; //!< Sample rate of signal (sps)
      const int d_samp_up; //!< Number of samples on the up-sweep
      const int d_samp_down; //!< Number of samples on the down-sweep
      const int d_samp_cw; //!< Number of samples on the CW part
      const int d_packet_len; //!< Total length of packet (up, down, CW)
      const float d_freq_cw; //!< Frequency of the CW part
      const float d_freq_sweep; //!< Sweep frequency
      const float d_amplitude; //!< Amplitude

      const pmt::pmt_t d_key_len; //!< Tag identifier for TSB length tags
      const pmt::pmt_t d_value_len; //!< Precalculated value of TSB tag
      const pmt::pmt_t d_srcid; //!< srcid for TSB tag

      std::complex<float> d_phase; //!< Store phase state
      std::vector<float> d_waveform; //!< Phase increment of the waveform
      int d_wv_counter; //!< Stores the current position in the waveform
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SIGNAL_GENERATOR_FMCW_C_IMPL_H */
