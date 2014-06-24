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
from time import sleep
import pmt
import numpy as np
from matplotlib import pyplot as plt

class qa_tracking_particle_singletarget (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# create input data (pmts)
		vec_velocity = np.linspace(-5,5,20)
		vec_range = np.linspace(1,10,20)
		target_pmts = [0]*len(vec_velocity)
		for k in range(len(vec_velocity)):
			pmt_velocity = pmt.list2(pmt.string_to_symbol("velocity"),pmt.init_f32vector(1,(vec_velocity[k],)))
			pmt_range = pmt.list2(pmt.string_to_symbol("range"),pmt.init_f32vector(1,(vec_range[k],)))
			target_pmts[k] = pmt.list2(pmt_velocity,pmt_range)
		
		# set up fg
		test_duration = 1000 # ms, do not change!
		
		num_particle = 0
		std_freq_meas = 0
		std_accel = 0
		
		# connect multiple strobes for different msgs
		src = [0]*len(target_pmts)
		for k in range(len(target_pmts)):
			src[k] = blocks.message_strobe(target_pmts[k], test_duration-400+400/len(target_pmts)*k)
		tracking = radar.tracking_particle_singletarget(num_particle, std_freq_meas, std_accel)
		snk = blocks.message_debug()
		
		for k in range(len(target_pmts)):
			self.tb.msg_connect(src[k],"strobe",tracking,"Msg in")
		self.tb.msg_connect(tracking,"Msg out",snk,"store")
		
		self.tb.start()
		sleep(test_duration/1000.0)
		self.tb.stop()
		self.tb.wait
		()
		# check data
		show_data = False # Toggle visibility of single messages
		msg_num = snk.num_messages()
		vec_out_range = []
		vec_out_velocity = []
		for k in range(msg_num):
			msg_part = snk.get_message(k)
			vel = pmt.nth(0,msg_part)
			rgn = pmt.nth(1,msg_part)
			vec_out_range.append(pmt.f32vector_elements(pmt.nth(1,rgn))[0])
			vec_out_velocity.append(pmt.f32vector_elements(pmt.nth(1,vel))[0])
			if show_data:
				print "msg:", k
				print pmt.symbol_to_string(pmt.nth(0,vel)), pmt.f32vector_elements(pmt.nth(1,vel))[0]
				print pmt.symbol_to_string(pmt.nth(0,rgn)), pmt.f32vector_elements(pmt.nth(1,rgn))[0]
				print 
		print "RANGE:"
		print vec_out_range
		print "VELOCITY:"
		print vec_out_velocity
		
		# make plots
		show_plots = False # Toggle visibility of plots
		if show_plots:
			time = range(len(vec_out_range))
			plt.figure(1)
			marker = 'o'
			p1 = plt.plot(time,vec_range,marker,time,vec_velocity,marker,time,vec_out_range,marker,time,vec_out_velocity,marker)
			plt.legend(p1,["IN range", "IN velocity", "OUT range","OUT velocity"])
			plt.title("OUTPUT")
			plt.xlabel('time')
			
			plt.show()

if __name__ == '__main__':
	gr_unittest.run(qa_tracking_particle_singletarget)#, "qa_tracking_particle_singletarget.xml")
