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
#include <iostream>

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
			d_spectrogram->attach(d_plot);
			
			d_data = new QwtMatrixRasterData(); // make data structure
			
			QVector<double> zData;
			int columns = 100;
			int rows = 200;
			int data_len = rows*columns;
			for(int i = 0; i < data_len; i++){
				zData.push_back(i*i);
			}
			
			float maximum, minimum;
			maximum = zData[0];
			minimum = zData[0];
			for(int k=0; k<data_len; k++){
				if(zData[k]>maximum) maximum = zData[k];
				if(zData[k]<minimum) maximum = zData[k];
			}
			
			d_data->setValueMatrix(zData, columns);
			d_data->setInterval(Qt::XAxis,QwtInterval(0, 100));
			d_data->setInterval(Qt::YAxis,QwtInterval(0, 100));
			d_data->setInterval(Qt::ZAxis,QwtInterval(minimum, maximum));
			
			d_spectrogram->setData(d_data);
			
			d_colormap = new QwtLinearColorMap(Qt::darkCyan, Qt::red); // make colormap
			d_colormap->addColorStop(0.1, Qt::cyan);
			d_colormap->addColorStop(0.6, Qt::green);
			d_colormap->addColorStop(0.95, Qt::yellow);
			
			d_spectrogram->setColorMap(d_colormap);
			
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
			d_plot->setAxisTitle(QwtPlot::xBottom, label_x.c_str());
			d_plot->setAxisTitle(QwtPlot::yLeft, label_y.c_str());
			
			// Colormap
			QwtInterval range(minimum, maximum);
			QwtScaleWidget *scale = d_plot->axisWidget(QwtPlot::yRight);
			scale->setColorBarEnabled(true);
			scale->setColorMap(range, d_colormap);
			
			// Do replot
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
			// Fetch new data
			d_plot_data.clear();
			d_plot_data.resize(d_buffer->size());
			for(int k=0; k<d_buffer->size(); k++) d_plot_data[k] = (*d_buffer)[k];
			for(int k=0; k<d_buffer->size(); k++) std::cout << d_plot_data[k] << std::endl;
			
			// Do replot
			d_plot->replot();
		}

	}
}
