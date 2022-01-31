#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 Communications Engineering Lab, KIT.
# Copyright 2022 A. Maitland Bottoms
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
  from gnuradio import radar
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import radar

class qa_split_cc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		samp_cw = 300
		samp_up = 200
		samp_down = 100
		
		mult = 3
		test_len = (samp_cw+samp_up+samp_down)*mult
		
		samp_rate = 2000
		len_key = "packet_len"
		packet_parts = (samp_cw, samp_up, samp_down)
		
		src = radar.signal_generator_fmcw_c(samp_rate, samp_up, samp_down, samp_cw, 500, 250, 1, len_key);
		head = blocks.head(8,test_len)
		split0 = radar.split_cc(0,packet_parts,len_key)
		split1 = radar.split_cc(1,packet_parts,len_key)
		split2 = radar.split_cc(2,packet_parts,len_key)
		snk0 = blocks.vector_sink_c()
		snk1 = blocks.vector_sink_c()
		snk2 = blocks.vector_sink_c()
		
		self.tb.connect(src,head)
		self.tb.connect(head,split0,snk0)
		self.tb.connect(head,split1,snk1)
		self.tb.connect(head,split2,snk2)
		self.tb.run ()
		
		# check data
		self.assertEqual(len(snk0.data()),mult*samp_cw) # check correct number of samples in every sink
		self.assertEqual(len(snk1.data()),mult*samp_up)
		self.assertEqual(len(snk2.data()),mult*samp_down)
		
	def test_002_t (self):
		# set up fg
		in_data = (1, 2, 3, 4, 5, 6)
		packet_len = len(in_data)
		
		src = blocks.vector_source_c(in_data)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		split = radar.split_cc(1,(1,3,2))
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src,s2ts,split,snk)
		self.tb.run ()
		
		# check data
		self.assertTupleEqual(tuple(snk.data()),(2,3,4)) # check if correct data is pushed to output

if __name__ == '__main__':
	gr_unittest.run(qa_split_cc)#, "qa_split_cc.xml")
