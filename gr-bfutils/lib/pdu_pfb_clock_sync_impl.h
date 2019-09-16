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

#ifndef INCLUDED_BFUTILS_PDU_PFB_CLOCK_SYNC_IMPL_H
#define INCLUDED_BFUTILS_PDU_PFB_CLOCK_SYNC_IMPL_H

#include <bfutils/pdu_pfb_clock_sync.h>
#include <vector>

#include "GrWrappedBlocks.h"

namespace gr {
  namespace bfutils {

    class pdu_pfb_clock_sync_impl : public pdu_pfb_clock_sync
    {
     private:
      // Nothing to declare in this block.
      GrWrappedPfbClockSync *pfbClockSync;

     public:
      pdu_pfb_clock_sync_impl(double sps, float loop_bw,
                        const std::vector<float> &taps,
                        unsigned int filter_size, float init_phase,
                        float max_rate_deviation, int osps );
      ~pdu_pfb_clock_sync_impl();

      void handle_pdu(pmt::pmt_t msg);
    };

  } // namespace bfutils
} // namespace gr

#endif /* INCLUDED_BFUTILS_PDU_PFB_CLOCK_SYNC_IMPL_H */

