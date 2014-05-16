/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: config.hpp
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


#ifndef FQSCAN_CONFIG_H_
#define FQSCAN_CONFIG_H_

#include <libfqscan/fq_filters.hpp>

#define CONFIG_VERSION				"0.31"
#define CONFIG_MAX_QUERY_SIZE			2048
#define CONFIG_RTLSDR_SUPPORT			1

/* Default values for scanner arguments */
#define	CONFIG_DEFAULT_ARG_AVG_SIZE		1000
#define CONFIG_DEFAULT_ARG_BANDWIDTH_FINE	25000.0
#define CONFIG_DEFAULT_ARG_BANDWIDTH_COARSE	(CONFIG_DEFAULT_ARG_BANDWIDTH_FINE * 8)
#define CONFIG_DEFAULT_ARG_SPREAD		50000.0
#define CONFIG_DEFAULT_ARG_THRESHOLD		3.0
#define CONFIG_DEFAULT_ARG_FREQ_START		87000000.0
#define CONFIG_DEFAULT_ARG_FREQ_END		108000000.0
#define CONFIG_DEFAULT_ARG_SAMPLE_RATE		2000000.0
#define CONFIG_DEFAULT_ARG_FFT_WIDTH		1000.0
#define CONFIG_DEFAULT_ARG_STEP			(CONFIG_DEFAULT_ARG_SAMPLE_RATE / 4.0)
#define CONFIG_DEFAULT_ARG_PTIME		0.0
#define CONFIG_DEFAULT_ARG_FREQ_CORRECTION	0.0
#define CONFIG_DEFAULT_ARG_GAIN			10.0
#define CONFIG_DEFAULT_ARG_IF_GAIN		20.0
#define CONFIG_DEFAULT_ARG_FREQ_SHIFT		0.0
#define CONFIG_DEFAULT_ARG_NR_SCANS		1
#define CONFIG_DEFAULT_ARG_FILTER		FILTER_DOLPH_CHEBYSHEV
#define CONFIG_DEFAULT_ARG_FILTER_ARGS		false
#define CONFIG_DEFAULT_ARG_FILTER_A1		-1
#define CONFIG_DEFAULT_ARG_FILTER_A2		-1
#define CONFIG_DEFAULT_ARG_FILTER_A3		-1
#define CONFIG_DEFAULT_ARG_FILTER_A4		-1
#define CONFIG_DEFAULT_ARG_FILTER_A5		-1
#define CONFIG_DEFAULT_ARG_USE_MYSQL		false
#define CONFIG_DEFAULT_ARG_PROC_DAEMONIZE	false
#define CONFIG_DEFAULT_ARG_PROC_OUTFILE		NULL

