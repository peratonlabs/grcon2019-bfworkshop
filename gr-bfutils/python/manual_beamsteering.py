#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright (c) 2019 Perspecta Labs Inc. All rights reserved
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


import numpy as np
from gnuradio import gr
from math import pi, sin
import pmt

class manual_beamsteering(gr.basic_block):
    """
    Calculate weights for a calibrated ULA in a given direction
    """
    def __init__(self, num_elements=4, sep_lambda=0.5, angle_degrees=0.0):
        gr.basic_block.__init__(self,
            name="manual_beamsteering",
            in_sig=None,
            out_sig=None)

        self.num_elements = num_elements
        self.sep_lambda = sep_lambda
        self.message_port_register_out(pmt.intern("weights"))
        self.set_angle_degrees(angle_degrees)

    def set_angle_degrees(self, angle_degrees):
        self.angle_degrees = angle_degrees
        self.theta = self.angle_degrees * pi / 180.0
        # Calculate the weights
        w = [0.0+0.0j]*self.num_elements
        m = 0
        for m in range(self.num_elements):
            w[m] = np.exp(((-2.0*pi*float(m)*self.sep_lambda*sin(self.theta)))*1.0j)

        vec = pmt.init_c32vector(len(w), w)
        sz = pmt.init_u32vector(2,[1,len(w)])
        self.message_port_pub(pmt.intern("weights"), pmt.cons(sz,vec))