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
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "usrp_echotimer_cc_impl.h"
#include <iostream>

namespace gr {
  namespace radar {

    usrp_echotimer_cc::sptr
    usrp_echotimer_cc::make(int samp_rate, float center_freq, int num_delay_samps, std::vector<size_t> channel_nums_tx, std::vector<size_t> channel_nums_rx,
		std::string args, std::vector<std::string> time_source, std::vector<std::string> clock_source,
		std::vector<std::string> wire, std::vector<float> wait, std::vector<float> timeout,
		std::vector<std::string> antenna_tx, std::vector<std::string> antenna_rx, std::vector<float> gain_tx, std::vector<float> gain_rx,
		const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new usrp_echotimer_cc_impl(samp_rate, center_freq, num_delay_samps, channel_nums_tx, channel_nums_rx,
			args, time_source, clock_source, wire, wait, timeout,
			antenna_tx, antenna_rx, gain_tx, gain_rx,
			len_key));
    }

    /*
     * The private constructor
     */
    usrp_echotimer_cc_impl::usrp_echotimer_cc_impl(int samp_rate, float center_freq, int num_delay_samps, std::vector<size_t> channel_nums_tx, std::vector<size_t> channel_nums_rx,
		std::string args, std::vector<std::string> time_source, std::vector<std::string> clock_source,
		std::vector<std::string> wire, std::vector<float> wait, std::vector<float> timeout,
		std::vector<std::string> antenna_tx, std::vector<std::string> antenna_rx, std::vector<float> gain_tx, std::vector<float> gain_rx,
		const std::string& len_key)
      : gr::tagged_stream_block("usrp_echotimer_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(channel_nums_rx.size(), channel_nums_rx.size(), sizeof(gr_complex)), len_key)
    {
		d_samp_rate = samp_rate;
		d_num_delay_samps = num_delay_samps;
		d_wait = wait;
		d_timeout = timeout;
		d_num_tx = channel_nums_tx.size();
		d_num_rx = channel_nums_rx.size();
		
		//*** Setup USRPs ***//
		
		// General setup of multi_usrp object
		d_usrp = uhd::usrp::multi_usrp::make(args);
		std::cout << "Using USRP Device(s): " << std::endl << d_usrp->get_pp_string() << std::endl;
		int num_mboard = d_usrp->get_num_mboards();
		std::cout << "Num Mboards: " << num_mboard << std::endl;
		
		//*** Set Mboard option ***//
		
		// Set time sources
		for(int k=0; k<num_mboard; k++){
			d_usrp->set_time_source(time_source[k], k);
		}
		
		// Set clock sources
		for(int k=0; k<num_mboard; k++){
			d_usrp->set_clock_source(clock_source[k], k);
		}
		
		// Set time
		// FIXME: implement set time, with gpsdo
		
		//*** Get TX and RX streamer and channels ***//
		
		// Get TX streamer and channels
		uhd::stream_args_t stream_args_tx("fc32", wire[0]);
		stream_args_tx.channels = channel_nums_tx;
		d_tx_stream = d_usrp->get_tx_stream(stream_args_tx);
		
		// Get RX streamer and channels
		uhd::stream_args_t stream_args_rx("fc32", wire[1]);
		stream_args_rx.channels = channel_nums_rx;
		d_rx_stream = d_usrp->get_rx_stream(stream_args_rx);
		
		//***  Set channel options ***//
		
		// Set sample rate on TX and RX
		d_usrp->set_tx_rate(samp_rate);
		d_usrp->set_rx_rate(samp_rate);
		
		// Set center frequency on TX and RX
		uhd::tune_request_t tune_request = uhd::tune_request_t(center_freq);
		d_usrp->set_tx_freq(tune_request);
		d_usrp->set_rx_freq(tune_request);
		
		// Set antennas
		for(int k=0; k<d_num_tx; k++){
			d_usrp->set_tx_antenna(antenna_tx[k], channel_nums_tx[k]);
		}
		for(int k=0; k<d_num_rx; k++){
			d_usrp->set_rx_antenna(antenna_rx[k], channel_nums_rx[k]);
		}
		
		// Set gain
		for(int k=0; k<d_num_tx; k++){
			d_usrp->set_tx_gain(gain_tx[k], channel_nums_tx[k]);
		}
		for(int k=0; k<d_num_rx; k++){
			d_usrp->set_rx_gain(gain_rx[k], channel_nums_rx[k]);
		}
		
		//*** Misc ***//
		
		// Resize output buffer
		d_out_buffer.resize(d_num_rx);
		d_out_buffer_ptrs.resize(d_num_rx);
		
		// Setup rx_time pmt
		d_time_key = pmt::string_to_symbol("rx_time");
		d_srcid = pmt::string_to_symbol("usrp_echotimer");
		
		// Setup thread priority
		//uhd::set_thread_priority_safe(); // necessary? doesnt work...
		
		// Sleep to get sync done
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); // FIXME: necessary?
	}

