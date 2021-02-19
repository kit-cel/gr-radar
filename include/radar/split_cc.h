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

#ifndef INCLUDED_RADAR_SPLIT_CC_H
#define INCLUDED_RADAR_SPLIT_CC_H

#include <gnuradio/tagged_stream_block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block splits a tagged stream into segments. As packet_parts you give the
 * structure of the packet, e.g. (10, 20, 5). With the packet number you can choose which
 * packet shall be pushed to output. Counting begins on zero. E.g. packet_num=1 returns 20
 * items.
 *
 * \param packet_num Number of packet to push to output
 * \param packet_parts Packet structure as vector of packet length
 * \param len_key Packet length key for tagged stream
 *
 * \ingroup radar
 *
 */
class RADAR_API split_cc : virtual public gr::tagged_stream_block
{
public:
    typedef boost::shared_ptr<split_cc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::split_cc.
     *
     * To avoid accidental use of raw pointers, radar::split_cc's
     * constructor is in a private implementation
     * class. radar::split_cc::make is the public interface for
     * creating new instances.
     */
    static sptr make(int packet_num,
                     const std::vector<int> packet_parts,
                     const std::string& len_key = "packet_len");
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_SPLIT_CC_H */
