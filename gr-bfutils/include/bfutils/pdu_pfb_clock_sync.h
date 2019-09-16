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

#ifndef INCLUDED_BFUTILS_PDU_PFB_CLOCK_SYNC_H
#define INCLUDED_BFUTILS_PDU_PFB_CLOCK_SYNC_H

#include <bfutils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace bfutils {

    /*!
     * \brief <+description of block+>
     * \ingroup bfutils
     *
     */
    class BFUTILS_API pdu_pfb_clock_sync : virtual public gr::block
    {
     public:

      
      typedef boost::shared_ptr<pdu_pfb_clock_sync> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of bfutils::pdu_pfb_clock_sync.
       *
       * To avoid accidental use of raw pointers, bfutils::pdu_pfb_clock_sync's
       * constructor is in a private implementation
       * class. bfutils::pdu_pfb_clock_sync::make is the public interface for
       * creating new instances.
       */
      static sptr make(double sps, float loop_bw,
                        const std::vector<float> &taps,
                        unsigned int filter_size, float init_phase,
                        float max_rate_deviation, int osps );
    };

  } // namespace bfutils
} // namespace gr

#endif /* INCLUDED_BFUTILS_PDU_PFB_CLOCK_SYNC_H */

