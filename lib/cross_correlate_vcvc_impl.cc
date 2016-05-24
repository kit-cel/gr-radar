/* -*- c++ -*- */
/*
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "cross_correlate_vcvc_impl.h"
#include <volk/volk.h>

namespace gr {
  namespace radar {

    cross_correlate_vcvc::sptr
    cross_correlate_vcvc::make(int vlen)
    {
      return gnuradio::get_initial_sptr
        (new cross_correlate_vcvc_impl(vlen));
    }

    /*
     * The private constructor
     */
    cross_correlate_vcvc_impl::cross_correlate_vcvc_impl(int vlen)
      : gr::sync_block("cross_correlate_vcvc",
                      gr::io_signature::make(2, 2, sizeof(gr_complex) * vlen),
                      gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen))
    {
      // Verify datatype
      if(sizeof(gr_complex) != sizeof(fftw_complex))
        std::runtime_error("sizeof(gr_complex) != sizeof(fftw_complex)");
      d_vlen = vlen;
      d_fft_buffer1 = (fftwf_complex*) fftwf_malloc(sizeof(fftw_complex) * d_vlen);
      d_fft_buffer2 = (fftwf_complex*) fftwf_malloc(sizeof(fftw_complex) * d_vlen);
      d_fft_plan1 = fftwf_plan_dft_1d(d_vlen, d_fft_buffer1, d_fft_buffer1,
                                      FFTW_FORWARD, FFTW_ESTIMATE);
      d_fft_plan2 = fftwf_plan_dft_1d(d_vlen, d_fft_buffer2, d_fft_buffer2,
                                      FFTW_FORWARD, FFTW_ESTIMATE);

      // Correct volk alignment
      const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
      set_alignment(std::max(1, alignment_multiple));
    }

    /*
     * Our virtual destructor.
     */
    cross_correlate_vcvc_impl::~cross_correlate_vcvc_impl()
    {
      fftwf_destroy_plan(d_fft_plan1);
      fftwf_destroy_plan(d_fft_plan2);
      fftwf_free(d_fft_buffer1);
      fftwf_free(d_fft_buffer2);
    }

    int
    cross_correlate_vcvc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Cross correlation is defined as ifft(fft(conj(a)*fft(b)))
      const gr_complex *in1 = (const gr_complex *) input_items[0];
      const gr_complex *in2 = (const gr_complex *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];

      memcpy(d_fft_buffer1, in1, d_vlen*sizeof(gr_complex));
      memcpy(d_fft_buffer2, in2, d_vlen*sizeof(gr_complex));

      // Move to frequency domain
      fftwf_execute(d_fft_plan1);
      fftwf_execute(d_fft_plan2);
      int noi = d_vlen*noutput_items;

      // Calculate cross correlation
      // NOTE: Without zeropadding ffts, this will be a circular cross correlation
      volk_32fc_x2_multiply_conjugate_32fc(out, (gr_complex *) d_fft_buffer1,
                            (gr_complex *) d_fft_buffer2, noi);

      // Move back to time domain
      fftwf_plan ifft_plan = fftwf_plan_dft_1d(d_vlen, (fftwf_complex*) out,
                            (fftwf_complex*) out, FFTW_BACKWARD, FFTW_ESTIMATE);
      fftwf_execute(ifft_plan);
      fftwf_destroy_plan(ifft_plan);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace radar */
} /* namespace gr */