#define CONFIG_ARGS_USAGE \
	{NULL, 0, NULL, 0, "Scanner options:", 0}, \
	{"avg-size", 'a', "COUNT", 0, "Average over COUNT samples", 1}, \
	{"bandwidth-fine", 'f', "FREQ", 0, "Bandwidth of the fine window in kHz", 1}, \
	{"bandwidth-coarse", 'c', "FREQ", 0, "Bandwidth of the coarse window in kHz", 1}, \
	{"spread", 's', "FREQ", 0, "Minimum frequency between detected signals", 1}, \
	{"threshold", 't', "POWER", 0, "Threshold for the difference between the coarse and fine filtered signals in dB", 1}, \
	{"freq-start", 'x', "FREQ", 0, "Start frequency in MHz", 1}, \
	{"freq-end", 'y', "FREQ", 0, "End frequency in MHz", 1}, \
	{"sample-rate", 'r', "RATE", 0, "Samplerate in Msamples/s", 1}, \
	{"fft-width", 'w', "COUNT", 0, "Width of FFT in samples", 1}, \
	{"step", 'z', "FREQ", 0, "Increment step in MHz", 1}, \
	{"time", 'p', "TIME", 0, "Time in seconds to scan on each frequency", 1}, \
	{"shift", 'i', "FREQ", 0, "Shift the frequency FREQ kHz (+)up/(-)down", 1}, \
	{"freq-correction", 'm', "PPM", 0, "Frequency correction in ppm.", 1}, \
	{"gain", 'g', "POWER", 0, "Gain in dB.", 1}, \
	{"if-gain", 'n', "POWER", 0, "IF Gain in dB.", 1}, \
	{"nr-scans", 'k', "COUNT", 0, "Perform COUNT scans. Use 0 for infinite loop. The default is 1 scan.", 1}, \
	{NULL, 0, NULL, 0, "High-Resolution Filter options:", 2}, \
	{"filter-rectangular", 3001, NULL, 0, "Use a rectangular filter", 3}, \
	{NULL, 0, NULL, 0, "Medium-Resolution Filter options:", 4}, \
	{"filter-hann", 5002, NULL, 0, "Use a Hann filter", 5}, \
	{"filter-hamming", 5003, NULL, 0, "Use a Hamming filter", 5}, \
	{"filter-tukey", 5004, NULL, 0, "Use a Tukey filter with default params", 5}, \
	{"filter-tukey-custom", 5005, NULL, 0, "Use a Tukey filter with custom params (See --filter-param-1 flag)", 5}, \
	{"filter-cosine", 5006, NULL, 0, "Use a Cosine filter", 5}, \
	{"filter-triangular", 5007, NULL, 0, "Use a Triangular filter", 5}, \
	{"filter-bartlett", 5008, NULL, 0, "Use a Bartlett filter", 5}, \
	{"filter-bartlett-hann", 5009, NULL, 0, "Use a Bartlett-Hann filter with default params", 5}, \
	{"filter-bartlett-hann-custom", 5010, NULL, 0, "Use a Bartlett-Hann filter with custom params (See --filter-param-1 to 3 flags)", 5}, \
	{"filter-blackman", 5011, NULL, 0, "Use a Blackman filter with default params (default filter)", 5}, \
	{"filter-blackman-custom", 5012, NULL, 0, "Use a Blackman filter with custom params (See --filter-param-1 to 3 flags)", 5}, \
	{"filter-kaiser", 5013, NULL, 0, "Use a Kaiser filter with default params", 5}, \
	{"filter-kaiser-custom", 5014, NULL, 0, "Use a Kaiser filter with custom params (See --filter-param-1 flag)", 5}, \
	{"filter-hann-poisson", 5015, NULL, 0, "Use a Hann-Poisson filter with default params", 5}, \
	{"filter-hann-poisson-custom", 5016, NULL, 0, "Use a Hann-Poisson filter with custom params (See --filter-param-1 flag)", 5}, \
	{"filter-planck-taper", 5017, NULL, 0, "Use a Planck-taper filter with default params", 5}, \
	{"filter-planck-taper-custom", 5018, NULL, 0, "Use a Planck-taper filter with custom params (See --filter-param-1 flag)", 5}, \
	{"filter-planck-bessel", 5019, NULL, 0, "Use a Planck-Bessel filter with default params", 5}, \
	{"filter-planck-bessel-custom", 5020, NULL, 0, "Use a Planck-Bessel filter with custom params (See --filter-param-1 to 2 flags)", 5}, \
	{"filter-gauss", 5021, NULL, 0, "Use a Gauss filter with default params", 5}, \
	{"filter-gauss-custom", 5022, NULL, 0, "Use a Gauss filter with custom params (See --filter-param-1 flag)", 5}, \
	{"filter-ultraspherical", 5023, NULL, 0, "Use a Ultraspherical filter with default params", 5}, \
	{"filter-ultraspherical-custom", 5024, NULL, 0, "Use a Ultraspherical filter with custom params (See --filter-param-1 to 3 flags)", 5}, \
	{"filter-dolph-chebyshev", 5025, NULL, 0, "Use a Dolph-Chebyshev filter with default params", 5}, \
	{"filter-dolph-chebyshev-custom", 5026, NULL, 0, "Use a Dolph-Chebyshev filter with custom params (See --filter-param-1 flag)", 5}, \
	{"filter-saramaki", 5027, NULL, 0, "Use a Saramäki filter", 5}, \
	{"filter-lanczos", 5028, NULL, 0, "Use a Lanczos filter", 5}, \
	{NULL, 0, NULL, 0, "Low-Resolution Filter options:", 6}, \
	{"filter-nuttall", 7029, NULL, 0, "Use a Nuttall filter", 7}, \
	{"filter-blackman-harris", 7030, NULL, 0, "Use a Blackman-Harris filter", 7}, \
	{"filter-blackman-nuttall", 7031, NULL, 0, "Use a Blackman-Nuttall filter", 7}, \
	{"filter-flat-top", 7032, NULL, 0, "Use a Flat-Top filter", 7}, \
	{"filter-generic-4th", 7033, NULL, 0, "Use a Generic Low Resolution filter of 4th degree (See --filter-param-1 to 4)", 7}, \
	{"filter-generic-5th", 7034, NULL, 0, "Use a Generic Low Resolution filter of 5th degree (See --filter-param-1 to 5)", 7}, \
	{NULL, 0, NULL, 0, "Filter Param options:", 8}, \
	{"filter-param-1", 9001, "ARG1", 0, "First parameter for the specified custom filter", 9}, \
	{"filter-param-2", 9002, "ARG2", 0, "Second parameter for the specified custom filter", 9}, \
	{"filter-param-3", 9003, "ARG3", 0, "Third parameter for the specified custom filter", 9}, \
	{"filter-param-4", 9004, "ARG4", 0, "Fourth parameter for the specified custom filter", 9}, \
	{"filter-param-5", 9005, "ARG4", 0, "Fourth parameter for the specified custom filter", 9}, \
	{NULL, 0, NULL, 0, "Database options:", 10}, \
	{"use-mysql", 11001, NULL, 0, "Record data in MySQL", 11}, \
	{"mysql-host", 11002, "HOST", 0, "MySQL Hostname", 11}, \
	{"mysql-port", 11003, "PORT", 0, "MySQL Port", 11}, \
	{"mysql-user", 11004, "PASSWD", 0, "MySQL Username", 11}, \
	{"mysql-passwd", 11005, "PASSWD", 0, "MySQL Password", 11}, \
	{"mysql-db", 11006, "DATABASE", 0, "MySQL Database", 11}, \
	{NULL, 0, NULL, 0, "Process Control options:", 12}, \
	{"daemonize", 'd', NULL, 0, "Daemonize the process (useful with --nr-scans=0)", 13}, \
	{"output-file", 'o', "FILE", 0, "Print results to FILE (default is Standard Output)", 13}, \
	{0}


