/*
 *	libfqscan - RF Signal Scanner Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fq_sink.cpp
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


#include <ctime>
#include <set>
#include <utility>

#include <boost/shared_ptr.hpp>

#include <gnuradio/gr_block.h>
#include <gnuradio/gr_io_signature.h>

#include <osmosdr/osmosdr_source_c.h>

#include "fq_sink.hpp"

fq_sink::fq_sink(
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
	int (*fq_signal_hook) (
		unsigned int fq_time,
		double fq_freq,
		double fq_width,
		float fq_power,
		float fq_diff,
		double fq_band_start,
		double fq_band_end),
	int (*fq_freq_range_hook) (
		unsigned int fq_time,
		double fq_band_start,
		double fq_band_end)
	) :
	gr_block ("scanner_sink",
		gr_make_io_signature (1, 1, sizeof (float) * vector_length),
		gr_make_io_signature (0, 0, 0)),
	m_source(source), // We need the source in order to be able to control it
	m_buffer(new float[vector_length]), // buffer into which we accumulate the total for averaging
	m_vector_length(vector_length), // size of the FFT
	m_count(0), // number of FFTs totalled in the buffer
	m_wait_count(0), // number of times we've listenned on this frequency
	m_avg_size(avg_size), // the number of FFTs we should average over
	m_step(step), // the amount by which the frequency shold be incremented
	m_freq_cur(freq_start), // current frequency
	m_freq_start(freq_start), // start frequency (and then current frequency)
	m_freq_end(freq_end), // end frequency
	m_sample_rate(sample_rate), // samples per second
	m_bandwidth_fine(bandwidth_fine), // fine window (band)width
	m_bandwidth_coarse(bandwidth_coarse), // coarse window (band)width
	m_threshold(threshold), // threshold in dB for discovery
	m_spread(spread), // minimum distance between radio signals (overlapping scans might produce slightly different frequencies)
	m_time(ptime), // the amount of time to listen on the same frequency for
	m_start_time(time(0)), // the start time of the scan (useful for logging/reporting/monitoring)
	m_fq_signal_hook(fq_signal_hook),
	m_fq_freq_range_hook(fq_freq_range_hook)
{
	zero_buffer();
}
		
fq_sink::~fq_sink() {
	wait();
	stop();
	delete []m_buffer; //delete the buffer
}

int fq_sink::general_work(
	int noutput_items,
	gr_vector_int &ninput_items,
	gr_vector_const_void_star &input_items,
	gr_vector_void_star &output_items)
{
	int ret = 0;

	for (int i = 0; i < ninput_items[0]; i ++) {
		ret = process_vector(((float *) input_items[0]) + i * m_vector_length);

		if (ret == WORK_DONE)
			break;
	}
			
	consume_each(ninput_items[0]);

	if (ret == WORK_DONE) {
		/* Leave the object in its initial state */
		m_signals.clear();
		m_count = 0;
		m_wait_count = 0;
		m_start_time = time(0);
		m_freq_cur = m_freq_start;
		m_source->set_center_freq(m_freq_start);
		zero_buffer();

		/* We're done */
		return WORK_DONE;
	}

	return ret;
}
		
int fq_sink::process_vector(float *input) {
	// Add the FFT to the total
	for (unsigned int i = 0; i < m_vector_length; i ++) {
		m_buffer[i] += input[i];
	}

	m_count ++; // increment the total

	if (m_avg_size == m_count) { //we've averaged over the number we intended to
		double freqs[m_vector_length]; //for convenience
		float bands_rate[m_vector_length]; //bands in order of frequency
		float bands_fine[m_vector_length]; //fine window bands
		float bands_coarse[m_vector_length]; //coarse window bands
				
		rearrange(bands_rate, freqs, m_freq_cur, m_sample_rate); //organise the buffer into a convenient order (saves to bands_rate)
		get_bands(bands_rate, bands_fine, m_bandwidth_fine); //apply the fine window (saves to bands_fine)
		get_bands(bands_rate, bands_coarse, m_bandwidth_coarse); //apply the coarse window (saves to bands_coarse)
		process_signals(freqs, bands_fine, bands_coarse);
				
		m_count = 0; //next time, we're starting from scratch - so note this
		zero_buffer(); //get ready to start again
				
		m_wait_count ++; //we've just done another listen

		if ((m_time / (m_sample_rate / (double) (m_vector_length * m_avg_size))) <= m_wait_count) { //if we should move to the next frequency
			while (true) { //keep moving to the next frequency until we get to one we can listen on (copes with holes in the tunable range)
				if (m_freq_end <= m_freq_cur) { //we reached the end!
					/* Return the WORK_DONE magic number to inform general_work() that we're done. */
					return WORK_DONE;
				}
				
				m_freq_cur += m_step; //calculate the frequency we should change to

				double actual = m_source->set_center_freq(m_freq_cur); //change frequency

				if ((m_freq_cur - actual < 10.0) && (actual - m_freq_cur < 10.0)) { //success
					break; //so stop changing frequency
				}
			}

			m_wait_count = 0; //new frequency - we've listened 0 times on it
		}
	}

	return 0;
}
		
