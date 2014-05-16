/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: args.hpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 * 
 *	Compatibility:
 *		- The short fqscan command line options are backward compatible with gr-scan v2012082301 except that:
 *		   + All magnitudes of all values from user input were normalized (freq is always MHz, power is dB, etc)
 *
 *		- The default values of the backward compatible options were retained from gr-scan v2012082301.
 *
 *		- Some extra command line options were added to support new implemeted features:
 *		   + Added support for frequency correction (ppm)
 *		   + Added support for Gain param
 *		   + Added support for IF Gain param
 *		   + Implemented support for multiple filters (window functions)
 *		   + Implemented support for MySQL
 *		   + Implemented support for frequency shifting, allowing the transparent use of up/down converters
 *		   + Implemented support for daemon mode
 *		   + Implemented support for output files
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


#ifndef FQSCAN_ARGS_HPP
#define FQSCAN_ARGS_HPP

#include <argp.h>

#include <libfqscan/fq_filters.hpp>

class args {
	public:
		void pre_process(int argc, char **argv);
		void post_process();

		unsigned int avg_size;
		double bandwidth_fine;
		double bandwidth_coarse;
		double spread;
		double threshold;
		double freq_start;
		double freq_end;
		double sample_rate;
		double fft_width;
		double step;
		double ptime;
		double freq_correction;
		double gain;
		double if_gain;
		double freq_shift;
		unsigned long nr_scans;
		FILTER filter;
		bool filter_args;
		double filter_a1;
		double filter_a2;
		double filter_a3;
		double filter_a4;
		double filter_a5;

		bool use_mysql;
		char *mysql_host;
		unsigned short mysql_port;
		char *mysql_user;
		char *mysql_pass;
		char *mysql_db;

		bool proc_daemonize;
		char *proc_outfile;

	private:
		error_t process_option(int key, char *arg, struct argp_state *state);

		static error_t process_option_static_wrapper(int key, char *arg, struct argp_state *state);
};

#endif
