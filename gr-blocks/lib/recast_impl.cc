/* -*- c++ -*- */
/*
 * Copyright 2025 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "recast_impl.h"

#include "gnuradio/io_signature.h"
#include "gnuradio/sptr_magic.h"

namespace gr {
namespace blocks {

recast_impl::recast_impl(int input_size, int output_size)
    : block("Reinterpret Cast",
            io_signature::make(1, 1, input_size),
            io_signature::make(1, 1, output_size)),
      d_input_size(input_size),
      d_output_size(output_size)
{
}

recast_impl::~recast_impl() {};

void recast_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] =
        (noutput_items * d_output_size + (d_input_size - 1)) / d_input_size;
}

int recast_impl::general_work(int noutput_items,
                              gr_vector_int& ninput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    // Ensure we always consume entire input items
    if (noutput_items < d_input_size / d_output_size)
        return 0;

    auto in = reinterpret_cast<const uint8_t*>(input_items[0]);
    auto out = reinterpret_cast<uint8_t*>(output_items[0]);
    noutput_items =
        std::min(noutput_items, (ninput_items[0] * d_input_size) / d_output_size);

    memcpy(out, in, noutput_items * d_output_size);
    int n_consumed = (noutput_items * d_output_size) / d_input_size;
    consume_each(n_consumed);
    return noutput_items;
}

recast::sptr recast::make(int input_size, int output_size)
{
    return gnuradio::make_block_sptr<recast_impl>(input_size, output_size);
}

} // namespace blocks
} // namespace gr