void fq_sink::process_signals(double *freqs, float *bands_fine, float *bands_coarse) {
	/* Calculate the current time after start */
	unsigned int t = time(0) - m_start_time;
	/* hook variables */
	double fq_freq; // kHz
	double fq_width; // kHz
	float fq_power; // dB
	float fq_diff; // dB
	double fq_band_start; // kHz
	double fq_band_end; // kHz

	/* Calculate the differences between the fine and coarse window bands */
	float diffs[m_vector_length];

	/* Invoke current frequency hook */
	m_fq_freq_range_hook(t, (m_freq_cur - m_sample_rate / 2.0) / 1000.0, (m_freq_cur + m_sample_rate / 2.0) / 1000.0);

	for (unsigned int i = 0; i < m_vector_length; i ++) {
		diffs[i] = bands_fine[i] - bands_coarse[i];
	}
			
	/* Look through to find signals */
	//start with no signal found (note: diffs[0] should always be very negative because of the way the windowing function works)
	bool sig = false;
	unsigned int peak = 0;

	for (unsigned int i = 0; i < m_vector_length; i ++) {
		if (sig) { // we're already in a signal
			if (diffs[peak] < diffs[i]) { //we found a rough end to the signal
				peak = i;
			}
					
			if (diffs[i] < m_threshold) { //we're transitionning to the end
				/* look for the "start" of the signal */
				unsigned int min = peak; //scan outwards for the minimum

				while ((diffs[min] > diffs[peak] - 3.0) && (min > 0)) { //while the signal is still more than half power
					min --;
				}
						
				/* look for the "end" */
				unsigned int max = peak;

				while ((diffs[max] > diffs[peak] - 3.0) && (max < m_vector_length - 1)) {
					max ++;
				}

				sig = false; //we're now in no signal state
						
				/* Print the signal if it's a genuine hit */
				if (try_signal(freqs[max], freqs[min])) {
					/* Store in database */
					fq_freq = (freqs[max] + freqs[min]) / 2000.0; // kHz
					fq_width = (freqs[max] - freqs[min]) / 1000.0; // kHz
					fq_power = bands_fine[peak]; // dB
					fq_diff = diffs[peak]; // dB
					fq_band_start = (m_freq_cur - m_sample_rate / 2.0) / 1000.0; // kHz
					fq_band_end = (m_freq_cur + m_sample_rate / 2.0) / 1000.0; // kHz

					m_fq_signal_hook(t, fq_freq, fq_width, fq_power, fq_diff, fq_band_start, fq_band_end);
				}
			}
		} else if (diffs[i] >= m_threshold) { // we found a signal!
			peak = i;
			sig = true;
		}
	}
}

bool fq_sink::try_signal(double min, double max) {
	double mid = (min + max) / 2.0; //calculate the midpoint of the signal

	/* check to see if the signal is too close to the centre frequency (a signal often erroniously appears there) */
	if (((mid - m_freq_cur) < m_spread) && ((m_freq_cur - mid) < m_spread)) {
		return false; //if so, this is not a genuine hit
	}
			
	/* check to see if the signal is close to any other (the same signal often appears with a slightly different centre frequency) */
	for (double signal: m_signals) {
		if (((mid - signal) < m_spread) && ((signal - mid) < m_spread)) { //tpo close
			return false; //if so, this is not a genuine hit
		}
	}
			
	/* Genuine hit!:D */
	m_signals.insert(mid); //add to the set of signals

	return true; //genuine hit
}
		
void fq_sink::rearrange(float *bands, double *freqs, double centre, double bandwidth) {
	double samplewidth = bandwidth / (double) m_vector_length;

	for (unsigned int i = 0; i < m_vector_length; i ++) {
		/* FFT is arranged starting at 0 Hz at the start, rather than in the middle */
		if (i < (m_vector_length / 2)) { // lower half of the fft
			bands[i + (m_vector_length / 2)] = m_buffer[i] / (float) m_avg_size;
		} else { // upper half of the fft
			bands[i - (m_vector_length / 2)] = m_buffer[i] / (float) m_avg_size;
		}
				
		freqs[i] = centre + (i * samplewidth) - (bandwidth / 2); // calculate the frequency of this sample
	}
}
		
void fq_sink::get_bands(float *powers, float *bands, unsigned int bandwidth) {
	double samplewidth = m_sample_rate / (double) m_vector_length; // the width in Hz of each sample
	unsigned int bandwidth_samples = bandwidth / samplewidth; // the number of samples in our window

	for (unsigned int i = 0; i < m_vector_length; i ++) { // we're averaging, so start with 0
		bands[i] = 0.0;
	}
			
	for (unsigned int i = 0; i < m_vector_length; i ++) { // over the entire FFT
		// make the buffer contains the entire window
		if ((i >= (bandwidth_samples / 2)) && (i < (m_vector_length + (bandwidth_samples / 2) - bandwidth_samples))) {
			for (unsigned int j = 0; j < bandwidth_samples; j ++) { // iterate over the window for averaging
				bands[i + j - (bandwidth_samples / 2)] += powers[i] / (float) bandwidth_samples; // add this sample to the bands
			}
		}
	}
}
		
void fq_sink::zero_buffer() {
	/* writes zeros to m_buffer */
	for (unsigned int i = 0; i < m_vector_length; i ++) {
		m_buffer[i] = 0.0;
	}
}
		
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
	int (*fq_signal_hook) (
		unsigned int fq_time,
		double fq_freq,
		double fq_width,
		float fq_power,
		float fq_diff,
		double fq_band_start,
		double fq_band_end),
	int (*fq_freq_range_hook) (
		unsigned int fq_time,
		double fq_band_start,
		double fq_band_end)
)
{
	return boost::shared_ptr<fq_sink>(new fq_sink(source, vector_length, freq_start, freq_end, sample_rate, bandwidth_fine, bandwidth_coarse, step, avg_size, spread, threshold, ptime, fq_signal_hook, fq_freq_range_hook));
}

