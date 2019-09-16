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

#ifndef INCLUDED_BFUTILS_PREAMBLE_TO_PDU_IMPL_H
#define INCLUDED_BFUTILS_PREAMBLE_TO_PDU_IMPL_H

#include <bfutils/preamble_to_pdu.h>
#include <vector>
#include <string>

namespace gr {
  namespace bfutils {

    class preamble_to_pdu_impl : public preamble_to_pdu
    {
     private:
      int d_num_elements;
      std::string d_corr_tag;
      unsigned int d_preamble_size;
      std::vector < gr_complex > d_rx_preamble;

      uint64_t mute_until_offset;
      void publish();

     public:
      preamble_to_pdu_impl(int num_elements, 
        const unsigned int data_length, 
        const std::string &corr_tag  );
      ~preamble_to_pdu_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace bfutils
} // namespace gr

#endif /* INCLUDED_BFUTILS_PREAMBLE_TO_PDU_IMPL_H */

