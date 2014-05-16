/*
 *	libfqscan - RF Signal Scanner Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fq_tp.hpp
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


#ifndef LIBFQSCAN_FQ_TP_HPP
#define LIBFQSCAN_FQ_TP_HPP

#include <osmosdr/osmosdr_source_c.h>

#include <gnuradio/gr_top_block.h>
#include <gnuradio/gr_stream_to_vector.h>
#include <gnuradio/gr_fft_vcc_fftw.h>
#include <gnuradio/gr_complex_to_xxx.h>
#include <gnuradio/gr_single_pole_iir_filter_ff.h>
#include <gnuradio/gr_nlog10_ff.h>

#include "fq_sink.hpp"
#include "fq_filters.hpp"

using namespace std;

class fq_tp : public gr_top_block
{
	public:
		fq_tp(
			double freq_start,
			double freq_end,
			double sample_rate,
			double fft_width,
			double bandwidth1,
			double bandwidth2,
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
			int (*fq_freq_range_hook) (unsigned int fq_time, double fq_band_start, double fq_band_end));

		virtual ~fq_tp();

		virtual void reset();

	private:
		vector<float> get_window(
			size_t n,
			FILTER filter,
			bool filter_args,
			double filter_a1,
			double filter_a2,
			double filter_a3,
			double filter_a4,
			double filter_a5);

		double get_window_power();

		size_t vector_length;
		vector<float> window;
		
		osmosdr_source_c_sptr source;
		gr_stream_to_vector_sptr stv;
		gr_fft_vcc_sptr fft;
		gr_complex_to_mag_squared_sptr ctf;
		gr_single_pole_iir_filter_ff_sptr iir;
		gr_nlog10_ff_sptr lg;
		fq_sink_sptr sink;
};

#endif
