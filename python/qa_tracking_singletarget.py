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
import random

class qa_tracking_singletarget (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# create input data
		steps = 50
		vec_time = np.linspace(0,20,steps);
		vec_velocity = np.linspace(5,5,steps)
		vec_range = np.linspace(100,1,steps)
		vec_velocity_real = [0]*steps
		vec_range_real = [0]*steps
		for k in range(steps):
			vec_velocity_real[k] = vec_velocity[k]
			vec_range_real[k] = vec_range[k]
		
		# random data on trajectory
		mu = 0
		sigma_vel = 0.5
		sigma_rge = 7
		for k in range(len(vec_velocity)):
			vec_velocity[k] = vec_velocity[k] + random.gauss(mu,sigma_vel)
			vec_range[k] = vec_range[k] + random.gauss(mu,sigma_rge)
		
		# set up pmts with zero points
		target_pmts = [0]*len(vec_velocity)
		#zero_points = (5,12,17)
		zero_points = ()
		for k in range(len(vec_velocity)):
			pmt_time = pmt.list2(pmt.string_to_symbol("rx_time"),pmt.make_tuple(pmt.from_long(int(vec_time[k])),pmt.from_double(vec_time[k]-int(vec_time[k]))))
			if k in zero_points:
				vec = [0]
				pmt_velocity = pmt.list2(pmt.string_to_symbol("velocity"),pmt.init_f32vector(1,vec))
				pmt_range = pmt.list2(pmt.string_to_symbol("range"),pmt.init_f32vector(1,vec))
			else:
				pmt_velocity = pmt.list2(pmt.string_to_symbol("velocity"),pmt.init_f32vector(1,(vec_velocity[k],)))
				pmt_range = pmt.list2(pmt.string_to_symbol("range"),pmt.init_f32vector(1,(vec_range[k],)))
			target_pmts[k] = pmt.list3(pmt_time,pmt_velocity,pmt_range)
		
		# set up fg
		test_duration = 1000 # ms, do not change!
		
		num_particle = 300
		std_range_meas = sigma_rge
		std_velocity_meas = sigma_vel
		std_accel_sys = 0.1
		threshold_track = 0.001
		threshold_lost = 4
		tracking_filter = "particle"
		
		# connect multiple strobes for different msgs
		src = [0]*len(target_pmts)
		for k in range(len(target_pmts)):
			src[k] = blocks.message_strobe(target_pmts[k], test_duration-400+400/len(target_pmts)*k)
		tracking = radar.tracking_singletarget(num_particle, std_range_meas, std_velocity_meas, std_accel_sys, threshold_track, threshold_lost, tracking_filter)
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
#		show_data = False # Toggle visibility of single messages # broken
		msg_num = snk.num_messages()
		vec_out_range = []
		vec_out_velocity = []
		for k in range(msg_num):
			msg_part = snk.get_message(k)
			tme = pmt.nth(0,msg_part) # not used
			vel = pmt.nth(1,msg_part)
			rgn = pmt.nth(2,msg_part)
			vec_out_range.append(pmt.f32vector_elements(pmt.nth(1,rgn))[0])
			vec_out_velocity.append(pmt.f32vector_elements(pmt.nth(1,vel))[0])
#			if show_data:
#				print "msg:", k
#				print pmt.symbol_to_string(pmt.nth(0,vel)), pmt.f32vector_elements(pmt.nth(1,vel))[0]
#				print pmt.symbol_to_string(pmt.nth(0,rgn)), pmt.f32vector_elements(pmt.nth(1,rgn))[0]
#				print 
#		print "RANGE:"
#		print vec_out_range
#		print "VELOCITY:"
#		print vec_out_velocity
		
		# make plots
		show_plots = True # Toggle visibility of plots
		if show_plots:
			time = range(len(vec_range))
			time_out = range(len(vec_out_range))
			plt.figure(1)
			
			plt.subplot(211)
			marker = '-o'
			p1 = plt.plot(time,vec_velocity_real,marker,time,vec_velocity,marker,time_out,vec_out_velocity,marker)
			plt.legend(p1,["IN velocity real", "IN velocity", "OUT velocity"])
			plt.title("VELOCITY")
			plt.xlabel('time')
			
			plt.subplot(212)
			marker = '-o'
			p1 = plt.plot(time,vec_range_real,marker,time,vec_range,marker,time_out,vec_out_range,marker)
			plt.legend(p1,["IN range real","IN range","OUT range"])
			plt.title("RANGE")
			plt.xlabel('time')
			
			plt.show()

if __name__ == '__main__':
	gr_unittest.run(qa_tracking_singletarget)#, "qa_tracking_singletarget.xml")
