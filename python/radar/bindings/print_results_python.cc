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
/* BINDTOOL_HEADER_FILE(print_results.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(924d069bc77b8c935ae0b6e24c1f5f2c)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <radar/print_results.h>
// pydoc.h is automatically generated in the build directory
#include <print_results_pydoc.h>

void bind_print_results(py::module& m)
{

    using print_results = ::gr::radar::print_results;


    py::class_<print_results, gr::block, gr::basic_block, std::shared_ptr<print_results>>(
        m, "print_results", D(print_results))

        .def(py::init(&print_results::make),
             py::arg("store_msg") = false,
             py::arg("filename") = "",
             D(print_results, make))


        ;
}