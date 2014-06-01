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
 
#ifndef INCLUDED_RADAR_ESTIMATOR_MSG_GATE_IMPL_H
#define INCLUDED_RADAR_ESTIMATOR_MSG_GATE_IMPL_H

#include <radar/estimator_msg_gate.h>

namespace gr {
  namespace radar {

    class estimator_msg_gate_impl : public estimator_msg_gate
    {
     private:
      // Nothing to declare in this block.

     public:
      estimator_msg_gate_impl(std::vector<std::string> keys, std::vector<float> val_min, std::vector<float> val_max, int verbose);
      ~estimator_msg_gate_impl();
      void handle_msg(pmt::pmt_t msg);
      
      int d_verbose;
      std::vector<std::string> d_keys;
      std::vector<float> d_val_max;
      std::vector<float> d_val_min;
      
      pmt::pmt_t d_port_id_in, d_port_id_out;
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_ESTIMATOR_MSG_GATE_IMPL_H */

