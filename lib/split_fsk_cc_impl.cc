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
#include "split_fsk_cc_impl.h"

namespace gr {
  namespace radar {

    split_fsk_cc::sptr
    split_fsk_cc::make(int samp_per_freq, int samp_discard, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new split_fsk_cc_impl(samp_per_freq, samp_discard, len_key));
    }

    /*
     * The private constructor
     */
    split_fsk_cc_impl::split_fsk_cc_impl(int samp_per_freq, int samp_discard, const std::string& len_key)
      : gr::tagged_stream_block("split_fsk_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(2, 2, sizeof(gr_complex)), len_key)
    {
		d_samp_per_freq = samp_per_freq;
		d_samp_discard = samp_discard;
		
		if(d_samp_per_freq<=d_samp_discard) throw std::runtime_error("samp_per_freq is smaller or equal than samp_discard");
		
		set_tag_propagation_policy(TPP_DONT); // does not apply on stream tags!
	}

    /*
     * Our virtual destructor.
     */
    split_fsk_cc_impl::~split_fsk_cc_impl()
    {
    }

    int
    split_fsk_cc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0]/2; // max is half of ninput_items
      return noutput_items ;
    }

    int
    split_fsk_cc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out0 = (gr_complex *) output_items[0];
        gr_complex *out1 = (gr_complex *) output_items[1];

        // Do <+signal processing+>
        
        // get all tags, reset offset and push to outputs
		get_tags_in_range(d_tags,0,nitems_read(0),nitems_read(0)+1);
		for(int k=0; k<d_tags.size(); k++){
			add_item_tag(0,nitems_written(0),d_tags[k].key,d_tags[k].value,d_tags[k].srcid);
			add_item_tag(1,nitems_written(1),d_tags[k].key,d_tags[k].value,d_tags[k].srcid);
		}
        
        // Resize packet len and set noutput_items
        d_nblocks = ninput_items[0]/2/d_samp_per_freq;
        noutput_items = d_nblocks*(d_samp_per_freq-d_samp_discard);
        update_length_tags(noutput_items,2); // update length tag
        
        // Push input_items to outputs
        int k=0;
        while(k<ninput_items[0]){
			// first freq
			k+=d_samp_discard;
			for(int l=0; l<d_samp_per_freq-d_samp_discard; l++) *out0++ = in[k+l];
			k+=d_samp_per_freq-d_samp_discard;
			// second freq
			k+=d_samp_discard;
			for(int l=0; l<d_samp_per_freq-d_samp_discard; l++) *out1++ = in[k+l];
			k+=d_samp_per_freq-d_samp_discard;
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */

