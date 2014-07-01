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
 
#ifndef INCLUDED_RADAR_USRP_ECHOTIMER_CC_IMPL_H
#define INCLUDED_RADAR_USRP_ECHOTIMER_CC_IMPL_H

#include <radar/usrp_echotimer_cc.h>

#include <uhd/utils/thread_priority.hpp>
#include <uhd/usrp/multi_usrp.hpp>

namespace gr {
  namespace radar {

    class usrp_echotimer_cc_impl : public usrp_echotimer_cc
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      usrp_echotimer_cc_impl(int n_outputs, int samp_rate, float center_freq, std::vector<int> num_delay_samps,
		std::string args_tx, std::string wire_tx, std::string clock_source_tx, std::string time_source_tx, std::string antenna_tx, float gain_tx,
		float timeout_tx, float wait_tx, float lo_offset_tx,
		std::vector<std::string> args_rx, std::vector<std::string> wire_rx, std::vector<std::string> clock_source_rx, std::vector<std::string> time_source_rx, std::vector<std::string> antenna_rx, std::vector<float> gain_rx,
		std::vector<float> timeout_rx, std::vector<float> wait_rx, std::vector<double> lo_offset_rx,
		const std::string& len_key);
      ~usrp_echotimer_cc_impl();
      void send();
      void receive(int k);
      void set_num_delay_samps(std::vector<int> num_samps);
      void set_rx_gain(std::vector<float> gain);
      void set_tx_gain(float gain);
      
      int d_samp_rate;
      float d_center_freq;
      std::vector<int> d_num_delay_samps;
      int d_n_outputs;
      std::vector<std::vector<gr_complex> > d_out_buffer;
      
      std::string d_args_tx;
      std::vector<std::string> d_args_rx;
      std::string d_clock_source_tx;
      std::vector<std::string> d_clock_source_rx;
	  std::string d_wire_tx;
	  std::vector<std::string> d_wire_rx;
	  std::string d_antenna_tx;
	  std::vector<std::string> d_antenna_rx;
	  std::string d_time_source_tx;
	  std::vector<std::string> d_time_source_rx;
      uhd::usrp::multi_usrp::sptr d_usrp_tx;
      std::vector<uhd::usrp::multi_usrp::sptr> d_usrp_rx;
      uhd::tune_request_t d_tune_request_tx;
      uhd::tune_request_t d_tune_request_rx;
      uhd::tx_streamer::sptr d_tx_stream;
      std::vector<uhd::rx_streamer::sptr> d_rx_stream;
      uhd::tx_metadata_t d_metadata_tx;
      std::vector<uhd::rx_metadata_t> d_metadata_rx;
      double d_lo_offset_tx;
      std::vector<double> d_lo_offset_rx;
      float d_timeout_tx;
      std::vector<float> d_timeout_rx;
      float d_wait_tx;
      std::vector<float> d_wait_rx;
      float d_gain_tx;
      std::vector<float> d_gain_rx;
      
      uhd::time_spec_t d_time_now_tx;
      std::vector<uhd::time_spec_t> d_time_now_rx;
      
      std::vector<gr::thread::thread*> d_thread_recv;
      std::vector<gr_complex*> d_out_recv;
      std::vector<int> d_noutput_items_recv;
      pmt::pmt_t d_time_key, d_time_val, d_srcid;
      
      gr::thread::thread d_thread_send;
      gr_complex *d_in_send;
      int d_noutput_items_send;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_USRP_ECHOTIMER_CC_IMPL_H */

