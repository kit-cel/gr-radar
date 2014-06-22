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

		range_velocity_diagram::range_velocity_diagram(int interval, std::vector<float> axis_range, std::vector<float> axis_velocity, std::vector<float>* range, std::vector<float>* velocity){
			d_interval = interval;
			d_axis_range = axis_range;
			d_axis_velocity = axis_velocity;
			d_range = range;
			d_velocity = velocity;
			d_marker.resize(0);
			
			// Setup GUI
			d_plot = new QwtPlot(this);
			d_symbol = new QwtSymbol(QwtSymbol::Diamond,Qt::red,Qt::NoPen,QSize(20,20));
			d_grid = new QwtPlotGrid;
			
			d_plot->setTitle(QwtText("Range Velocity Diagram")); 
			d_plot->setGeometry(0,0,600,600);
			d_plot->setAxisScale(QwtPlot::xBottom,d_axis_range[0],d_axis_range[1]);
			d_plot->setAxisTitle(QwtPlot::xBottom, "Range [m]");
			d_plot->setAxisScale(QwtPlot::yLeft,d_axis_velocity[0],d_axis_velocity[1]);
			d_plot->setAxisTitle(QwtPlot::yLeft, "Velocity [m/s]");
			
			// Add grid
			d_grid->setPen(QPen(QColor(119,136,153),0.5, Qt::DashLine));
			d_grid->attach(d_plot);
			
			d_plot->replot();
			
			// Setup timer
			d_timer = new QTimer(this);
			connect(d_timer, SIGNAL(timeout()), this, SLOT(refresh()));
			d_timer->start(d_interval);
		}

		range_velocity_diagram::~range_velocity_diagram(){
		}
		
		void
		range_velocity_diagram::refresh(){
			// Clear plot from old markers
			for(int k=0; k<d_marker.size(); k++) d_marker[k]->detach();
			
			// Set new marker
			for(int k=0; k<(*d_velocity).size(); k++){ // len(velocity)==len(range)
				if(k<d_marker.size()){
					d_marker[k]->setValue(QPointF((*d_range)[k],(*d_velocity)[k]));
					d_marker[k]->attach(d_plot);
				}
				else{
					d_marker.push_back(new QwtPlotMarker);
					d_marker[k]->setSymbol(d_symbol);
					d_marker[k]->setValue(QPointF((*d_range)[k],(*d_velocity)[k]));
					d_marker[k]->attach(d_plot);
				}
			}
			
			// Replot
			d_plot->replot();
		}

	}
}
