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
from gnuradio import blocks
import radar_swig as radar

class qa_ofdm_divide_vcvc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 20
		vlen = 5
		ts_len = test_len/vlen/2
		
		in_data0 = [0]*test_len
		in_data1 = [0]*test_len
		for k in range(test_len):
			in_data0[k] = complex(k-4, k+1)
			in_data1[k] = complex(k+3, k-2)
			
		src0 = blocks.vector_source_c(in_data0)
		s2v0 = blocks.stream_to_vector(8,vlen)
		s2ts0 = blocks.stream_to_tagged_stream(8,vlen,ts_len,'packet_len')
		src1 = blocks.vector_source_c(in_data1)
		s2v1 = blocks.stream_to_vector(8,vlen)
		s2ts1 = blocks.stream_to_tagged_stream(8,vlen,ts_len,'packet_len')
		div = radar.ofdm_divide_vcvc(vlen,2*vlen,())
		v2s = blocks.vector_to_stream(8,2*vlen)
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src0,s2v0,s2ts0)
		self.tb.connect(src1,s2v1,s2ts1)
		self.tb.connect((s2ts0,0),(div,0))
		self.tb.connect((s2ts1,0),(div,1))
		self.tb.connect(div,v2s,snk)
		
		self.tb.run ()
		
		# get ref data
		ref_data = [0]*test_len*2
		for k in range(test_len/vlen):
			for l in range(vlen):
				ref_data[2*vlen*k+l] = in_data0[vlen*k+l]/in_data1[vlen*k+l]
		
		# check data
		out_data =  snk.data()
		for k in range(len(out_data)):
			self.assertAlmostEqual(ref_data[k], out_data[k],4)
			
	def test_002_t (self):
		# set up fg
		test_len = 20
		vlen = 5
		ts_len = test_len/vlen/2
		
		in_data0 = [0]*test_len
		in_data1 = [0]*test_len
		for k in range(test_len):
			in_data0[k] = complex(k-4, k+1)
			in_data1[k] = complex(k+3, k-2)
			
		src0 = blocks.vector_source_c(in_data0)
		s2v0 = blocks.stream_to_vector(8,vlen)
		s2ts0 = blocks.stream_to_tagged_stream(8,vlen,ts_len,'packet_len')
		src1 = blocks.vector_source_c(in_data1)
		s2v1 = blocks.stream_to_vector(8,vlen)
		s2ts1 = blocks.stream_to_tagged_stream(8,vlen,ts_len,'packet_len')
		div = radar.ofdm_divide_vcvc(vlen,vlen,())
		v2s = blocks.vector_to_stream(8,vlen)
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src0,s2v0,s2ts0)
		self.tb.connect(src1,s2v1,s2ts1)
		self.tb.connect((s2ts0,0),(div,0))
		self.tb.connect((s2ts1,0),(div,1))
		self.tb.connect(div,v2s,snk)
		
		self.tb.run ()
		
		# get ref data
		ref_data = [0]*test_len
		for k in range(test_len/vlen):
			for l in range(vlen):
				ref_data[vlen*k+l] = in_data0[vlen*k+l]/in_data1[vlen*k+l]
		
		# check data
		out_data =  snk.data()
		for k in range(len(out_data)):
			self.assertAlmostEqual(ref_data[k], out_data[k],4)

if __name__ == '__main__':
	gr_unittest.run(qa_ofdm_divide_vcvc)#, "qa_ofdm_divide_vcvc.xml")
