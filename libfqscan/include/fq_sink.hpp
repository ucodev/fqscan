/*
 *	libfqscan - RF Signal Scanner Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fq_sink.hpp
 *
 *	Authors:
 *		- Nicholas Tomlinson (gr-scan v2012082301) (Copyright (C) 2012  Nicholas Tomlinson) - Author of gr-scan
 *		- Pedro A. Hortas (pah@ucodev.org) - Changes:
 *		   + Splitted scanner_sink.hpp (original file) into fq_sink.hpp and fq_sink.cpp
 * 		   + Some structural changes to allow the convertion of grscan program to a shared library
 * 		   + Stop worker gracefully through WORK_DONE magic number.
 * 		   + When WORK_DONE is returned by general_work(), the sink object is now left as it was at the
 *		     construction time, allowing smooth flow control operations on top block object.
 * 		   + Implemented a fq_signal_hook() function pointer to act as a notifier for the discovered signals
 * 		   + Implemented a fq_freq_range_hook() function pointer to act as a notifier for the current tunned
 *		     frequecy range
 * 		   + Removed the hardcoded data output, leaving the data processing of newly discovered frequencies to
 *		     the newly implemented hook fq_signal_hook() and the current tunned frequency range to the
 *		     fq_freq_range_hook()
 *		   + Enhanced destructors
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


#ifndef LIBFQSCAN_FQ_SINK_HPP
#define LIBFQSCAN_FQ_SINK_HPP

#include <set>

#include <boost/shared_ptr.hpp>

#include <gnuradio/gr_block.h>

using namespace std;

class fq_sink : public gr_block
{
	public:
		fq_sink(
			osmosdr_source_c_sptr source,
			unsigned int vector_length,
			double freq_start,
			double freq_end,
			double sample_rate,
			double bandwidth_fine,
			double bandwidth_coarse,
			double step,
			unsigned int avg_size,
			double spread,
			double threshold,
			double ptime,
			int (*fq_signal_hook) (unsigned int fq_time, double fq_freq, double fq_width, float fq_power, float fq_diff, double fq_band_start, double fq_band_end),
			int (*fq_freq_range_hook) (unsigned int fq_time, double fq_band_start, double fq_band_end));

		virtual ~fq_sink();

	private:
		virtual int general_work(
			int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);

		int process_vector(float *input);

		void process_signals(double *freqs, float *bands1, float *bands2);

		bool try_signal(double min, double max);

		void rearrange(float *bands, double *freqs, double centre, double bandwidth);

		void get_bands(float *powers, float *bands, unsigned int bandwidth);

		void zero_buffer();

		set<double> m_signals;
		osmosdr_source_c_sptr m_source;
		osmosdr_source_c_sptr m_source_init;
		float *m_buffer;
		unsigned int m_vector_length;
		unsigned int m_count;
		unsigned int m_wait_count;
		unsigned int m_avg_size;
		double m_step;
		double m_freq_cur;
		double m_freq_start;
		double m_freq_end;
		double m_sample_rate;
		double m_bandwidth_fine;
		double m_bandwidth_coarse;
		double m_threshold;
		double m_spread;
		double m_time;
		time_t m_start_time;
		int (*m_fq_signal_hook) (unsigned int fq_time, double fq_freq, double fq_width, float fq_power, float fq_diff, double fq_band_start, double fq_band_end);
		int (*m_fq_freq_range_hook) (unsigned int fq_time, double fq_band_start, double fq_band_end);
};

/* Shared pointer thing gnuradio is fond of */
typedef boost::shared_ptr<fq_sink> fq_sink_sptr;

fq_sink_sptr make_fq_sink(
	osmosdr_source_c_sptr source,
	unsigned int vector_length,
	double freq_start,
	double freq_end,
	double sample_rate,
	double bandwidth_fine,
	double bandwidth_coarse,
	double step,
	unsigned int avg_size,
	double spread,
	double threshold,
	double ptime,
	int (*fq_signal_hook) (unsigned int fq_time, double fq_freq, double fq_width, float fq_power, float fq_diff, double fq_band_start, double fq_band_end),
	int (*fq_freq_range_hook) (unsigned int fq_time, double fq_band_start, double fq_band_end));

#endif
