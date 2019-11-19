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


#ifndef INCLUDED_RADAR_FIND_MAX_PEAK_C_H
#define INCLUDED_RADAR_FIND_MAX_PEAK_C_H

#include <gnuradio/tagged_stream_block.h>
#include <radar/api.h>

namespace gr {
namespace radar {

/*!
 * \brief This block estimates a single peak of a given FFT spectrum as tagged stream.
 * Output is a message with the information of frequency, phase and power of the peak as a
 * f32vector with a single item. All data is tagged with the identifiers (symbols)
 * 'frequency', 'phase' and 'power'. The peak is estimated on the whole spectrum or on the
 * range max_freq if cut_max_freq is true. Furthermore a threshold of the spectrum
 * amplitude can be given with threshold. The DC peak can be cut out with the protected
 * samples samp_protect. This value do not evaluate samp_protect samples around the DC
 * peak. If no suitable peak is found the block returns empty vectors with the
 * identifiers.
 *
 * \param samp_rate Sample rate
 * \param threshold Threshold for detection of the spectrum amplitude
 * \param samp_protect Protected samples for cutting DC peak
 * \param max_freq Frequency range for cutting spectrum
 * \param cut_max_freq Toggle cutting the spectrum for peak estimation
 * \param len_key Packet length key for tagged stream
 *
 * \ingroup radar
 *
 */
class RADAR_API find_max_peak_c : virtual public gr::tagged_stream_block
{
public:
    typedef boost::shared_ptr<find_max_peak_c> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of radar::find_max_peak_c.
     *
     * To avoid accidental use of raw pointers, radar::find_max_peak_c's
     * constructor is in a private implementation
     * class. radar::find_max_peak_c::make is the public interface for
     * creating new instances.
     */
    static sptr make(int samp_rate,
                     float threshold,
                     int samp_protect,
                     std::vector<float> max_freq,
                     bool cut_max_freq,
                     const std::string& len_key = "packet_len");
    virtual void set_threshold(float threshold) = 0;
    virtual void set_samp_protect(int samp) = 0;
    virtual void set_max_freq(std::vector<float> freq) = 0;
};

} // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_FIND_MAX_PEAK_C_H */
