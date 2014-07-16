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

#include <complex>

#include <QApplication>
#include <QTimer>
#include <QWidget>

#include <qwt_plot.h>
#include <qwt_color_map.h>
#include <qwt_plot_spectrogram.h>

namespace gr {
	namespace radar {
		
		class SpectrogramData: public QwtRasterData
		{
		public:
			double value(double x, double y) const
				{
					return 0;
				}
		};
		
		class spectrogram_plot : public QWidget
		{
		Q_OBJECT

		public:
			spectrogram_plot(int interval, int vlen, std::vector<float> *buffer, std::string label_x, std::string label_y, std::string label,
			QWidget* parent = 0);
			~spectrogram_plot();
			
		private:
			int d_interval, d_vlen;
			std::vector<float> *d_buffer;
			QTimer *d_timer;
			
			QwtPlot *d_plot;
			QwtPlotSpectrogram *d_spectrogram;
			SpectrogramData *d_data;
			
		protected:
			void resizeEvent(QResizeEvent * event);
			
		public slots:
			void refresh();
			
		};
	}
}
