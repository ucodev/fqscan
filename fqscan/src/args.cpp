/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: args.cpp
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


#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <argp.h>

#include <libfqscan/fq_filters.hpp>

#include "config.hpp"
#include "args.hpp"

using namespace std;

/* Globals */
const char *argp_program_version = CONFIG_LICENSE_DISCLAIMER;
const char *argp_program_bug_address = CONFIG_AUTHOR_EMAIL;


/* Class */
void args::pre_process(int argc, char **argv) {
	argp_option opt[] = { CONFIG_ARGS_USAGE };
	struct argp a = { opt, process_option_static_wrapper, 0, 0 };

	avg_size		= CONFIG_DEFAULT_ARG_AVG_SIZE;
	bandwidth_fine		= CONFIG_DEFAULT_ARG_BANDWIDTH_FINE;
	bandwidth_coarse	= CONFIG_DEFAULT_ARG_BANDWIDTH_COARSE;
	spread			= CONFIG_DEFAULT_ARG_SPREAD;
	threshold		= CONFIG_DEFAULT_ARG_THRESHOLD;
	freq_start		= CONFIG_DEFAULT_ARG_FREQ_START;
	freq_end		= CONFIG_DEFAULT_ARG_FREQ_END;
	sample_rate		= CONFIG_DEFAULT_ARG_SAMPLE_RATE;
	fft_width		= CONFIG_DEFAULT_ARG_FFT_WIDTH;
	step			= CONFIG_DEFAULT_ARG_STEP;
	ptime			= CONFIG_DEFAULT_ARG_PTIME;
	freq_correction		= CONFIG_DEFAULT_ARG_FREQ_CORRECTION;
	gain			= CONFIG_DEFAULT_ARG_GAIN;
	if_gain			= CONFIG_DEFAULT_ARG_IF_GAIN;
	freq_shift		= CONFIG_DEFAULT_ARG_FREQ_SHIFT;
	nr_scans		= CONFIG_DEFAULT_ARG_NR_SCANS;
	filter			= CONFIG_DEFAULT_ARG_FILTER;
	filter_args		= CONFIG_DEFAULT_ARG_FILTER_ARGS;
	filter_a1		= CONFIG_DEFAULT_ARG_FILTER_A1;
	filter_a2		= CONFIG_DEFAULT_ARG_FILTER_A2;
	filter_a3		= CONFIG_DEFAULT_ARG_FILTER_A3;
	filter_a4		= CONFIG_DEFAULT_ARG_FILTER_A4;
	filter_a5		= CONFIG_DEFAULT_ARG_FILTER_A5;
	use_mysql		= CONFIG_DEFAULT_ARG_USE_MYSQL;
	proc_daemonize		= CONFIG_DEFAULT_ARG_PROC_DAEMONIZE;
	proc_outfile		= CONFIG_DEFAULT_ARG_PROC_OUTFILE;

	argp_parse(&a, argc, argv, 0, 0, this);
}

void args::post_process() {
	/* XXX: This is a very basic test. :(
	 * TODO: Validate each custom filter arguments.
	 */
	if (filter_args && (filter_a1 == -1)) {
		cout << "Fatal: A custom filter was selected, but no filter arguments were supplied." << endl << endl;
		exit(EXIT_FAILURE);
	}
}

