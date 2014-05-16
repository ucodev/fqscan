/*
 *	libfqscan - RF Signal Scanner Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fq_tp.cpp
 *
 *	Authors:
 *		- Nicholas Tomlinson (gr-scan v2012082301) (Copyright (C) 2012  Nicholas Tomlinson) - Author of gr-scan
 *		- Pedro A. Hortas (pah@ucodev.org) - Changes:
 *		   + Splitted top_block.hpp (original file) into fq_tp.hpp and fq_tp.cpp
 *		   + Some structural changes to allow the convertion of grscan program to a shared library
 *		   + Implemented reset() functions.
 *		   + Implemented destructors.
 * 		   + Implemented the fq_signal_hook() function pointer to act as a notifier for discovered signals
 *		   + Renamed the single window function to blackman_approx() and also implemented the blackman_exact()
 *		   + Implemented support for the following new optional windowing functions:
 *		    . Rectangular
 *		    . Hann
 *		    . Hamming
 *		    . Tukey
 *		    . Cosine
 *		    . Lanczos
 *		    . Triangular
 *		    . Bartlett (triangular with zero-valued end-points)
 *		    . Bartlett-Hann
 *		    . Kaiser
 *		    . Hann-Poisson
 *		    . Plank-taper
 *		    . Plank-Bessel
 *		    . Gauss
 *		    . Nuttall
 *		    . Blackman-Harris
 *		    . Blackman-Nuttall
 *		    . Flat Top
 *
 *	Last Update: 2014/03/28
 * 
 *	This project contains parts of, or links to, the following projects, licensed under, at least, one GPL version:
 *	  - gr-scan - http://techmeology.co.uk/gr-scan/
 *	  - gnuradio - http://gnuradio.org/
 *	  - osmocomsdr - http://sdr.osmocom.org/trac/
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */


#include <cmath>

#include <gnuradio/gr_top_block.h>
#include <gnuradio/gr_stream_to_vector.h>
#include <gnuradio/gr_fft_vcc_fftw.h>
#include <gnuradio/gr_complex_to_xxx.h>
#include <gnuradio/gr_single_pole_iir_filter_ff.h>
#include <gnuradio/gr_nlog10_ff.h>

#include <osmosdr/osmosdr_source_c.h>

#include <librfsp/rfsp_window.hpp>

#include "fq_tb.hpp"
#include "fq_sink.hpp"
#include "fq_filters.hpp"

using namespace std;

fq_tp::fq_tp(
	double freq_start,
	double freq_end,
	double sample_rate,
	double fft_width,
	double bandwidth_fine,
	double bandwidth_coarse,
	double step,
	unsigned int avg_size,
	double spread,
	double threshold,
	double ptime,
	double freq_correction,
	bool gain_mode,
	double gain,
	double if_gain,
	FILTER filter,
	bool filter_args,
	double filter_a1,
	double filter_a2,
	double filter_a3,
	double filter_a4,
	double filter_a5,
	int (*fq_signal_hook) (unsigned int fq_time, double fq_freq, double fq_width, float fq_power, float fq_diff, double fq_band_start, double fq_band_end),
	int (*fq_freq_range_hook) (unsigned int fq_time, double fq_band_start, double fq_band_end)
	) :
	gr_top_block("Top Block"),
	vector_length(sample_rate / fft_width),
	window(get_window(vector_length, filter, filter_args, filter_a1, filter_a2, filter_a3, filter_a4, filter_a5)), /* Setup the window function */
	source(osmosdr_make_source_c()), /* OsmoSDR Source */
	stv(gr_make_stream_to_vector(sizeof(float) * 2, vector_length)), /* Stream to vector */

	/* Based on the logpwrfft (a block implemented in python) */
	fft(gr_make_fft_vcc_fftw(vector_length, true, window, false, 1)),
	ctf(gr_make_complex_to_mag_squared(vector_length)),
	iir(gr_make_single_pole_iir_filter_ff(1.0, vector_length)),
	lg(gr_make_nlog10_ff(10, vector_length, (-20 * log10(vector_length)) - (10 * log10(get_window_power() / vector_length)))),
	/* Sink - this does most of the interesting work */
	sink(make_fq_sink(source, vector_length, freq_start, freq_end, sample_rate, bandwidth_fine, bandwidth_coarse, step, avg_size, spread, threshold, ptime, fq_signal_hook, fq_freq_range_hook))
{
	/* Set up the OsmoSDR Source */
	source->set_sample_rate(sample_rate);
	source->set_center_freq(freq_start);
	source->set_freq_corr(freq_correction);
	source->set_gain_mode(gain_mode);
	source->set_gain(gain);
	source->set_if_gain(if_gain);
		
	/* Set up the connections */
	connect(source, 0, stv, 0);
	connect(stv, 0, fft, 0);
	connect(fft, 0, ctf, 0);
	connect(ctf, 0, iir, 0);
	connect(iir, 0, lg, 0);
	connect(lg, 0, sink, 0);
}

