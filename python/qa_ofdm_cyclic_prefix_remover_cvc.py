#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks, digital
import radar_swig as radar

class qa_ofdm_cyclic_prefix_remover_cvc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        test_len = 1000
        fft_len = 100;
        cp_len = fft_len/4
        
        in_data = range(0,fft_len)*(test_len/fft_len);
        
        src = blocks.vector_source_c(in_data)
        s2v = blocks.stream_to_vector(8,fft_len)
        s2ts = blocks.stream_to_tagged_stream(8,fft_len,test_len/fft_len,'packet_len')
        cp_add = digital.ofdm_cyclic_prefixer(fft_len,fft_len+cp_len,0,'packet_len')
        cp_remove = radar.ofdm_cyclic_prefix_remover_cvc(fft_len,cp_len,'packet_len')
        v2s = blocks.vector_to_stream(8,fft_len)
        snk = blocks.vector_sink_c()
        
        self.tb.connect(src,s2v,s2ts,cp_add,cp_remove,v2s,snk)
        self.tb.run ()
        
        # check data
        out_data = snk.data()
        out_data_real = [0]*len(out_data)
        for k in range(len(out_data)):
			out_data_real[k] = out_data[k].real
        
        for k in range(len(in_data)):
			self.assertEqual(in_data[k],out_data_real[k])


if __name__ == '__main__':
    gr_unittest.run(qa_ofdm_cyclic_prefix_remover_cvc)#, "qa_ofdm_cyclic_prefix_remover_cvc.xml")
