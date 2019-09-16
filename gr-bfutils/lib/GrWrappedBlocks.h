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

#include <gnuradio/block_detail.h>
#include <gnuradio/buffer.h>
#include <gnuradio/digital/clock_recovery_mm_cc.h>
#include <gnuradio/digital/cma_equalizer_cc.h>
#include <gnuradio/digital/pfb_clock_sync_ccf.h>

#include <vector>

using namespace std;

#ifndef GR_WRAPPED_BLOCKS_H_
#define GR_WRAPPED_BLOCKS_H_

#include "GrWrapper.h"

namespace gr {
namespace bfutils {

class GrWrappedPfbClockSync : public gr::digital::pfb_clock_sync_ccf::sptr,
                              public GrWrapper {
private:
  int d_num_taps;
  int d_sps;
  std::vector<float> d_taps;
  unsigned int d_filter_size;


public:
  GrWrappedPfbClockSync(double sps, float loop_bw,
                        const std::vector<float> &taps,
                        unsigned int filter_size, float init_phase,
                        float max_rate_deviation, int osps)
      : gr::digital::pfb_clock_sync_ccf::sptr(
            gr::digital::pfb_clock_sync_ccf::make( sps, loop_bw,
                        taps, filter_size,  init_phase,
                        max_rate_deviation,  osps)),
        GrWrapper(1, 1) {
    d_sps = sps;
    d_filter_size = filter_size;
    d_taps = taps;
    // d_num_taps = num_taps; // input buffer needs to be offset by num_taps

    init_wrapper(*this);
  }

  vector<gr_complex> process(const gr_complex *samples_in, int nsamps_in) {
    vector<int> ninput_items;
    ninput_items.push_back(nsamps_in);

    // because the pfb clock sync deals in history 
    // set_history(d_taps_per_filter + d_sps + d_sps);
    int taps_per_filter = (unsigned int)ceil((double)d_taps.size() / (double)d_filter_size);
    int history_offset = taps_per_filter + d_sps + d_sps;
    vector<gr_complex> samples_in_padded(history_offset + nsamps_in - 1);
    memcpy(&samples_in_padded[history_offset - 1], samples_in,
           nsamps_in * sizeof(gr_complex));
    gr_vector_const_void_star input_items;
    input_items.push_back((const void *)&samples_in_padded[0]);
    vector<gr_complex> output(nsamps_in);
    gr_vector_void_star output_items;
    output_items.push_back((void *)&output[0]);
    int actual_nout = block->general_work(nsamps_in / d_sps, ninput_items,
                                          input_items, output_items);
    output.resize(actual_nout);

    // //

    return output;
  }
};

class GrWrappedClockRecoveryMM : public gr::digital::clock_recovery_mm_cc::sptr,
                                 public GrWrapper {
private:
public:
  GrWrappedClockRecoveryMM(float omega, float gain_omega, float mu,
                           float gain_mu, float omega_relative_limit)
      : gr::digital::clock_recovery_mm_cc::sptr(
            gr::digital::clock_recovery_mm_cc::make(
                omega, gain_omega, mu, gain_mu, omega_relative_limit)),
        GrWrapper(1, 1) {
    init_wrapper(*this);
  }

  vector<gr_complex> process(const gr_complex *samples_in, int nsamps_in) {
    vector<int> ninput_items;
    ninput_items.push_back(nsamps_in);
    gr_vector_const_void_star input_items;
    input_items.push_back((const void *)samples_in);
    vector<gr_complex> output(nsamps_in);
    gr_vector_void_star output_items;
    output_items.push_back((void *)&output[0]);
    int actual_nout =
        block->general_work(nsamps_in, ninput_items, input_items, output_items);
    output.resize(actual_nout);

    // //

    return output;
  }
};

class GrWrappedCmaEqualizer : public gr::digital::cma_equalizer_cc::sptr,
                              public GrWrapper {
private:
  int d_num_taps;
  int d_sps;

public:
  GrWrappedCmaEqualizer(int num_taps, float modulus, float mu, int sps)
      : gr::digital::cma_equalizer_cc::sptr(
            gr::digital::cma_equalizer_cc::make(num_taps, modulus, mu, sps)),
        GrWrapper(1, 1) {
    d_sps = sps;
    d_num_taps = num_taps; // input buffer needs to be offset by num_taps

    init_wrapper(*this);
  }

  vector<gr_complex> process(const gr_complex *samples_in, int nsamps_in) {
    vector<int> ninput_items;
    ninput_items.push_back(nsamps_in);

    vector<gr_complex> samples_in_padded(d_num_taps + nsamps_in - 1);
    memcpy(&samples_in_padded[d_num_taps - 1], samples_in,
           nsamps_in * sizeof(gr_complex));
    gr_vector_const_void_star input_items;
    input_items.push_back((const void *)&samples_in_padded[0]);
    vector<gr_complex> output(nsamps_in);
    gr_vector_void_star output_items;
    output_items.push_back((void *)&output[0]);
    int actual_nout = block->general_work(nsamps_in / d_sps, ninput_items,
                                          input_items, output_items);
    output.resize(actual_nout);

    // //

    return output;
  }
};
} // namespace bfutils

} // namespace gr

#endif