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

#include "preamble_to_pdu_impl.h"
#include <gnuradio/io_signature.h>
#include <stdio.h>

using namespace pmt;
using namespace std;

namespace gr {
namespace bfutils {

preamble_to_pdu::sptr
preamble_to_pdu::make(int num_elements,
                      const unsigned int data_length,
                      const std::string &corr_tag) {
  return gnuradio::get_initial_sptr(
      new preamble_to_pdu_impl(num_elements, data_length, corr_tag));
}

/*
 * The private constructor
 */
preamble_to_pdu_impl::preamble_to_pdu_impl(
    int num_elements, const unsigned int data_length,
    const std::string &corr_tag)
    : gr::sync_block(
          "preamble_to_pdu",
          gr::io_signature::make(1, 1, num_elements * sizeof(gr_complex)),
          gr::io_signature::make(0, 0, 0)),
      d_num_elements(num_elements),
      d_corr_tag(corr_tag) {

  d_preamble_size = data_length;
  mute_until_offset = 0;

  // printf("d_preamble.size() = %lu\n\n", d_preamble_size);

  message_port_register_out(pmt::mp("pdus"));
}

/*
 * Our virtual destructor.
 */
preamble_to_pdu_impl::~preamble_to_pdu_impl() {}

void preamble_to_pdu_impl::publish() {
  // send the vector
  pmt_t vecpmt(pmt::init_c32vector(d_rx_preamble.size(), &d_rx_preamble[0]));
  pmt_t pdu(pmt::cons(pmt::PMT_NIL, vecpmt));
  message_port_pub(mp("pdus"), pdu);

  d_rx_preamble.resize(0);
}

int preamble_to_pdu_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items) {
  const gr_complex *in = (const gr_complex *)input_items[0];

  vector<tag_t> tags;
  get_tags_in_window(tags, 0, 0, noutput_items, pmt::intern(d_corr_tag));

  unsigned long int nread = nitems_read(0);

  // If we have a partially received preamble, add to it
  if (d_rx_preamble.size() > 0 &&
      d_rx_preamble.size() < d_preamble_size * d_num_elements) {
    int samples_stored_already = d_rx_preamble.size() / d_num_elements;
    int samples_remaining = d_preamble_size - samples_stored_already;
    samples_remaining =
        (samples_remaining <= noutput_items) ? samples_remaining : noutput_items;
    d_rx_preamble.resize(d_rx_preamble.size() + samples_remaining*d_num_elements);
    std::copy(&in[0], &in[samples_remaining * d_num_elements],
              &d_rx_preamble[d_num_elements * samples_stored_already]);
  }

  if (d_rx_preamble.size() >= d_preamble_size * d_num_elements)
    publish();


  for (unsigned t = 0; t < tags.size(); t++) {
    uint64_t offset = tags[t].offset - nread;

    if (tags[t].offset < mute_until_offset)
      continue;

    mute_until_offset = tags[t].offset + d_preamble_size;

    int samples_in_window = noutput_items - offset;
    if (samples_in_window >= d_preamble_size)
      d_rx_preamble.assign(in + offset*d_num_elements,
                           in + offset*d_num_elements + d_num_elements * d_preamble_size);
    else {
      d_rx_preamble.assign(in + offset*d_num_elements,
                           in + offset*d_num_elements + d_num_elements * samples_in_window);
    }

    if (d_rx_preamble.size() >= d_preamble_size * d_num_elements)
      publish();
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} /* namespace bfutils */
} /* namespace gr */
