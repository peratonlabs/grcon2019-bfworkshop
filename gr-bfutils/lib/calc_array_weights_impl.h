/* -*- c++ -*- */
/*
 * Copyright (c) 2019 Perspecta Labs Inc. All rights reserved
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_BFUTILS_CALC_ARRAY_WEIGHTS_IMPL_H
#define INCLUDED_BFUTILS_CALC_ARRAY_WEIGHTS_IMPL_H

#include <bfutils/calc_array_weights.h>


#include "arma_types.h"

using namespace pmt;
using namespace std;
using namespace arma;

namespace gr {
  namespace bfutils {

    class calc_array_weights_impl : public calc_array_weights
    {
     private:
      void handle_pdus(pmt_t msg);
      int d_num_elements;
      std::vector < gr_complex > d_modulated_preamble;
      cf_vec d_training_sequence;
      BfMode d_bf_mode;

     public:
      calc_array_weights_impl(int num_elements, const std::vector<gr_complex> &modulated_preamble, BfMode bf_mode );
      ~calc_array_weights_impl();
      
    };

  } // namespace bfutils
} // namespace gr

#endif /* INCLUDED_BFUTILS_CALC_ARRAY_WEIGHTS_IMPL_H */

