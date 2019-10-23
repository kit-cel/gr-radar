/* -*- c++ -*- */
/*
 * Copyright 2014 Communications Engineering Lab, KIT.
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

		class scatter_plot : public QWidget
		{
		Q_OBJECT

		public:
			scatter_plot(int interval, std::vector<float> axis_x, std::vector<float> axis_y, std::vector<float>* x, std::vector<float>* y, std::string label_x, std::string label_y, bool* xy_read, std::string label,
			QWidget* parent = 0);
			~scatter_plot();

		private:
			int d_interval;
			std::string d_label_x, d_label_y;
			std::vector<float> d_axis_x, d_axis_y;
			std::vector<float>* d_x, *d_y;
			bool* d_xy_read;
			QwtPlot* d_plot;
			QwtSymbol* d_symbol;
			QwtPlotGrid* d_grid;
			std::vector<QwtPlotMarker*> d_marker;
			QTimer *d_timer;

		protected:
			void resizeEvent(QResizeEvent * event);

		public slots:
			void refresh();

		};

	}
}
