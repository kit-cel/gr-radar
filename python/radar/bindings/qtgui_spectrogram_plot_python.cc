/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(qtgui_spectrogram_plot.h) */
/* BINDTOOL_HEADER_FILE_HASH(ff65522254da20afbc0e0ad04f44244b)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <radar/qtgui_spectrogram_plot.h>
// pydoc.h is automatically generated in the build directory
#include <qtgui_spectrogram_plot_pydoc.h>

void bind_qtgui_spectrogram_plot(py::module& m)
{

    using qtgui_spectrogram_plot = ::gr::radar::qtgui_spectrogram_plot;


    py::class_<qtgui_spectrogram_plot,
               gr::tagged_stream_block,
               gr::block,
               gr::basic_block,
               std::shared_ptr<qtgui_spectrogram_plot>>(
        m, "qtgui_spectrogram_plot", D(qtgui_spectrogram_plot))

        .def(py::init(&qtgui_spectrogram_plot::make),
             py::arg("vlen"),
             py::arg("interval"),
             py::arg("xlabel"),
             py::arg("ylabel"),
             py::arg("label"),
             py::arg("axis_x"),
             py::arg("axis_y"),
             py::arg("axis_z"),
             py::arg("autoscale_z"),
             py::arg("len_key") = "packet_len",
             D(qtgui_spectrogram_plot, make))


        ;
}
