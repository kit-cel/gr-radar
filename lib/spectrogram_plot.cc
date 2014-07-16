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

#include "spectrogram_plot.h"

namespace gr {
	namespace radar {

		spectrogram_plot::spectrogram_plot(int interval, int vlen, std::vector<float> *buffer, std::string label_x, std::string label_y, std::string label,
		QWidget* parent) : QWidget(parent)
		{
			d_interval = interval;
			d_vlen = vlen;
			d_buffer = buffer;
			
			// Setup GUI
			resize(QSize(600,600));
			
			d_plot = new QwtPlot(this); // make main plot
			d_spectrogram = new QwtPlotSpectrogram(); // make spectrogram
			setColorMap(gray);
			d_spectrogram->attach(d_plot);
			
			d_data = new SpectrogramData(); // make data structure
			d_spectrogram->setData(d_data);
			
			// Plot axis and title
			std::string label_title = "Spectrogram Plot: ";
			label_title.append(label_x);
			label_title.append("/");
			label_title.append(label_y);
			if(label!=""){
				label_title.append(" (");
				label_title.append(label);
				label_title.append(")");
			}
			d_plot->setTitle(QwtText(label_title.c_str())); 
			//d_plot->setAxisScale(QwtPlot::xBottom,d_axis_x[0],d_axis_x[1]);
			d_plot->setAxisTitle(QwtPlot::xBottom, label_x.c_str());
			//d_plot->setAxisScale(QwtPlot::yLeft,d_axis_y[0],d_axis_y[1]);
			d_plot->setAxisTitle(QwtPlot::yLeft, label_y.c_str());
			
			d_plot->replot();
			
			// Setup timer and connect refreshing plot
			d_timer = new QTimer(this);
			connect(d_timer, SIGNAL(timeout()), this, SLOT(refresh()));
			d_timer->start(d_interval);
		}

		spectrogram_plot::~spectrogram_plot(){
		}
		
		void
		spectrogram_plot::resizeEvent( QResizeEvent * event ){
			d_plot->setGeometry(0,0,this->width(),this->height());
		}
		
		void
		spectrogram_plot::refresh(){
			d_plot->replot();
		}

	}
}
