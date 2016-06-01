#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2016 Anton Debner.
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


class qa_cross_correlate_vcvc(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def execute_block(self, src1_data, src2_data, packet_len):
        src1_data += src1_data + (0,)  # Add an extra element
        src2_data += src2_data + (0,)  # Add an extra element
        src1 = blocks.vector_source_c(src1_data, False, 1)
        src2 = blocks.vector_source_c(src2_data, False, 1)
        s2ts1 = blocks.stream_to_tagged_stream(8, 1, packet_len, 'packet_len')
        s2ts2 = blocks.stream_to_tagged_stream(8, 1, packet_len, 'packet_len')
        cross_corr = radar.cross_correlate_vcvc(packet_len, 'packet_len')
        sink = blocks.vector_sink_c()
        # set up fg
        self.tb.connect(src1, s2ts1)
        self.tb.connect(s2ts1, (cross_corr, 0))
        self.tb.connect(src2, s2ts2)
        self.tb.connect(s2ts2, (cross_corr, 1))
        self.tb.connect(cross_corr, sink)
        self.tb.run()
        result = sink.data()
        return result[0:packet_len]  # Leave out the extra element

    def validate_tuples(self, expected, result):
        #  print(expected)
        #  print(result)
        self.assertEqual(len(expected), len(result))
        for i in range(len(expected)):
            self.assertAlmostEqual(expected[i], result[i])

    def test_001_delay(self):
        packet_len = 3
        src1_data = (0, 1, 0)  # reference signal
        src2_data = (0, 0, 1)  # echo signal

        # get data
        result_data = self.execute_block(src1_data, src2_data, packet_len)

        # check data
        exp_data = (0, 1, 0)  # delay of one sample
        self.validate_tuples(exp_data, result_data)

    def test_002_equal(self):
        packet_len = 4
        src1_data = (1, 1, 0, 1)  # reference signal
        src2_data = (1, 1, 0, 1)  # echo signal
        result_data = self.execute_block(src1_data, src2_data, packet_len)

        # check data
        exp_data = (3, 2, 2, 2)  # no delay
        self.validate_tuples(exp_data, result_data)

    def test_003_inverse_delay(self):
        packet_len = 3
        src1_data = (0, 0, 1)  # reference signal
        src2_data = (0, 1, 0)  # echo signal
        result_data = self.execute_block(src1_data, src2_data, packet_len)

        # check data
        exp_data = (0, 0, 1)  # inverted delay of one sample
        self.validate_tuples(exp_data, result_data)

'''
    def test_004_multiple_vectors(self):
        packet_len = 2
        src1_data = (1, 0, 1, 0, 1, 0)  # reference signal
        src2_data = (0, 1, 0, 1, 0, 1)  # echo signal
        result_data = self.execute_block(src1_data, src2_data, packet_len)
        exp_data = (0, 1, 0, 1, 0, 1)  # inverted delay of one for each vector
        self.validate_tuples(exp_data, result_data)
'''


if __name__ == '__main__':
    gr_unittest.run(qa_cross_correlate_vcvc, "qa_cross_correlate_vcvc.xml")
