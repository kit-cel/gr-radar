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


#ifndef INCLUDED_RADAR_TRIGGER_COMMAND_H
#define INCLUDED_RADAR_TRIGGER_COMMAND_H

#include <gnuradio/block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block executes a command with the std::system() command if a value from a
 * f32vector with a given identifer (symbol) is in a given range. Each index of a vector
 * refers to a identifier. The execution of a command can be blocked for block_time
 * milliseconds after the last execution.
 *
 * \param command Command string
 * \param identifiers Identifiers (symbols) as vector of strings
 * \param vals_min Minimum values as f32vector
 * \param vals_max Maximum values as f32vector
 * \param block_time Block executing commands for block_time milliseconds
 *
 * \ingroup radar
 *
 */
class RADAR_API trigger_command : virtual public gr::block
{
public:
    typedef std::shared_ptr<trigger_command> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::trigger_command.
     *
     * To avoid accidental use of raw pointers, radar::trigger_command's
     * constructor is in a private implementation
     * class. radar::trigger_command::make is the public interface for
     * creating new instances.
     */
    static sptr make(std::string command,
                     std::vector<std::string> identifiers,
                     std::vector<float> vals_min,
                     std::vector<float> vals_max,
                     int block_time);
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_TRIGGER_COMMAND_H */
