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

class qa_signal_generator_sync_pulse_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		test_len = 30;
		packet_len = 10;
		pulse_send = (2,3,1)
		pulse_wait = (1,2)
		amplitude = 0.7
		src = radar.signal_generator_sync_pulse_c(packet_len,pulse_send,pulse_wait,amplitude,"packet_len")
		head = blocks.head(8,test_len)
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src,head,snk)
		self.tb.run ()
		# create ref data
		ref_data = [0]*packet_len
		counter = 0
		for k in range(pulse_wait[0]):
			ref_data[counter+k] = complex(0,0)
		counter = counter+pulse_wait[0]
		for k in range(pulse_send[0]):
			ref_data[counter+k] = complex(amplitude,0)
		counter = counter+pulse_send[0]
		for k in range(pulse_wait[1]):
			ref_data[counter+k] = complex(0,0)
		counter = counter+pulse_wait[1]
		for k in range(pulse_send[1]):
			ref_data[counter+k] = complex(amplitude,0)
		counter = counter+pulse_send[1]
		for k in range(pulse_send[2]):
			ref_data[counter+k] = complex(amplitude,0)
		
		# check data
		data = snk.data()
		data1 = data[0:packet_len] # first packet
		data2 = data[0:packet_len] # second packet
		self.assertComplexTuplesAlmostEqual(ref_data,data1,4) # check first packet
		self.assertComplexTuplesAlmostEqual(ref_data,data2,4) # check second packet


if __name__ == '__main__':
	gr_unittest.run(qa_signal_generator_sync_pulse_c)#, "qa_signal_generator_sync_pulse_c.xml")
