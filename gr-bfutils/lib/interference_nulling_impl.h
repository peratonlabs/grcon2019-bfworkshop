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

#ifndef INCLUDED_BFUTILS_INTERFERENCE_NULLING_IMPL_H
#define INCLUDED_BFUTILS_INTERFERENCE_NULLING_IMPL_H

#include <armadillo>
#include <bfutils/interference_nulling.h>

#include "arma_types.h"

using namespace pmt;
using namespace std;
using namespace arma;

namespace gr {
namespace bfutils {

class interference_nulling_impl : public interference_nulling {
private:
  unsigned int d_nulling_dims;
  unsigned int d_num_elements;

  void handle_pdus(pmt_t msg);

public:
  interference_nulling_impl(unsigned int num_elements,
                            unsigned int nulling_dims);
  ~interference_nulling_impl();

};

} // namespace bfutils
} // namespace gr

#endif /* INCLUDED_BFUTILS_INTERFERENCE_NULLING_IMPL_H */