error_t args::process_option(int key, char *arg, struct argp_state *state) {
	switch (key) {
		case 'a':   avg_size = atoi(arg);							break;
		case 'f':   bandwidth_fine = atof(arg) * 1000000.0;					break;
		case 'c':   bandwidth_coarse = atof(arg) * 1000000.0;					break;
		case 's':   spread = atof(arg) * 1000000.0;						break;
		case 't':   threshold = atof(arg);							break;
		case 'x':   freq_start = atof(arg) * 1000000.0;						break;
		case 'y':   freq_end = atof(arg) * 1000000.0;						break;
		case 'r':   sample_rate = atof(arg) * 1000000.0;					break;
		case 'w':   fft_width = atoi(arg);							break;
		case 'z':   step = atof(arg) * 1000000.0;						break;
		case 'p':   ptime = atof(arg);								break;
		case 'i':   freq_shift = atof(arg) * 1000000.0;						break;
		case 'm':   freq_correction = atof(arg);						break;
		case 'g':   gain = atof(arg);								break;
		case 'n':   if_gain = atof(arg);							break;
		case 'k':   nr_scans = atol(arg);							break;
		case 'd':   proc_daemonize = true;							break;
		case 'o':   proc_outfile = (char *) malloc(strlen(arg) + 1); strcpy(proc_outfile, arg);	break;
		case 3001:  filter = FILTER_RECTANGULAR;						break;
		case 5002:  filter = FILTER_HANN;							break;
		case 5003:  filter = FILTER_HAMMING;							break;
		case 5004:  filter = FILTER_TUKEY;							break;
		case 5005:  filter = FILTER_TUKEY; filter_args = true;					break;
		case 5006:  filter = FILTER_COSINE;							break;
		case 5007:  filter = FILTER_TRIANGULAR;							break;
		case 5008:  filter = FILTER_BARTLETT;							break;
		case 5009:  filter = FILTER_BARTLETT_HANN;						break;
		case 5010:  filter = FILTER_BARTLETT_HANN; filter_args = true;				break;
		case 5011:  filter = FILTER_BLACKMAN;							break;
		case 5012:  filter = FILTER_BLACKMAN; filter_args = true;				break;
		case 5013:  filter = FILTER_KAISER;							break;
		case 5014:  filter = FILTER_KAISER; filter_args = true;					break;
		case 5015:  filter = FILTER_HANN_POISSON;						break;
		case 5016:  filter = FILTER_HANN_POISSON; filter_args = true;				break;
		case 5017:  filter = FILTER_PLANCK_TAPER;						break;
		case 5018:  filter = FILTER_PLANCK_TAPER; filter_args = true;				break;
		case 5019:  filter = FILTER_PLANCK_BESSEL;						break;
		case 5020:  filter = FILTER_PLANCK_BESSEL; filter_args = true;				break;
		case 5021:  filter = FILTER_GAUSS;							break;
		case 5022:  filter = FILTER_GAUSS; filter_args = true;					break;
		case 5023:  filter = FILTER_ULTRASPHERICAL;						break;
		case 5024:  filter = FILTER_ULTRASPHERICAL; filter_args = true;				break;
		case 5025:  filter = FILTER_DOLPH_CHEBYSHEV;						break;
		case 5026:  filter = FILTER_DOLPH_CHEBYSHEV; filter_args = true;			break;
		case 5027:  filter = FILTER_SARAMAKI;							break;
		case 5028:  filter = FILTER_LANCZOS;							break;
		case 7029:  filter = FILTER_NUTTALL;							break;
		case 7030:  filter = FILTER_BLACKMAN_HARRIS;						break;
		case 7031:  filter = FILTER_BLACKMAN_NUTTALL;						break;
		case 7032:  filter = FILTER_FLAT_TOP;							break;
		case 7033:  filter = FILTER_GENERIC_4TH; filter_args = true;				break;
		case 7034:  filter = FILTER_GENERIC_5TH; filter_args = true;				break;
		case 9001:  filter_a1 = atof(arg);							break;
		case 9002:  filter_a2 = atof(arg);							break;
		case 9003:  filter_a3 = atof(arg);							break;
		case 9004:  filter_a4 = atof(arg);							break;
		case 9005:  filter_a5 = atof(arg);							break;
		case 11001: use_mysql = true;								break;
		case 11002: mysql_host = (char *) malloc(strlen(arg) + 1); strcpy(mysql_host, arg);	break;
		case 11003: mysql_port = atoi(arg);							break;
		case 11004: mysql_user = (char *) malloc(strlen(arg) + 1); strcpy(mysql_user, arg);	break;
		case 11005: mysql_pass = (char *) malloc(strlen(arg) + 1); strcpy(mysql_pass, arg);	break;
		case 11006: mysql_db   = (char *) malloc(strlen(arg) + 1); strcpy(mysql_db, arg);	break;
		case ARGP_KEY_ARG: if (state->arg_num > 0) argp_usage(state);				break;
		case ARGP_KEY_END:									break;
		default:    return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

error_t args::process_option_static_wrapper(int key, char *arg, struct argp_state *state) {
	return ((args *) state->input)->process_option(key, arg, state);
}