fq_tp::~fq_tp() {
	sink.reset();

	lock();

	disconnect(lg, 0, sink, 0);
	disconnect(iir, 0, lg, 0);
	disconnect(ctf, 0, iir, 0);
	disconnect(fft, 0, ctf, 0);
	disconnect(stv, 0, fft, 0);
	disconnect(source, 0, stv, 0);

	unlock();
}

void fq_tp::reset() {
	sink.reset();
}

vector<float> fq_tp::get_window(
		size_t n,
		FILTER filter,
		bool filter_args,
		double a1,
		double a2,
		double a3,
		double a4,
		double a5)
{
	rfsp_window w;

	if (!filter_args) {
		switch (filter) {
			case FILTER_RECTANGULAR:	return w.hr_rectangular(n);
			case FILTER_BARTLETT:		return w.mr_bartlett(n);
			case FILTER_BARTLETT_HANN:	return w.mr_bartlett_hann(n);
			case FILTER_BLACKMAN:		return w.mr_blackman(n);
			case FILTER_COSINE:		return w.mr_cosine(n);
			case FILTER_DOLPH_CHEBYSHEV:	return w.mr_dolph_chebyshev(n);
			case FILTER_GAUSS:		return w.mr_gauss(n);
			case FILTER_HAMMING:		return w.mr_hamming(n);
			case FILTER_HANN:		return w.mr_hann(n);
			case FILTER_HANN_POISSON:	return w.mr_hann_poisson(n);
			case FILTER_KAISER:		return w.mr_kaiser(n);
			case FILTER_PLANCK_BESSEL:	return w.mr_planck_bessel(n);
			case FILTER_PLANCK_TAPER:	return w.mr_planck_taper(n);
			case FILTER_SARAMAKI:		return w.mr_saramaki(n);
			case FILTER_LANCZOS:		return w.mr_lanczos(n);
			case FILTER_TRIANGULAR:		return w.mr_triangular(n);
			case FILTER_TUKEY:		return w.mhr_tukey(n);
			case FILTER_BLACKMAN_HARRIS:	return w.lr_blackman_harris(n);
			case FILTER_BLACKMAN_NUTTALL:	return w.lr_blackman_nuttall(n);
			case FILTER_FLAT_TOP:		return w.lr_flat_top(n);
			case FILTER_NUTTALL:		return w.lr_nuttall(n);
			default:			abort();
		}
	} else {
		switch (filter) {
			case FILTER_BARTLETT_HANN:	return w.mr_bartlett_hann(n, a1, a2, a3);
			case FILTER_BLACKMAN:		return w.mr_blackman(n, a1, a2, a3);
			case FILTER_DOLPH_CHEBYSHEV:	return w.mr_dolph_chebyshev(n, a1);
			case FILTER_GAUSS:		return w.mr_gauss(n, a1);
			case FILTER_HANN_POISSON:	return w.mr_hann_poisson(n, a1);
			case FILTER_KAISER:		return w.mr_kaiser(n, a1);
			case FILTER_PLANCK_BESSEL:	return w.mr_planck_bessel(n, a1, a2);
			case FILTER_PLANCK_TAPER:	return w.mr_planck_taper(n, a1);
			case FILTER_TUKEY:		return w.mhr_tukey(n, a1);
			case FILTER_ULTRASPHERICAL:	return w.mr_ultraspherical(n, a1, a2);
			case FILTER_GENERIC_4TH:	return w.lr_generic_4th(n, a1, a2, a3, a4);
			case FILTER_GENERIC_5TH:	return w.lr_generic_5th(n, a1, a2, a3, a4, a5);
			default:			abort();
		}
	}

	/** We'll enver reach this. Just avoid compiler warnings */
	abort();

	return w.mr_dolph_chebyshev(n);
}

double fq_tp::get_window_power() {
	double total = 0.0;

	for (double d: window) {
		total += d * d;
	}

	return total;
}

