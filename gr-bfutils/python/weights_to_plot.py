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
import numpy

class weights_to_plot(gr.basic_block):
    """
    docstring for block weights_to_plot
    """
    def __init__(self, num_elements, sep_lambda, log_response ):
        gr.basic_block.__init__(self,
            name="weights_to_plot",
            in_sig=None,
            out_sig=None)
        self.message_port_register_in(pmt.intern("weights"))
        self.set_msg_handler(pmt.intern("weights"), self.handle_weights)
        self.message_port_register_out(pmt.intern("plot"))

        self.num_elements = num_elements

        self.set_sep_lambda(sep_lambda)

        self.set_log_response(log_response)
        self.plot_depth=70.0


    def set_sep_lambda(self, sep_lambda):
        self.sep_lambda = sep_lambda

    def set_log_response(self, log_response):
        self.log_response = log_response

    def handle_weights(self, msg):
        # weights = np.conj(pmt.c32vector_elements(msg))

        sizevec = pmt.u32vector_elements(pmt.car(msg))
        # print(sizevec)
        weights = pmt.c32vector_elements(pmt.cdr(msg))
        weights = numpy.reshape(weights,sizevec,order='C')
        # print(weights.shape)

        angles = np.array(np.linspace(-180.0,180.0,3601), dtype=np.float32    )
        wr = self.weight_response(weights[0], angles, self.sep_lambda)
        # TODO: make the polar plot do multiple weight vectors
        # print(weights)
        self.message_port_pub(pmt.intern("plot"), pmt.to_pmt({"r":wr,"theta":angles}))

    def weight_response(self,weights,angles,lam):
        M = len(weights)
        theta = np.pi * angles/180.0
        sum_array = np.zeros(len(theta))
        for m in range(M):
            a = np.exp(-2.0*np.pi*m*lam*np.sin(theta)*1j)
            sum_array = sum_array + a*weights[m]
        
        wr = np.abs(sum_array)       
        wr = wr / np.max(wr)
        
        if self.log_response:
            wr = 20.0 * np.log10(wr)
            wr = 1 + wr / self.plot_depth
            wr = np.clip(wr,0.0,1.0)
        
        return wr
