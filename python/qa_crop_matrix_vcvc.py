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

class qa_crop_matrix_vcvc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		in_data = range(16)
		vlen_in = 4
		
		crop_x = (0,3)
		crop_y = (0,3)
		
		src = blocks.vector_source_c(in_data)
		s2v = blocks.stream_to_vector(8,vlen_in)
		s2ts = blocks.stream_to_tagged_stream(8,vlen_in,len(in_data)/vlen_in,'packet_len')
		crop = radar.crop_matrix_vcvc(vlen_in,crop_x,crop_y)
		v2s = blocks.vector_to_stream(8,crop_x[1]-crop_x[0])
		snk = blocks.vector_sink_c()
		debug = blocks.tag_debug(8,'debug')
		
		self.tb.connect(src,s2v,s2ts,crop,v2s,snk)
		self.tb.connect(v2s,debug)
		self.tb.run ()
		# check data
		ref_data = (0,1,2,4,5,6,8,9,10)
		out_data = snk.data()
		print "INDATA:", in_data
		print "REF: ", ref_data
		print "OUTPUT:", out_data
		for k in range(len(out_data)):
			self.assertEqual(out_data[k].real,ref_data[k])
			
	def test_002_t (self):
		# set up fg
		in_data = range(20)
		vlen_in = 5
		
		crop_x = (2,4)
		crop_y = (1,4)
		
		src = blocks.vector_source_c(in_data)
		s2v = blocks.stream_to_vector(8,vlen_in)
		s2ts = blocks.stream_to_tagged_stream(8,vlen_in,len(in_data)/vlen_in,'packet_len')
		crop = radar.crop_matrix_vcvc(vlen_in,crop_x,crop_y)
		v2s = blocks.vector_to_stream(8,crop_x[1]-crop_x[0])
		snk = blocks.vector_sink_c()
		debug = blocks.tag_debug(8,'debug')
		
		self.tb.connect(src,s2v,s2ts,crop,v2s,snk)
		self.tb.connect(v2s,debug)
		self.tb.run ()
		# check data
		ref_data = (7,8,12,13,17,18)
		out_data = snk.data()
		print "INDATA:", in_data
		print "REF: ", ref_data
		print "OUTPUT:", out_data
		for k in range(len(out_data)):
			self.assertEqual(out_data[k].real,ref_data[k])

if __name__ == '__main__':
	gr_unittest.run(qa_crop_matrix_vcvc)#, "qa_crop_matrix_vcvc.xml")
