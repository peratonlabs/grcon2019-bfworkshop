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

#include <gnuradio/io_signature.h>
#include "pdu_pfb_clock_sync_impl.h"



#include <vector>
using namespace std;
using namespace pmt;

namespace gr {
  namespace bfutils {

    pdu_pfb_clock_sync::sptr
    pdu_pfb_clock_sync::make( double sps, float loop_bw,
                        const std::vector<float> &taps,
                        unsigned int filter_size, float init_phase,
                        float max_rate_deviation, int osps)
    {
      return gnuradio::get_initial_sptr
        (new pdu_pfb_clock_sync_impl( sps,  loop_bw,
                        taps,
                        filter_size, init_phase,
                        max_rate_deviation, osps));
    }


    /*
     * The private constructor
     */
    pdu_pfb_clock_sync_impl::pdu_pfb_clock_sync_impl( double sps, float loop_bw,
                        const std::vector<float> &taps,
                        unsigned int filter_size, float init_phase,
                        float max_rate_deviation, int osps)
      : gr::block("pdu_pfb_clock_sync",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {

      message_port_register_in(pmt::mp("pdu"));
      set_msg_handler(pmt::mp("pdu"),
                      boost::bind(&pdu_pfb_clock_sync_impl::handle_pdu, this, _1));

      message_port_register_out(pmt::mp("pdu"));


      pfbClockSync = new GrWrappedPfbClockSync(sps,  loop_bw,
                        taps,
                        filter_size, init_phase,
                        max_rate_deviation, osps);
    }

    /*
     * Our virtual destructor.
     */
    pdu_pfb_clock_sync_impl::~pdu_pfb_clock_sync_impl()
    {
      delete pfbClockSync;
    }



    void pdu_pfb_clock_sync_impl::handle_pdu(pmt_t msg) {

      pmt_t meta(pmt::car(msg));
      pmt_t data(pmt::cdr(msg));

      size_t nsamps_in = pmt::length(data);
      size_t len_bytes(0);

      printf("got pdu, %lu samples\n", nsamps_in);

      

      const gr_complex *samples = pmt::c32vector_elements(data, len_bytes);
      vector<gr_complex> out = pfbClockSync->process(samples, nsamps_in);


      pmt_t vecpmt(
          pmt::init_c32vector(out.size(), &out[0]));
      pmt_t pdu(pmt::cons(pmt::PMT_NIL, vecpmt));
      message_port_pub(mp("pdu"), pdu);

    }


  } /* namespace bfutils */
} /* namespace gr */

