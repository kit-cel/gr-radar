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

    def add_length_tag()

    def validate_tuples(self, expected, result):
        self.assertEqual(len(expected), len(result))
        for i in range(len(expected)):
            self.assertAlmostEqual(expected[i], result[i])

    def test_001_delay(self):
        packet_len = 3
        src1_data = (0, 1, 0)  # reference signal
        src2_data = (0, 0, 1)  # echo signal
        src1 = blocks.vector_source_c(src1_data, False, 1)
        src2 = blocks.vector_source_c(src2_data, False, 1)
        s2ts = blocks.stream_to_tagged_stream(8, 1, packet_len, 'packet_len')
        op = radar.cross_correlate_vcvc(1)
        exp_data = (0, 1, 0)  # delay of one sample
        dst = blocks.vector_sink_c(1)

        # set up fg
        self.tb.connect(src1, (op, 0))
        self.tb.connect(src2, (op, 1))
        self.tb.connect(op, dst)
        self.tb.run()
        # check data
        result_data = dst.data()
        self.validate_tuples(exp_data, result_data)

    def test_002_equal(self):
        src1_data = (1, 1, 0, 1)  # reference signal
        src2_data = (1, 1, 0, 1)  # echo signal
        src1 = blocks.vector_source_c(src1_data, False, 1)
        src2 = blocks.vector_source_c(src2_data, False, 1)
        op = radar.cross_correlate_vcvc(1)
        exp_data = (3, 2, 2, 2)  # no delay
        dst = blocks.vector_sink_c(1)

        # set up fg
        self.tb.connect(src1, (op, 0))
        self.tb.connect(src2, (op, 1))
        self.tb.connect(op, dst)
        self.tb.run()
        # check data
        result_data = dst.data()
        self.validate_tuples(exp_data, result_data)

    def test_003_inverse_delay(self):
        src1_data = (0, 0, 1)  # reference signal
        src2_data = (0, 1, 0)  # echo signal
        src1 = blocks.vector_source_c(src1_data, False, 1)
        src2 = blocks.vector_source_c(src2_data, False, 1)
        op = radar.cross_correlate_vcvc(1)
        exp_data = (0, 0, 1)  # inverted delay of one sample
        dst = blocks.vector_sink_c(1)

        # set up fg
        self.tb.connect(src1, (op, 0))
        self.tb.connect(src2, (op, 1))
        self.tb.connect(op, dst)
        self.tb.run()
        # check data
        result_data = dst.data()
        self.validate_tuples(exp_data, result_data)

    def test_004_multiple_vectors(self):
        src1_data = (1, 0, 1, 0, 1, 0)  # reference signal
        src2_data = (0, 1, 0, 1, 0, 1)  # echo signal
        src1 = blocks.vector_source_c(src1_data, False, 1)
        src2 = blocks.vector_source_c(src2_data, False, 1)
        op = radar.cross_correlate_vcvc(1)
        exp_data = (0, 1, 0, 1, 0, 1)  # inverted delay of one for each vector
        dst = blocks.vector_sink_c(1)

        # set up fg
        self.tb.connect(src1, (op, 0))
        self.tb.connect(src2, (op, 1))
        self.tb.connect(op, dst)
        #raw_input ('Press Enter to continue: ') #for attaching debugger
        self.tb.run()
        # check data
        result_data = dst.data()
        self.validate_tuples(exp_data, result_data)



if __name__ == '__main__':
    gr_unittest.run(qa_cross_correlate_vcvc, "qa_cross_correlate_vcvc.xml")
