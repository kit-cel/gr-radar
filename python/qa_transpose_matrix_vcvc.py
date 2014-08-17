#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 Communications Engineering Lab, KIT.
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

class qa_transpose_matrix_vcvc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		# check samples on output
		test_len = 12
		in_data = range(test_len)
		vlen_in = 3
		vlen_out = 4
		
		src = blocks.vector_source_c(in_data)
		stv = blocks.stream_to_vector(8,vlen_in)
		s2ts = blocks.stream_to_tagged_stream(8,vlen_in,test_len/vlen_in,'packet_len')
		transpose = radar.transpose_matrix_vcvc(vlen_in,vlen_out,'packet_len')
		vts = blocks.vector_to_stream(8,vlen_out)
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src,stv,s2ts,transpose,vts,snk)
		self.tb.run()
		
		# check data
		out_data = snk.data()
		ref_data = [0]*len(in_data)
		counter = 0;
		for k in range(vlen_in):
			for l in range(test_len/vlen_in):
				ref_data[counter] = in_data[l*vlen_in+k]
				counter = counter+1
			
		out_data_real = [0]*len(in_data)
		for k in range(len(in_data)):
			out_data_real[k] = out_data[k].real
				
		for k in range(len(out_data_real)):
			self.assertEqual(out_data_real[k],ref_data[k])
			
	def test_002_t (self):
		# set up fg
		# check on reverse by using two times
		test_len = 12
		in_data = range(test_len)
		vlen_in = 3
		vlen_out = 4
		
		src = blocks.vector_source_c(in_data)
		stv = blocks.stream_to_vector(8,vlen_in)
		s2ts = blocks.stream_to_tagged_stream(8,vlen_in,test_len/vlen_in,'packet_len')
		transpose1 = radar.transpose_matrix_vcvc(vlen_in,vlen_out,'packet_len')
		transpose2 = radar.transpose_matrix_vcvc(vlen_out,vlen_in,'packet_len')
		vts = blocks.vector_to_stream(8,vlen_in)
		snk = blocks.vector_sink_c()
		vts2 = blocks.vector_to_stream(8,vlen_out)
		snk2 = blocks.vector_sink_c()
		
		self.tb.connect(src,stv,s2ts,transpose1,transpose2,vts,snk)
		self.tb.connect(transpose1,vts2,snk2)
		self.tb.run()
		
		# check data
		out_data_real = [0]*len(in_data)
		out_data = snk.data()
		for k in range(len(in_data)):
			out_data_real[k] = out_data[k].real
			
		print "Input data:", in_data
		print "Output data:", out_data_real
		print "Transpose data:", snk2.data()
			
		for k in range(len(out_data_real)):
			self.assertEqual(out_data_real[k],in_data[k])


if __name__ == '__main__':
	gr_unittest.run(qa_transpose_matrix_vcvc)#, "qa_transpose_matrix_vcvc.xml")
