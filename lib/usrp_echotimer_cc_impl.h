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
      usrp_echotimer_cc_impl(int samp_rate, float center_freq, int num_delay_samps, std::vector<size_t> channel_nums_tx, std::vector<size_t> channel_nums_rx,
		std::string args, std::vector<std::string> time_source, std::vector<std::string> clock_source,
		std::vector<std::string> wire, std::vector<float> wait, std::vector<float> timeout,
        std::vector<std::string> tx_subdev_spec, std::vector<std::string> rx_subdev_spec,
		std::vector<std::string> antenna_tx, std::vector<std::string> antenna_rx, std::vector<float> gain_tx, std::vector<float> gain_rx,
		const std::string& len_key);
      ~usrp_echotimer_cc_impl();
      void send(std::vector<gr_complex*> in_ptrs, int noutput_items, uhd::time_spec_t time_now);
      void receive(std::vector<gr_complex*> out_ptrs, int noutput_items, uhd::time_spec_t time_now);
      void set_num_delay_samps(int num_samps);

      int d_samp_rate, d_num_delay_samps, d_num_tx, d_num_rx, d_num_mboard;
      std::vector<float> d_wait, d_timeout;

      uhd::usrp::multi_usrp::sptr d_usrp;
      uhd::tx_streamer::sptr d_tx_stream;
      uhd::rx_streamer::sptr d_rx_stream;

      std::vector<std::vector<gr_complex> > d_out_buffer;
      std::vector<gr_complex*> d_out_buffer_ptrs;

      pmt::pmt_t d_time_key, d_time_val, d_srcid;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_USRP_ECHOTIMER_CC_IMPL_H */

