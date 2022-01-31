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

#include "qtgui_time_plot_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace radar {

qtgui_time_plot::sptr qtgui_time_plot::make(int interval,
                                            std::string label_y,
                                            std::vector<float> axis_y,
                                            float range_time,
                                            std::string label)
{
    return gnuradio::make_block_sptr<qtgui_time_plot_impl>(interval, label_y, axis_y, range_time, label);
}

/*
 * The private constructor
 */
qtgui_time_plot_impl::qtgui_time_plot_impl(int interval,
                                           std::string label_y,
                                           std::vector<float> axis_y,
                                           float range_time,
                                           std::string label)
    : gr::block("qtgui_time_plot",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    d_interval = interval;
    d_label_y = label_y;
    d_axis_y = axis_y;
    d_range_time = range_time;
    d_y_read = false;
    d_label = label;

    // Register input message port
    d_port_id_in = pmt::mp("Msg in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
                    boost::bind(&qtgui_time_plot_impl::handle_msg, this, _1));

    // Setup GUI
    run_gui();
}

/*
 * Our virtual destructor.
 */
qtgui_time_plot_impl::~qtgui_time_plot_impl() {}

void qtgui_time_plot_impl::handle_msg(pmt::pmt_t msg)
{
    size_t size_msg;
    d_y.clear();

    size_msg = pmt::length(msg);
    pmt::pmt_t msg_part;
    bool item_found_y;
    // Go through msg and search for key symbol
    item_found_y = false;
    for (int k = 0; k < size_msg; k++) {
        msg_part = pmt::nth(k, msg);
        if (pmt::symbol_to_string(pmt::nth(0, msg_part)) == d_label_y.c_str()) {
            d_y = pmt::f32vector_elements(pmt::nth(1, msg_part));
            item_found_y = true;
            d_y_read = false;
        }
    }
    // Error handling
    if (!(item_found_y)) {
        throw std::runtime_error("Identifier (symbol) not found");
    }
}

void qtgui_time_plot_impl::run_gui()
{
    // Set QT window
    if (qApp != NULL) {
        d_qApplication = qApp;
    } else {
        d_argc = 1;
        d_argv = new char;
        d_argv[0] = '\0';
        d_qApplication = new QApplication(d_argc, &d_argv);
    }

    // Set QWT plot widget
    d_main_gui = new time_plot(
        d_interval, d_label_y, d_axis_y, d_range_time, &d_y, &d_y_read, d_label);
    d_main_gui->show();
}

} /* namespace radar */
} /* namespace gr */
