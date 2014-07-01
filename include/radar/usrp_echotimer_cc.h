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

#ifndef INCLUDED_RADAR_USRP_ECHOTIMER_CC_H
#define INCLUDED_RADAR_USRP_ECHOTIMER_CC_H

#include <radar/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief <+description of block+>
     * \ingroup radar
     *
     */
    class RADAR_API usrp_echotimer_cc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<usrp_echotimer_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::usrp_echotimer_cc.
       *
       * To avoid accidental use of raw pointers, radar::usrp_echotimer_cc's
       * constructor is in a private implementation
       * class. radar::usrp_echotimer_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n_outputs, int samp_rate, float center_freq, std::vector<int> num_delay_samps,
		std::string args_tx, std::string wire_tx, std::string clock_source_tx, std::string time_source_tx, std::string antenna_tx, float gain_tx,
		float timeout_tx, float wait_tx, float lo_offset_tx,
		std::vector<std::string> args_rx, std::vector<std::string> wire_rx, std::vector<std::string> clock_source_rx, std::vector<std::string> time_source_rx, std::vector<std::string> antenna_rx, std::vector<float> gain_rx,
		std::vector<float> timeout_rx, std::vector<float> wait_rx, std::vector<double> lo_offset_rx,
		const std::string& len_key="packet_len");
		
	  virtual void set_num_delay_samps(std::vector<int> num_samps) = 0;
	  virtual void set_rx_gain(std::vector<float> gain) = 0;
      virtual void set_tx_gain(float gain) = 0;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_USRP_ECHOTIMER_CC_H */

