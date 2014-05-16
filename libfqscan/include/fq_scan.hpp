/*
 *	libfqscan - RF Signal Scanner Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fq_scan.hpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
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



#ifndef LIBFQSCAN_FQ_SCAN_HPP
#define LIBFQSCAN_FQ_SCAN_HPP

#include "fq_tb.hpp"
#include "fq_filters.hpp"

class fq_scan {
	public:
		fq_tp top_block;

		fq_scan(
			double freq_start,
			double freq_end,
			double sample_rate,
			double fft_width,
			double bandwidth_fine,
			double bandwidth_coarse,
			double inc_step,
			unsigned int avg_size,
			double spread,
			double power_threshold,
			double freq_timeout,
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
		);

		~fq_scan();

		void setup(
			double freq_start,
			double freq_end,
			double sample_rate,
			double fft_width,
			double bandwidth_fine,
			double bandwidth_coarse,
			double inc_step,
			unsigned int avg_size,
			double spread,
			double power_threshold,
			double freq_timeout,
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

		void start();

		void wait();

		void stop();

		void reset();
};

#endif
