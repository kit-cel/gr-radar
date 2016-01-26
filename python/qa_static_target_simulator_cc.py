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
from gnuradio import blocks, analog, fft
import radar_swig as radar
import numpy as np
import numpy.fft

class qa_static_target_simulator_cc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# check doppler freq (frequency shifting)
		# set up fg
		test_len = 1000
		
		packet_len = test_len
		samp_rate = 2000
		frequency = (0,)
		amplitude = 1
		
		Range = (10,)
		velocity = (15,)
		rcs = (1e9,)
		azimuth = (0,)
		position_rx = (0,)
		center_freq = 1e9
		rndm_phase = True
		self_coupling = False
		self_coupling_db = -10;
		
		src = radar.signal_generator_cw_c(packet_len,samp_rate,frequency,amplitude)
		head = blocks.head(8,test_len)
		sim = radar.static_target_simulator_cc(Range, velocity, rcs, azimuth, position_rx, samp_rate, center_freq, self_coupling_db, rndm_phase, self_coupling)
		mult = blocks.multiply_cc()
		snk = blocks.vector_sink_c()
		
		self.tb.connect(src,head,sim)
		self.tb.connect((sim,0),(mult,0))
		self.tb.connect((head,0),(mult,1))
		self.tb.connect(mult,snk)
		self.tb.run ()
		
		# check data
		data = snk.data()
		doppler_freq = 2*velocity[0]*center_freq/3e8 # peak estimation, calc with doppler formula
		fft = numpy.fft.fft(data) # get fft
		num = np.argmax(abs(fft)) # index of max sample
		fft_freq = samp_rate*num/len(fft) # calc freq out of max sample index, works only for frequencies < samp_rate/2!
		
		self.assertAlmostEqual(fft_freq,doppler_freq,2) # check if peak in data is doppler peak
		
		
	def test_002_t (self):
		#print "TEST2: SHIFT OF 1 SAMPLE"
		# set up fg
		test_len = 1000
		packet_len = 1000
		c_light = 3e8
		
		samp_rate = 1000
		freq = 5
		ampl = 1
		
		R = c_light/2/samp_rate # shift of 1 sample
		rcs = 1e9
		Range = (R,)
		velocity = (0,) # no freq shift
		rcs = (rcs,)
		azimuth = (0,)
		position_rx = (0,)
		center_freq = 1e9
		rndm_phase = False
		self_coupling = False
		self_coupling_db = 1;
		
		src = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq, ampl)
		head = blocks.head(8,test_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,"packet_len")
		sim = radar.static_target_simulator_cc(Range, velocity, rcs, azimuth, position_rx, samp_rate, center_freq, self_coupling_db, rndm_phase, self_coupling)
		snk0 = blocks.vector_sink_c()
		snk1 = blocks.vector_sink_c()
		
		self.tb.connect(src,head,s2ts,sim,snk0)
		self.tb.connect(head,snk1)
		self.tb.run()
		
		# check data with shifting of 1 sample
		data_in = snk1.data()
		data_out = snk0.data()
		data_out = data_out/max(np.real(data_out)) # rescale output data
		self.assertComplexTuplesAlmostEqual(data_out[1:len(data_out)-1],data_in[0:len(data_in)-2],4)
		
	def test_003_t (self):
		#print "TEST3: AZIMUTH ESTIMATION"
		# set up fg
		packet_len = 2**12
		samp_rate = 32000
		center_freq = 2.45e9
		freq = 0
		ampl = 1
		
		Range = (20,)
		velocity = (10,)
		rcs = (1e9,0)
		azimuth = (10,)
		position_rx = (0,0.2)
		
		src = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, freq, ampl)
		head = blocks.head(8,packet_len)
		s2ts = blocks.stream_to_tagged_stream(8,1,packet_len,'packet_len')
		sim = radar.static_target_simulator_cc(Range, velocity, rcs, azimuth, position_rx, samp_rate, center_freq, -10, False, False)
		s2v0 = blocks.stream_to_vector(8,packet_len)
		fft0 = fft.fft_vcc(packet_len,True,())
		v2s0 = blocks.vector_to_stream(8,packet_len)
		snk0 = blocks.vector_sink_c()
		s2v1 = blocks.stream_to_vector(8,packet_len)
		fft1 = fft.fft_vcc(packet_len,True,())
		v2s1 = blocks.vector_to_stream(8,packet_len)
		snk1 = blocks.vector_sink_c()
		
		mult = blocks.multiply_conjugate_cc(packet_len)
		v2s2 = blocks.vector_to_stream(8,packet_len)
		snk2 = blocks.vector_sink_c()
		
		
		self.tb.connect(src,head,s2ts,sim)
		self.tb.connect((sim,0),(s2v0,0))
		self.tb.connect(s2v0,fft0,v2s0,snk0)
		self.tb.connect((sim,1),(s2v1,0))
		self.tb.connect(s2v1,fft1,v2s1,snk1)
		
		self.tb.connect((fft0,0),(mult,1))
		self.tb.connect((fft1,0),(mult,0))
		self.tb.connect(mult,v2s2,snk2)
		
		self.tb.run()
		
		# check ffts data0 und data1 on peak
		data0 = snk0.data()
		data1 = snk1.data()
		
		data0_abs = [0]*len(data0)
		data1_abs = [0]*len(data1)
		for k in range(len(data0)):
			data0_abs[k] = abs(data0[k])
			data1_abs[k] = abs(data1[k])
		
		num0 = np.argmax(data0_abs) # index of max sample (data)
		num1 = np.argmax(data1_abs) # index of max sample (data)
		#print "NUM0:", num0, "FREQ:", num0*samp_rate/packet_len, "VELOCITY:", num0*samp_rate/packet_len*3e8/2/center_freq, "PHI:", np.angle(data0[num0])
		#print "NUM1:", num1, "FREQ:", num1*samp_rate/packet_len, "VELOCITY:", num1*samp_rate/packet_len*3e8/2/center_freq, "PHI:", np.angle(data1[num1])
		
		# check fft data2 on peak
		data2 = snk2.data()
		data2_abs = [0]*len(data2)
		for k in range(len(data0)):
			data2_abs[k] = abs(data2[k])
		num2 = np.argmax(data2_abs) # index of max sample (data)
		#print "NUM2:", num2, "FREQ:", num2*samp_rate/packet_len, "VELOCITY:", num2*samp_rate/packet_len*3e8/2/center_freq, "PHI:", np.angle(data2[num2])
		
		# assert phases of rx streams of angle(data1)-angle(data0) and angle(data2)
		self.assertAlmostEqual(np.angle(data1[num1])-np.angle(data0[num0]),np.angle(data2[num2]),4)
		
		# assert azimuth
		angle = np.arcsin(np.angle(data2[num2])*3e8/center_freq/2/np.pi/0.2)/2/np.pi*360;
		self.assertAlmostEqual(angle/azimuth[0],1,0)

if __name__ == '__main__':
	gr_unittest.run(qa_static_target_simulator_cc)#, "qa_static_target_simulator_cc.xml")
