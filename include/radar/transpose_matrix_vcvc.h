/* -*- c++ -*- */
/*
 * Copyright 2014 Communications Engineering Lab, KIT.
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


#ifndef INCLUDED_RADAR_TRANSPOSE_MATRIX_VCVC_H
#define INCLUDED_RADAR_TRANSPOSE_MATRIX_VCVC_H

#include <gnuradio/tagged_stream_block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block transposes a matrix. A tagged stream combined with vectors as items
 * represent a matrix. vlen_in is the vector length of the input data and vlen_out the
 * vector length of the output data. vlen_out is equal to the items (vectors) per tagged
 * stream on the input stream.
 *
 * \param vlen_in Vector length input
 * \param vlen_out Vector length output
 * \param len_key Packet length key for tagged stream
 *
 * \ingroup radar
 *
 */
class RADAR_API transpose_matrix_vcvc : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<transpose_matrix_vcvc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::transpose_matrix_vcvc.
     *
     * To avoid accidental use of raw pointers, radar::transpose_matrix_vcvc's
     * constructor is in a private implementation
     * class. radar::transpose_matrix_vcvc::make is the public interface for
     * creating new instances.
     */
    static sptr make(int vlen_in, int vlen_out, std::string len_key = "packet_len");
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TRANSPOSE_MATRIX_VCVC_H */
