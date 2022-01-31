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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "os_cfar_2d_vc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

os_cfar_2d_vc::sptr os_cfar_2d_vc::make(int vlen,
                                        std::vector<int> samp_compare,
                                        std::vector<int> samp_protect,
                                        float rel_threshold,
                                        float mult_threshold,
                                        const std::string& len_key)
{
    return gnuradio::make_block_sptr<os_cfar_2d_vc_impl>(
        vlen, samp_compare, samp_protect, rel_threshold, mult_threshold, len_key);
}

/*
 * The private constructor
 */
os_cfar_2d_vc_impl::os_cfar_2d_vc_impl(int vlen,
                                       std::vector<int> samp_compare,
                                       std::vector<int> samp_protect,
                                       float rel_threshold,
                                       float mult_threshold,
                                       const std::string& len_key)
    : gr::tagged_stream_block("os_cfar_2d_vc",
                              gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen),
                              gr::io_signature::make(0, 0, 0),
                              len_key)
{
    d_vlen = vlen;
    d_samp_compare = samp_compare;
    d_samp_protect = samp_protect;
    d_rel_threshold = rel_threshold;
    d_mult_threshold = mult_threshold;

    // Register message port
    d_port_id = pmt::mp("Msg out");
    message_port_register_out(d_port_id);
}

/*
 * Our virtual destructor.
 */
os_cfar_2d_vc_impl::~os_cfar_2d_vc_impl() {}

int os_cfar_2d_vc_impl::calculate_output_stream_length(const gr_vector_int& ninput_items)
{
    int noutput_items = 0;
    return noutput_items;
}

void os_cfar_2d_vc_impl::set_rel_threshold(float inp) { d_rel_threshold = inp; }

void os_cfar_2d_vc_impl::set_mult_threshold(float inp) { d_mult_threshold = inp; }

void os_cfar_2d_vc_impl::set_samp_compare(std::vector<int> inp) { d_samp_compare = inp; }

void os_cfar_2d_vc_impl::set_samp_protect(std::vector<int> inp) { d_samp_protect = inp; }

int os_cfar_2d_vc_impl::work(int noutput_items,
                             gr_vector_int& ninput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    const gr_complex* in = (const gr_complex*)input_items[0];

    // OS-CFAR detection (2D)

    // Get mean power
    int ylen = ninput_items[0]; // size of y axis
    float mean_power = 0;
    for (int x = 0; x < d_vlen; x++) {   // go through x axis
        for (int y = 0; y < ylen; y++) { // go through y axis
            mean_power += std::pow(std::abs(in[x + d_vlen * y]), 2);
        }
    }
    mean_power = mean_power / d_vlen / ylen;

    // Go through matrix and select CUT (cell under test)
    d_x.clear();
    d_y.clear();
    d_power.clear();
    std::vector<float> hold_samp;        // vector for holding samples
    for (int x = 0; x < d_vlen; x++) {   // go through x axis
        for (int y = 0; y < ylen; y++) { // go through y axis

            // Gather samples around CUT, logarithmic values are not possible!
            hold_samp.clear();
            for (int kx = 1; kx < d_samp_compare[0] + 1;
                 kx++) { // go through compare samples on x axis
                for (int ky = 1; ky < d_samp_compare[1] + 1;
                     ky++) { // go through compare samples on y axis

                    // Do not evaluate CUT
                    if (kx == 0 && ky == 0)
                        continue;

                    // Check lower right corner of the compare field
                    if (x + kx + d_samp_compare[0] >= d_vlen ||
                        y + ky + d_samp_compare[1] >=
                            ylen) { // push back mean_power if not in matrix
                        hold_samp.push_back(mean_power);
                    } else { // push back value of the matrix
                        hold_samp.push_back(
                            std::pow(std::abs(in[x + kx + d_samp_compare[0] +
                                                 d_vlen * (y + ky + d_samp_compare[1])]),
                                     2));
                    }

                    // Check upper right corner of the compare field
                    if (x + kx + d_samp_compare[0] >= d_vlen ||
                        y - ky - d_samp_compare[1] <
                            0) { // push back mean_power if not in matrix
                        hold_samp.push_back(mean_power);
                    } else { // push back value of the matrix
                        hold_samp.push_back(
                            std::pow(std::abs(in[x + kx + d_samp_compare[0] +
                                                 d_vlen * (y - ky - d_samp_compare[1])]),
                                     2));
                    }

                    // Check upper left corner of the compare field
                    if (x - kx - d_samp_compare[0] < 0 ||
                        y - ky - d_samp_compare[1] <
                            0) { // push back mean_power if not in matrix
                        hold_samp.push_back(mean_power);
                    } else { // push back value of the matrix
                        hold_samp.push_back(
                            std::pow(std::abs(in[x - kx - d_samp_compare[0] +
                                                 d_vlen * (y - ky - d_samp_compare[1])]),
                                     2));
                    }

                    // Check lower left corner of the compare field
                    if (x - kx - d_samp_compare[0] < 0 ||
                        y + ky + d_samp_compare[1] >=
                            ylen) { // push back mean_power if not in matrix
                        hold_samp.push_back(mean_power);
                    } else { // push back value of the matrix as abs-square
                        hold_samp.push_back(
                            std::pow(std::abs(in[x - kx - d_samp_compare[0] +
                                                 d_vlen * (y + ky + d_samp_compare[1])]),
                                     2));
                    }
                }
            }

            // Sort d_hold vector
            std::sort(hold_samp.begin(), hold_samp.end());

            // Accept CUT if
            // pow(abs(sort_vector[len_sort_vector*rel_threshold]*mult_threshold),2) <
            // pow(abs(value_CUT),2)
            float compare_value =
                hold_samp[int((hold_samp.size() - 1) * d_rel_threshold)] *
                d_mult_threshold;
            if (compare_value < std::pow(std::abs(in[x + d_vlen * y]), 2)) {
                d_x.push_back(x);
                d_y.push_back(y);
                d_power.push_back(std::pow(std::abs(in[x + d_vlen * y]), 2));
            }
        }
    }

    // Get rx_time tag
    get_tags_in_range(
        d_tags, 0, nitems_read(0), nitems_read(0) + 1, pmt::string_to_symbol("rx_time"));

    // Setup output msg
    if (d_tags.size() > 0)
        d_ptimestamp = pmt::list2(pmt::string_to_symbol("rx_time"), d_tags[0].value);
    else
        d_ptimestamp = pmt::list2(
            pmt::string_to_symbol("rx_time"),
            pmt::make_tuple(
                pmt::from_uint64(0),
                pmt::from_double(
                    -1))); // if no timetag is found, set to 0 and frac_sec to -1
    d_ppower = pmt::list2(pmt::string_to_symbol("power"),
                          pmt::init_f32vector(d_power.size(), d_power));
    d_px =
        pmt::list2(pmt::string_to_symbol("axis_x"), pmt::init_f32vector(d_x.size(), d_x));
    d_py =
        pmt::list2(pmt::string_to_symbol("axis_y"), pmt::init_f32vector(d_y.size(), d_y));
    d_value = pmt::list4(d_ptimestamp, d_px, d_py, d_ppower);

    // Publish message
    message_port_pub(d_port_id, d_value);

    // Tell runtime system how many output items we produced.
    return 0;
}

} /* namespace radar */
} /* namespace gr */