    /*
     * Our virtual destructor.
     */
    usrp_echotimer_cc_impl::~usrp_echotimer_cc_impl()
    {
    }

    int
    usrp_echotimer_cc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0];
      return noutput_items ;
    }
    
    void
    usrp_echotimer_cc_impl::set_num_delay_samps(int num_samps){
		d_num_delay_samps = num_samps;
	}
    
    void
    usrp_echotimer_cc_impl::send()
    {
		// Setup metadata for first package
        d_metadata_tx.start_of_burst = true;
		d_metadata_tx.end_of_burst = false;
		d_metadata_tx.has_time_spec = true;
		d_metadata_tx.time_spec = d_time_now_tx+uhd::time_spec_t(d_wait[0]); // Timespec needed?
		
		// Send input buffer
		size_t num_acc_samps = 0; // Number of accumulated samples
		size_t num_tx_samps, total_num_samps;
		total_num_samps = d_noutput_items_tx;
		// Data to USRP
		num_tx_samps = d_tx_stream->send(d_in_tx, total_num_samps, d_metadata_tx, total_num_samps/(float)d_samp_rate+d_timeout[0]);
		// Get timeout
		if (num_tx_samps < total_num_samps) std::cerr << "Send timeout..." << std::endl;
		
		//send a mini EOB packet
		d_metadata_tx.start_of_burst = false;
		d_metadata_tx.end_of_burst = true;
		d_metadata_tx.has_time_spec = false;
		d_tx_stream->send("", 0, d_metadata_tx);
    }
    
    void
    usrp_echotimer_cc_impl::receive()
    {
		// Setup RX streaming
		size_t total_num_samps = d_noutput_items_rx;
		uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
		stream_cmd.num_samps = total_num_samps;
		stream_cmd.stream_now = false;
		stream_cmd.time_spec = d_time_now_rx+uhd::time_spec_t(d_wait[1]);
		d_rx_stream->issue_stream_cmd(stream_cmd);
		
		size_t num_rx_samps;
		// Receive a packet
		num_rx_samps = d_rx_stream->recv(d_out_rx, total_num_samps, d_metadata_rx, total_num_samps/(float)d_samp_rate+d_timeout[1]);
		
		// Save timestamp
		d_time_val = pmt::make_tuple
			(pmt::from_uint64(d_metadata_rx.time_spec.get_full_secs()),
			 pmt::from_double(d_metadata_rx.time_spec.get_frac_secs()));

		// Handle the error code
		if (d_metadata_rx.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE){
			throw std::runtime_error(str(boost::format("Receiver error %s") % d_metadata_rx.strerror()));
		}

		if (num_rx_samps < total_num_samps) std::cerr << "Receive timeout before all samples received..." << std::endl;
    }

    int
    usrp_echotimer_cc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        gr_complex *in = (gr_complex *) input_items[0]; // remove const
        gr_complex *out;
        
        // Set output items on packet length
        noutput_items = ninput_items[0];
        
        // Resize output buffer
        if(d_out_buffer[0].size()!=noutput_items){
			for(int k=0; k<d_num_rx; k++) d_out_buffer[k].resize(noutput_items);
			for(int k=0; k<d_num_rx; k++) d_out_buffer_ptrs[k] = &d_out_buffer[k].front();
		}
		
		// Get actual time on USRPs
        uhd::time_spec_t time_now = d_usrp->get_time_now();
        d_time_now_tx = time_now;
        d_time_now_rx = time_now;
		
		// TX thread
        d_in_tx = in;
        d_noutput_items_tx = noutput_items;
        d_thread_tx = gr::thread::thread(boost::bind(&usrp_echotimer_cc_impl::send, this));
        
        // RX thread
        d_out_rx = d_out_buffer_ptrs;
        d_noutput_items_rx = noutput_items;
        d_thread_rx = gr::thread::thread(boost::bind(&usrp_echotimer_cc_impl::receive, this));
        
        // Wait for threads to complete
        d_thread_tx.join();
        d_thread_rx.join();
        
        // Shift of number delay samples (fill with zeros)
        for(int k=0; k<d_num_rx; k++){
			out = (gr_complex *) output_items[k];
			memcpy(out,d_out_rx[k]+d_num_delay_samps,(noutput_items-d_num_delay_samps)*sizeof(gr_complex)); // push buffer to output
			memset(out+(noutput_items-d_num_delay_samps),0,d_num_delay_samps*sizeof(gr_complex)); // set zeros
		}
        
        // Setup rx_time tag
        for(int k=0; k<d_num_rx; k++){
			add_item_tag(k, nitems_written(k), d_time_key, d_time_val, d_srcid);
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

