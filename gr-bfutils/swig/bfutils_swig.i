/* -*- c++ -*- */

#define BFUTILS_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "bfutils_swig_doc.i"

%{
#include "bfutils/preamble_to_pdu.h"
#include "bfutils/calc_array_weights.h"
#include "bfutils/pdu_pfb_clock_sync.h"
#include "bfutils/interference_nulling.h"
%}

%include "bfutils/preamble_to_pdu.h"
GR_SWIG_BLOCK_MAGIC2(bfutils, preamble_to_pdu);
%include "bfutils/calc_array_weights.h"
GR_SWIG_BLOCK_MAGIC2(bfutils, calc_array_weights);
%include "bfutils/pdu_pfb_clock_sync.h"
GR_SWIG_BLOCK_MAGIC2(bfutils, pdu_pfb_clock_sync);
%include "bfutils/interference_nulling.h"
GR_SWIG_BLOCK_MAGIC2(bfutils, interference_nulling);
