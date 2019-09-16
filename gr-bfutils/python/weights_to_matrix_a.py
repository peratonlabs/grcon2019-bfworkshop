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


import numpy
from gnuradio import gr
import numpy as np

import pmt

class weights_to_matrix_a(gr.sync_block):
    """
    Converts calculated weights to Matrix A format expected by Multiply Matrix block
    """

    def __init__(self):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.basic_block.__init__(
            self,
            name='weights_to_matrix_a',   # will show up in GRC
            in_sig=None,
            out_sig=None
        )

        self.message_port_register_in(pmt.intern("weights"))
        self.set_msg_handler(pmt.intern("weights"), self.handler)

        self.message_port_register_out(pmt.intern("A"))

    def handler(self, msg):
        # Check if msg is c32_vector
        if not pmt.is_pair(msg):
            print("Not expected c32 vector type")

        sizevec = pmt.u32vector_elements(pmt.car(msg))
        # print(sizevec)
        weights = pmt.c32vector_elements(pmt.cdr(msg))
        weights = numpy.reshape(weights,sizevec,order='C')
        # print(weights.shape)
        # print(weights)
        # print(weights.tolist())
        # print(weights[0])

        self.message_port_pub(pmt.intern("A"), pmt.to_pmt(weights.tolist()))

        # self.message_port_pub(pmt.intern("A"), pmt.to_pmt([[x.conjugate() for x in weights] ]))

        # message_port_pub(pdu_port_id, msg);
