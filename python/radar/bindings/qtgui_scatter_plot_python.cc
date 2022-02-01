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
/* BINDTOOL_HEADER_FILE(qtgui_scatter_plot.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(4e4d605a7b90a8b3a37dd8cf73c4d7cc)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <radar/qtgui_scatter_plot.h>
// pydoc.h is automatically generated in the build directory
#include <qtgui_scatter_plot_pydoc.h>

void bind_qtgui_scatter_plot(py::module& m)
{

    using qtgui_scatter_plot    = ::gr::radar::qtgui_scatter_plot;


    py::class_<qtgui_scatter_plot, gr::block, gr::basic_block,
        std::shared_ptr<qtgui_scatter_plot>>(m, "qtgui_scatter_plot", D(qtgui_scatter_plot))

        .def(py::init(&qtgui_scatter_plot::make),
           py::arg("interval"),
           py::arg("label_x"),
           py::arg("label_y"),
           py::arg("axis_x"),
           py::arg("axis_y"),
           py::arg("label") = "",
           D(qtgui_scatter_plot,make)
        )




        ;




}
