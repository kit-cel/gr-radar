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
#include "time_plot.h"

namespace gr {
	namespace radar {

		time_plot::time_plot(int interval, std::string label_y, std::vector<float> axis_y, float range_time, std::vector<float>* y, 
		QWidget* parent) : QWidget(parent)
		{
			d_interval = interval;
			d_axis_y = axis_y;
			d_y = y;
			d_label_y = label_y;
			d_range_time = range_time;
			d_refresh_counter = 0;
			d_marker.resize(0);
			d_marker_num.resize(0);
			
			// Setup GUI
			resize(QSize(600,300));
			d_plot = new QwtPlot(this);
			d_symbol = new QwtSymbol(QwtSymbol::Diamond,Qt::red,Qt::NoPen,QSize(15,15));
			d_grid = new QwtPlotGrid;
			
			d_plot->setTitle(QwtText("Time Plot")); 
			d_plot->setAxisScale(QwtPlot::xBottom,0,d_range_time);
			d_plot->setAxisTitle(QwtPlot::xBottom, "time");
			d_plot->setAxisScale(QwtPlot::yLeft,d_axis_y[0],d_axis_y[1]);
			d_plot->setAxisTitle(QwtPlot::yLeft, d_label_y.c_str());
			
			// Add grid
			d_grid->setPen(QPen(QColor(119,136,153),0.5, Qt::DashLine));
			d_grid->attach(d_plot);
			
			// Plot grid and axis
			d_plot->replot();
			
			// Setup timer and connect refreshing plot
			d_timer = new QTimer(this);
			connect(d_timer, SIGNAL(timeout()), this, SLOT(refresh()));
			d_timer->start(d_interval);
		}

		time_plot::~time_plot(){
		}
		
		void
		time_plot::resizeEvent( QResizeEvent * event ){
			d_plot->setGeometry(0,0,this->width(),this->height());
		}
		
		void
		time_plot::refresh(){
			// Reset axis
			if(d_range_time<=d_refresh_counter*float(d_interval)/1000.0){
				d_plot->setAxisScale(QwtPlot::xBottom,d_refresh_counter*float(d_interval)/1000.0-d_range_time,d_refresh_counter*float(d_interval)/1000.0);
			}
			
			// Detach and delete outdated markers
			if(d_marker_num.size()>d_range_time/(float)d_interval*1000.0){ // FIXME: errorhandling for interval>range_time?
				for(int k=0; k<d_marker_num[0]; k++){
					delete d_marker[k]; // FIXME: object is not deleted! segfault, why?
				}
				if(d_marker_num[0]!=0){
					d_marker.erase(d_marker.begin(),d_marker.begin()+d_marker_num[0]); // delete marker pointers
				}
				d_marker_num.erase(d_marker_num.begin()); // delete first element counting vector
			}
			
			// Set new marker
			for(int k=0; k<d_y->size(); k++){
				d_marker.push_back(new QwtPlotMarker);
				d_marker[d_marker.size()-1]->setSymbol(d_symbol);
				d_marker[d_marker.size()-1]->setValue(QPointF(d_refresh_counter*float(d_interval)/1000.0,(*d_y)[k]));
			}
			for(int k=0; k<d_marker.size(); k++){
				d_marker[k]->attach(d_plot);
			}
			d_marker_num.push_back(d_y->size()); // Save number of new markers
			
			std::cout << "NUM: " << d_y->size() << "->" << d_marker.size() << " / " << d_marker_num.size() << std::endl;
			
			// Replot and increment counter
			d_plot->replot();
			d_refresh_counter++;
		}

	}
}
