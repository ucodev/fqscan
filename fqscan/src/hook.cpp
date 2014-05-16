/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: hook.cpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 *
 *	Last Update: 2014/03/28
 * 
 *	This project links to, at least, the following libraries, licensed under, at least, one GPL version:
 *	  - libfqscan - http://libfqscan.ucodev.org/
 *	  - librfsp - http://librfsp.ucodev.org/
 *	  - gnuradio - http://gnuradio.org/
 *	  - osmocomsdr - http://sdr.osmocom.org/trac/
 *	  - libmysqlclient - http://www.mysql.com/
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


#include <iostream>

#include "runtime.hpp"

int fq_signal_hook(unsigned int fq_time, double fq_freq, double fq_width, float fq_power, float fq_diff, double fq_band_start, double fq_band_end) {
	cout << "[" << fq_time << ": Found signal] - Freq: " << fq_freq << " kHz, Width: " << fq_width << " kHz, Peak: " << fq_power << " dB, Relative Amplitude (from median static): " << fq_diff << " dB (Current band: " << fq_band_start << " kHz - " << fq_band_end << " kHz)\n";

	if (run.arg.use_mysql) {
		run.sql <<
			"INSERT INTO scan_data (" \
			"setup_id, freq, width, power, static_diff, range_freq_start, range_freq_end, scan_ts, count) " \
			"VALUES (" <<
			run.setup_id << "," <<
			fq_freq << "," <<
			fq_width << "," <<
			fq_power << "," <<
			fq_diff << "," <<
			fq_band_start << "," <<
			fq_band_end << "," <<
			fq_time << "," <<
			run.scan_count << ")" <<
			endl;

		run.sql.store_result();
		run.sql.free_result();
	}

	return 0;
}

int fq_freq_range_hook(unsigned int fq_time, double fq_band_start, double fq_band_end) {
	cout << "[" << fq_time <<
		": New Scanning Frequency Tuned] - " << fq_band_start <<
		" - " << fq_band_end <<
		" Hz" << endl;

	if (run.arg.use_mysql) {
		run.sql <<
			"INSERT INTO state (" \
			"setup_id, running, freq_range_start, freq_range_end) " \
			"VALUES (" <<
			run.setup_id << "," <<
			"1" << "," <<
			fq_band_start << "," <<
			fq_band_end << ")" <<
			endl;

		run.sql.store_result();
		run.sql.free_result();
	}

	return 0;
}

