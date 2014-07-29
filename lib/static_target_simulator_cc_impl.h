/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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
 
#ifndef INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_IMPL_H
#define INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_IMPL_H

#include <radar/static_target_simulator_cc.h>
#include <fftw3.h>

namespace gr {
  namespace radar {

    class static_target_simulator_cc_impl : public static_target_simulator_cc
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      static_target_simulator_cc_impl(std::vector<float> range, std::vector<float> velocity, std::vector<float> rcs, std::vector<float> azimuth, std::vector<float> position_rx,
													int samp_rate, float center_freq, float self_coupling_db, bool rndm_phaseshift, bool self_coupling, const std::string& len_key);
      ~static_target_simulator_cc_impl();
      void setup_targets(std::vector<float> range, std::vector<float> velocity, std::vector<float> rcs, std::vector<float> azimuth, std::vector<float> position_rx,
													int samp_rate, float center_freq, float self_coupling_db, bool rndm_phaseshift, bool self_coupling);
      
      std::vector<float> d_range, d_velocity, d_rcs, d_azimuth, d_position_rx;
      int d_samp_rate;
      float d_center_freq;
      int d_hold_noutput;
      bool d_rndm_phaseshift;
      
      bool d_self_coupling;
      float d_self_coupling_db;
      
      int d_num_targets;
      std::vector<float> d_doppler, d_scale_ampl, d_timeshift;
      gr_complex d_phase_doppler, d_phase_time, d_phase_random;
      std::vector<gr_complex> d_hold_in;
      
      fftwf_plan d_fft_plan, d_ifft_plan;
      std::vector<gr_complex> d_in_fft;
      std::vector<std::vector<gr_complex> > d_filt_doppler, d_filt_phase, d_filt_self_coupling;
      std::vector<std::vector<std::vector<gr_complex> > > d_filt_time;
      
      pmt::pmt_t d_key, d_val, d_srcid;
      uint64_t d_time_sec;
      double d_time_frac_sec;
           
      const static float c_light = 3e8;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_STATIC_TARGET_SIMULATOR_CC_IMPL_H */

