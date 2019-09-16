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

#ifndef GR_WRAPPER_H_
#define GR_WRAPPER_H_

namespace gr{
namespace bfutils{

class GrWrapper
{
    private:
        int d_num_inputs;
        int d_num_outputs;

    public:
    block_sptr block;
    GrWrapper(int num_inputs, int num_outputs)
    {
        d_num_inputs = num_inputs;
        d_num_outputs = num_outputs;
    }

    void init_wrapper(block_sptr block_to_wrap )
    {
        block = block_to_wrap;
        block_detail_sptr detail = make_block_detail(d_num_inputs,d_num_outputs);
        // input/output buffers just to trick the cleanup bookkeeping that gets called by the consume()
        buffer_sptr input_buffer = make_buffer(1, sizeof(gr_complex),block);
        buffer_sptr output_buffer = make_buffer(1, sizeof(gr_complex),block);
        for (int i=0; i<d_num_inputs; i++)
            detail->set_input(i, buffer_add_reader(input_buffer, 0, block, 0));
        for (int i=0; i<d_num_outputs; i++)
        detail->set_output(i, output_buffer);
        block->set_detail(detail);
    }


};

}
}

#endif