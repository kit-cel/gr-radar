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

#include <QApplication>
#include <QTimer>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_marker.h>

namespace gr {
	namespace radar {
		
		class range_velocity_diagram : public QWidget
		{
		Q_OBJECT

		public:
			range_velocity_diagram(int interval, std::vector<float> axis_range, std::vector<float> axis_velocity, std::vector<float>* range, std::vector<float>* velocity);
			~range_velocity_diagram();
			
		private:
			int d_interval;
			std::vector<float> d_axis_range, d_axis_velocity;
			std::vector<float>* d_range, *d_velocity;
			QwtPlot* d_plot;
			QwtSymbol* d_symbol;
			QwtPlotGrid* d_grid;
			std::vector<QwtPlotMarker*> d_marker;
			QTimer *d_timer;
			
		public slots:
			void refresh();

		signals:
			
		};

	}
}
