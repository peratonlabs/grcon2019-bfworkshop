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
from gnuradio.gr import pmt
import pyqtgraph as pg
from pyqtgraph import PlotWidget
from pyqtgraph.Qt import QtGui, QtCore
# from PyQt5 import QtCore
import math

class polar_plot(gr.sync_block, PlotWidget):
    """
    docstring for block test_gui
    """
    sigUpdateData = QtCore.Signal(object, object)
    def __init__(self, autoscale, log_plot, plot_depth_db, *args):
        gr.sync_block.__init__(self,
            name="polar_plot",
            in_sig=None,
            out_sig=None)
        PlotWidget.__init__(self,  enableMenu=False, axisItems={}, *args)

        self.curve = self.plot([], pen=(0,200,100), width=2.0, name="Polar Response")

        self.circles = []
        
        self.autoscale = autoscale
        self.log_plot = log_plot
        self.plot_depth=plot_depth_db
        angles = np.array(np.linspace(-180.0,180.0,361),dtype=np.float32)
        for kk in range(0,int(10*math.floor(self.plot_depth/10.0)),10):
            mag = 1 - float(kk)/self.plot_depth
            r = mag * np.ones(len(angles))
            x = self.pol_to_car_x(r,angles)
            y = self.pol_to_car_y(r,angles)
            self.circles.append(self.plot(x,y,pen=pg.mkPen((128,128,128),style=QtCore.Qt.DashLine)))


        self.message_port_register_in(pmt.intern("plot"))
        self.set_msg_handler(pmt.intern("plot"), self.handler)
    
        self.sigUpdateData.connect(self.updateGraph)

    # Polar to Cartesian
    def pol_to_car_x(self,r,theta):
        x = r * np.cos(theta * np.pi / 180.0)
        return x
        
    def pol_to_car_y(self,r,theta):    
        y = r * np.sin(theta * np.pi / 180.0)
        return y

    def db_to_lin(x):
        return np.power(10,x/20.0)

    def lin_to_db(x):
        return 20*np.log10(x)

    def updateGraph(self, theta, r):
        
        y = self.pol_to_car_y(r, theta)
        x = self.pol_to_car_x(r,theta)

        self.curve.setData(y,x)

    def handler(self, msg):
        # get input
        #x = float(pmt.to_python(msg))
        #print pmt.to_python(pmt.any_ref(msg))
        #print dir(multiaperture)
        # theta = np.array(np.linspace(-90.0,90.0,1801), dtype=np.float32)
        # r = np.random.uniform(0.0, 1.0, size=(1801,))

        
        d = pmt.to_python(msg)
        r = d['r']
        theta = d['theta']

        # self.updateGraph(theta, r)
        self.sigUpdateData.emit(theta,r)
        