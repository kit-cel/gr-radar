#
# Copyright 2008,2009 Free Software Foundation, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# The presence of this file turns this directory into a Python package

'''
This is the GNU Radio RADAR out-of-tree module.

The *gr-radar* project provides a toolbox of commonly used radar
algorithms. An important part is the *UHD Echotimer*, which enables a
synchronized TX and RX stream from USRPs to ensure a constant phase
relation in measurements. Example flowgraphs for CW, Dual CW, FSK,
FMCW and OFD M radar are given and partly tested on hardware. GUI
elements for target representation and further signal processing
algorithms such as target tracking are implemented. Check out the
project website for example videos and further information.

This project was initiated as a Google Summer of Code project and
developed at the *Communication Engineering Lab (CEL)* at the
*Karlsruhe Institute of Technology (KIT)*, Germany
'''
import os

# import pybind11 generated symbols into the radar namespace
try:
    # this might fail if the module is python-only
    from .bindings.radar_python import *
except ModuleNotFoundError:
    from .radar_python import *
    pass

# import any pure python here
#
