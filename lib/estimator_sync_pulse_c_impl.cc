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
#include "estimator_sync_pulse_c_impl.h"
#include <volk/volk.h>

namespace gr {
  namespace radar {

    estimator_sync_pulse_c::sptr
    estimator_sync_pulse_c::make(int num_xcorr, const std::string len_key)
    {
      return gnuradio::get_initial_sptr
        (new estimator_sync_pulse_c_impl(num_xcorr, len_key));
    }

    /*
     * The private constructor
     */
    estimator_sync_pulse_c_impl::estimator_sync_pulse_c_impl(int num_xcorr, const std::string len_key)
      : gr::tagged_stream_block("estimator_sync_pulse_c",
              gr::io_signature::make(2, 2, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0), len_key)
    {
		d_num_xcorr = num_xcorr;
		
		// Register message port
		d_port_id = pmt::mp("Msg out");
		message_port_register_out(d_port_id);
	}
	
	void
    estimator_sync_pulse_c_impl::set_num_xcorr(int num)
    {
      d_num_xcorr = num;
    }

    /*
     * Our virtual destructor.
     */
    estimator_sync_pulse_c_impl::~estimator_sync_pulse_c_impl()
    {
    }

    int
    estimator_sync_pulse_c_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }

    int
    estimator_sync_pulse_c_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_tx = (const gr_complex *) input_items[0];
        const gr_complex *in_rx = (const gr_complex *) input_items[1];
        
        // Do cross correlation and find max
        std::complex<float> xcorr, xcorr_hold;
        int ninput_items_min, num_delay;
        num_delay = -1;
        xcorr_hold = -1;
        
        ninput_items_min = std::min(ninput_items[0],ninput_items[1]);
        for(int k=0; k<d_num_xcorr; k++){
			volk_32fc_x2_conjugate_dot_prod_32fc(&xcorr, in_tx,in_rx+k,ninput_items_min-k);
			if(xcorr.real()>xcorr_hold.real()){
				xcorr_hold = xcorr.real();
				num_delay = k;
			}
		}
        
        // get rx_time tag
         std::vector< tag_t > tags;
        pmt::pmt_t ptimestamp, msg_out, pxcorr;
		get_tags_in_range(tags,0,nitems_read(0),nitems_read(0)+1,pmt::string_to_symbol("rx_time"));
		
		// setup msg pmt
		if(tags.size()>0) ptimestamp = tags[0].value;
		else ptimestamp = pmt::list2(pmt::string_to_symbol("rx_time"),pmt::make_tuple(pmt::from_uint64(0),pmt::from_double(-1))); // if no timetag is found, set to 0 and frac_sec to -1
		
		pxcorr = pmt::list2(pmt::string_to_symbol("sync_pulse"),pmt::from_long(std::abs(num_delay)));
		msg_out = pmt::list2(ptimestamp,pxcorr);
		
		// publish message
		message_port_pub(d_port_id,msg_out);

        // Do <+signal processing+>

        // Tell runtime system how many output items we produced.
        return 0;
    }

  } /* namespace radar */
} /* namespace gr */