/* Author */
#define CONFIG_AUTHOR_NAME		"Pedro A. Hortas"
#define CONFIG_AUTHOR_EMAIL		"pah@ucodev.org"


/* Licensing */
#define CONFIG_LICENSE_DISCLAIMER \
	"fqscan " CONFIG_VERSION " - RF Signal Scanner and Plotter\n" \
	"Copyright (C) 2014 " CONFIG_AUTHOR_NAME " " CONFIG_AUTHOR_EMAIL "\n" \
	"\n" \
	"This software links, at least, to the following libraries:\n" \
	"\t - libfqscan - http://libfqscan.ucodev.org/\n" \
	"\t - librfsp - http://librfsp.ucodev.org/\n" \
	"\t - gnuradio - http://gnuradio.org/\n" \
	"\t - osmocomsdr - http://sdr.osmocom.org/trac/\n" \
	"\t - libmysqlclient - http://www.mysql.com/\n" \
	"\n" \
	"This program is free software: you can redistribute it and/or modify\n" \
	"it under the terms of the GNU General Public License as published by\n" \
	"the Free Software Foundation, either version 3 of the License, or\n" \
	"(at your option) any later version.\n" \
	"\n" \
	"This program is distributed in the hope that it will be useful,\n" \
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
	"GNU General Public License for more details.\n" \
	"\n" \
	"You should have received a copy of the GNU General Public License\n" \
	"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"

#endif
