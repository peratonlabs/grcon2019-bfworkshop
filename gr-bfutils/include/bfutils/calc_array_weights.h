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

#ifndef INCLUDED_BFUTILS_CALC_ARRAY_WEIGHTS_H
#define INCLUDED_BFUTILS_CALC_ARRAY_WEIGHTS_H

#include <bfutils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace bfutils {

    enum BfMode
    {
      MMSE = 0,
      INTF_NULLING
    };

    /*!
     * \brief <+description of block+>
     * \ingroup bfutils
     *
     */
    class BFUTILS_API calc_array_weights : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<calc_array_weights> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of bfutils::calc_array_weights.
       *
       * To avoid accidental use of raw pointers, bfutils::calc_array_weights's
       * constructor is in a private implementation
       * class. bfutils::calc_array_weights::make is the public interface for
       * creating new instances.
       */
      static sptr make( int num_elements, const std::vector<gr_complex> &modulated_preamble, BfMode bf_mode );
    };

  } // namespace bfutils
} // namespace gr

#endif /* INCLUDED_BFUTILS_CALC_ARRAY_WEIGHTS_H */

