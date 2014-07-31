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
#include "estimator_rcs_impl.h"

namespace gr {
  namespace radar {

    estimator_rcs::sptr
    estimator_rcs::make(int num_mean, float center_freq, float antenna_gain_tx, float antenna_gain_rx, float usrp_gain_tx, float usrp_gain_rx, float amplitude)
    {
      return gnuradio::get_initial_sptr
        (new estimator_rcs_impl(num_mean, center_freq, antenna_gain_tx, antenna_gain_rx, usrp_gain_tx, usrp_gain_rx, amplitude));
    }

    /*
     * The private constructor
     */
    estimator_rcs_impl::estimator_rcs_impl(int num_mean, float center_freq, float antenna_gain_tx, float antenna_gain_rx, float usrp_gain_tx, float usrp_gain_rx, float amplitude)
      : gr::block("estimator_rcs",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
        d_num_mean = num_mean;
        d_center_freq = center_freq;
        d_antenna_gain_tx = antenna_gain_tx;
        d_antenna_gain_rx = antenna_gain_rx;
        d_usrp_gain_tx = usrp_gain_tx;
        d_usrp_gain_rx = usrp_gain_rx;
        d_amplitude = amplitude;

        // Register input message port
        d_port_id_in = pmt::mp("Msg in");
        message_port_register_in(d_port_id_in);
        set_msg_handler(d_port_id_in, boost::bind(&estimator_rcs_impl::handle_msg, this, _1));
        
        // Register output message port
        d_port_id_out = pmt::mp("Msg out");
        message_port_register_out(d_port_id_out);
    }

    /*
     * Our virtual destructor.
     */
    estimator_rcs_impl::~estimator_rcs_impl()
    {
    }

    void
    estimator_rcs_impl::handle_msg(pmt::pmt_t msg)
    {
        // Read msg from peak detector
        d_msg_hold.clear();
        pmt::pmt_t msg_part;
        bool found_range = false;
        bool found_power = false;
        for(int k=0; k<pmt::length(msg); k++){
          msg_part = pmt::nth(k,msg);
          if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="range"){
            d_prange = pmt::nth(1,msg_part);
            d_msg_hold.push_back(msg_part);
            found_range = true;
          }
          else if(pmt::symbol_to_string(pmt::nth(0,msg_part))=="power"){
            d_ppower = pmt::nth(1,msg_part);
            found_power = true;
          }
          else{
            d_msg_hold.push_back(msg_part);
          }
        }

        if(not(found_range&&found_power)) throw std::runtime_error("range or power identifier (symbol) not found");

        d_range = pmt::f32vector_elements(d_prange);
        d_power = pmt::f32vector_elements(d_ppower);
        d_rcs.clear();

        if(d_range.size()!=d_power.size()) throw std::runtime_error("range and power vectors do not have same size");

        // Calculate RCS
        for(int k=0; k<d_range.size(); k++){
            d_rcs.push_back(1);
        }
        
        // Push pmt to output msg port
        d_rcs_key = pmt::string_to_symbol("rcs"); // identifier velocity
        d_rcs_value = pmt::init_f32vector(d_rcs.size(), d_rcs); // vector to pmt
        d_rcs_pack = pmt::list2(d_rcs_key, d_rcs_value); // make list for velocity information
        
        d_value = pmt::list1(d_rcs_pack);
        for(int k=0; k<d_msg_hold.size(); k++){
            d_value = pmt::list_add(d_value, d_msg_hold[k]);
        }
        
        message_port_pub(d_port_id_out,d_value); // publish message
    }

  } /* namespace radar */
} /* namespace gr */

