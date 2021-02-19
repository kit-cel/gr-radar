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

#ifndef INCLUDED_RADAR_TRIGGER_COMMAND_IMPL_H
#define INCLUDED_RADAR_TRIGGER_COMMAND_IMPL_H

#include "boost/date_time/posix_time/posix_time.hpp"
#include <radar/trigger_command.h>

namespace gr {
namespace radar {

class trigger_command_impl : public trigger_command
{
private:
    // Nothing to declare in this block.

public:
    trigger_command_impl(std::string command,
                         std::vector<std::string> identifiers,
                         std::vector<float> vals_min,
                         std::vector<float> vals_max,
                         int block_time);
    ~trigger_command_impl();
    void handle_msg(pmt::pmt_t msg);

    std::string d_command;
    std::vector<std::string> d_identifiers;
    std::vector<float> d_vals_min, d_vals_max;
    int d_block_time;

    boost::posix_time::ptime d_last_time, d_actual_time;
    boost::posix_time::time_duration d_time_duration;

    pmt::pmt_t d_port_id_in;
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TRIGGER_COMMAND_IMPL_H */
