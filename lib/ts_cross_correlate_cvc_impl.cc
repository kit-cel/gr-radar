/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "ts_cross_correlate_cvc_impl.h"

namespace gr {
  namespace radar {

    ts_cross_correlate_cvc::sptr
    ts_cross_correlate_cvc::make(int packet_len, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new ts_cross_correlate_cvc_impl(packet_len, len_key));
    }

    /*
     * The private constructor
     */
    ts_cross_correlate_cvc_impl::ts_cross_correlate_cvc_impl(int packet_len, const std::string& len_key)
      : gr::tagged_stream_block("ts_cross_correlate_cvc",
              gr::io_signature::make(<+MIN_IN+>, <+MAX_IN+>, sizeof(<+ITYPE+>)),
              gr::io_signature::make(<+MIN_OUT+>, <+MAX_OUT+>, sizeof(<+OTYPE+>)), <+len_tag_key+>)
    {}

    /*
     * Our virtual destructor.
     */
    ts_cross_correlate_cvc_impl::~ts_cross_correlate_cvc_impl()
    {
    }

    int
    ts_cross_correlate_cvc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = /* <+set this+> */;
      return noutput_items ;
    }

    int
    ts_cross_correlate_cvc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
      <+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

