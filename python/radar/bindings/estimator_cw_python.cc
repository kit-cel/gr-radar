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
/* BINDTOOL_HEADER_FILE(estimator_cw.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(1a450631129b480dd9cd09bc07c5a418)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <radar/estimator_cw.h>
// pydoc.h is automatically generated in the build directory
#include <estimator_cw_pydoc.h>

void bind_estimator_cw(py::module& m)
{

    using estimator_cw = ::gr::radar::estimator_cw;


    py::class_<estimator_cw, gr::block, gr::basic_block, std::shared_ptr<estimator_cw>>(
        m, "estimator_cw", D(estimator_cw))

        .def(py::init(&estimator_cw::make), py::arg("center_freq"), D(estimator_cw, make))


        ;
}
