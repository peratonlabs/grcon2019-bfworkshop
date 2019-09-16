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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "calc_array_weights_impl.h"
#include <gnuradio/io_signature.h>

#include <iostream>
#include <fstream>
#include <stdio.h>

namespace gr {
namespace bfutils {

calc_array_weights::sptr
calc_array_weights::make(int num_elements,
                         const std::vector<gr_complex> &modulated_preamble, 
                         BfMode bf_mode) {
  return gnuradio::get_initial_sptr(
      new calc_array_weights_impl(num_elements, modulated_preamble, bf_mode));
}

/*
 * The private constructor
 */
calc_array_weights_impl::calc_array_weights_impl(
    int num_elements, const std::vector<gr_complex> &modulated_preamble, BfMode bf_mode)
    : gr::block("calc_array_weights", gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_num_elements(num_elements), d_modulated_preamble(modulated_preamble),
      d_bf_mode(bf_mode) {


  d_training_sequence.set_size(d_modulated_preamble.size());
  memcpy(d_training_sequence.memptr(),&d_modulated_preamble[0], sizeof(gr_complex)*d_modulated_preamble.size());

  message_port_register_in(pmt::mp("pdus"));
  set_msg_handler(pmt::mp("pdus"),
                  [this](pmt::pmt_t msg) { this->handle_pdus(msg); });

  message_port_register_out(pmt::mp("weights"));
}

/*
 * Our virtual destructor.
 */
calc_array_weights_impl::~calc_array_weights_impl() {}

void calc_array_weights_impl::handle_pdus(pmt_t msg) {
  // unpack the PDU
  pmt_t meta(pmt::car(msg));
  pmt_t data(pmt::cdr(msg));

  size_t nsamps_in = pmt::length(data);
  size_t num_samples(0);

  const gr_complex *samples_intlv = pmt::c32vector_elements(data, num_samples);

  num_samples /= d_num_elements;

  // deinterleave the vector data
  cf_mat X(d_num_elements, num_samples);
  for (int i=0; i<d_num_elements; i++)
  {
    for (int j=0; j<num_samples; j++)
    {
      X(i,j) = samples_intlv[j*d_num_elements+i];
    }
  }

  // calculate the weights

  cf_mat R = X*X.t();
  // Add diagonal so low rank scenario is not singular
  R += cf_mat(R.n_rows, R.n_cols,fill::eye)*1.0e-2;
  cf_mat p = X*conj(d_training_sequence);

  cf_mat w;
  if (d_bf_mode == MMSE)
  {
     w = solve(R,p);
     w = conj(w);
  }
  // Add more bf algorithms later

  // // publish the weights
  pmt_t vecpmt(pmt::init_c32vector(w.n_rows*w.n_cols, w.memptr()));
  vector < unsigned int > sizevec;
  sizevec.push_back(w.n_cols);
  sizevec.push_back(w.n_rows);
  pmt_t sizepmt(pmt::init_u32vector(2,sizevec));
  pmt_t pdu(pmt::cons(sizepmt, vecpmt));
  message_port_pub(mp("weights"), pdu);
}

} /* namespace bfutils */
} /* namespace gr */
