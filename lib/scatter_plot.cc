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
#include "scatter_plot.h"

namespace gr {
	namespace radar {

		scatter_plot::scatter_plot(int interval, std::vector<float> axis_x, std::vector<float> axis_y, std::vector<float>* x, std::vector<float>* y, std::string label_x, std::string label_y, bool* xy_read,
		QWidget* parent) : QWidget(parent)
		{
			d_interval = interval;
			d_xy_read = xy_read;
			d_axis_x = axis_x;
			d_axis_y = axis_y;
			d_x = x;
			d_y = y;
			d_label_x = label_x;
			d_label_y = label_y;
			d_marker.resize(0);
			
			// Setup GUI
			resize(QSize(600,600));
			d_plot = new QwtPlot(this);
			d_symbol = new QwtSymbol(QwtSymbol::Diamond,Qt::red,Qt::NoPen,QSize(20,20));
			d_grid = new QwtPlotGrid;
			
			std::string label_title = "Scatter Plot: ";
			label_title.append(label_x);
			label_title.append("/");
			label_title.append(label_y);
			d_plot->setTitle(QwtText(label_title.c_str())); 
			d_plot->setAxisScale(QwtPlot::xBottom,d_axis_x[0],d_axis_x[1]);
			d_plot->setAxisTitle(QwtPlot::xBottom, d_label_x.c_str());
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

		scatter_plot::~scatter_plot(){
		}
		
		void
		scatter_plot::resizeEvent( QResizeEvent * event ){
			d_plot->setGeometry(0,0,this->width(),this->height());
		}
		
		void
		scatter_plot::refresh(){
			// Clear plot from old markers
			for(int k=0; k<d_marker.size(); k++) d_marker[k]->detach();
			
			if(not(*d_xy_read)){
				// Set new marker
				for(int k=0; k<d_x->size(); k++){ // len(range)==len(velocity), same size x and y! FIXME: errorhandling!
					if(k<d_marker.size()){
						d_marker[k]->setValue(QPointF((*d_x)[k],(*d_y)[k]));
						d_marker[k]->attach(d_plot);
					}
					else{
						d_marker.push_back(new QwtPlotMarker);
						d_marker[k]->setSymbol(d_symbol);
						d_marker[k]->setValue(QPointF((*d_x)[k],(*d_y)[k]));
						d_marker[k]->attach(d_plot);
					}
				}
				*d_xy_read = true; // set points as read
			}
			
			// Replot
			d_plot->replot();
		}

	}
}
