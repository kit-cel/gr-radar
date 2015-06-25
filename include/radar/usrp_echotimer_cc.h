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
      static sptr make(int samp_rate, float center_freq, int num_delay_samps, std::vector<size_t> channel_nums_tx, std::vector<size_t> channel_nums_rx,
		std::string args, std::vector<std::string> time_source, std::vector<std::string> clock_source,
		std::vector<std::string> wire, std::vector<float> wait, std::vector<float> timeout,
        std::vector<std::string> tx_subdev_spec, std::vector<std::string> rx_subdev_spec,
		std::vector<std::string> antenna_tx, std::vector<std::string> antenna_rx, std::vector<float> gain_tx, std::vector<float> gain_rx,
		const std::string& len_key="packet_len");
		
	  virtual void set_num_delay_samps(int num_samps) = 0;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_USRP_ECHOTIMER_CC_H */

