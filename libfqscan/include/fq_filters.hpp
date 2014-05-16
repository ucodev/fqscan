/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fq_filters.hpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org) - Original Author
 *
 *	Last Update: 2014/03/28
 * 
 *	This project links to, at least, the following libraries, licensed under, at least, one GPL version:
 *	  - librfsp - http://librfsp.ucodev.org/
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


#ifndef LIBFQSCAN_FQ_FILTERS_HPP
#define LIBFQSCAN_FQ_FILTERS_HPP

enum FILTER {
	FILTER_RECTANGULAR = 1,
	FILTER_BARTLETT,
	FILTER_BARTLETT_HANN,
	FILTER_BLACKMAN,
	FILTER_COSINE,
	FILTER_DOLPH_CHEBYSHEV,
	FILTER_GAUSS,
	FILTER_HAMMING,
	FILTER_HANN,
	FILTER_HANN_POISSON,
	FILTER_KAISER,
	FILTER_PLANCK_BESSEL,
	FILTER_PLANCK_TAPER,
	FILTER_SARAMAKI,
	FILTER_LANCZOS,
	FILTER_TRIANGULAR,
	FILTER_TUKEY,
	FILTER_ULTRASPHERICAL,
	FILTER_BLACKMAN_HARRIS,
	FILTER_BLACKMAN_NUTTALL,
	FILTER_FLAT_TOP,
	FILTER_GENERIC_4TH,
	FILTER_GENERIC_5TH,
	FILTER_NUTTALL
};

#endif
