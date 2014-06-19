/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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

#include <iostream>
#include "range_velocity_diagram.h"

namespace gr {
	namespace radar {

		range_velocity_diagram::range_velocity_diagram(){
			setWindowTitle("Range Velocity Diagram");
			setFixedSize(600, 600);
			
			d_plot = new QwtPlot(this);

			d_curve = new QwtPlotCurve("Curve 1");


			QwtPointSeriesData* myData = new QwtPointSeriesData;

			QVector<QPointF>* samples = new QVector<QPointF>;
			samples->push_back(QPointF(1.0,1.0));
			samples->push_back(QPointF(2.0,2.0));
			samples->push_back(QPointF(3.0,3.0));
			samples->push_back(QPointF(4.0,5.0));
			myData->setSamples(*samples);
			d_curve->setData(myData);

			d_curve->attach(d_plot);

			d_plot->replot();
		}

		range_velocity_diagram::~range_velocity_diagram(){
		}
		
		void
		range_velocity_diagram::refresh(){
			d_plot->replot();
		}

	}
}
