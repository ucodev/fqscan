/*
 *	librfsp - RF Signal Processing Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: rfsp_window.hpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 *
 *	Last Update: 2014/03/30
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


#ifndef LIBRFSP_WINDOW_HPP
#define LIBRFSP_WINDOW_HPP

#include <vector>

using namespace std;

class rfsp_window {
	public:
		rfsp_window();

		virtual ~rfsp_window();

		vector<float> hr_rectangular(size_t n);
		vector<float> mr_hann(size_t n);
		vector<float> mr_hamming(size_t n);
		vector<float> mhr_tukey(size_t n);
		vector<float> mhr_tukey(size_t n, double a);
		vector<float> mr_cosine(size_t n);
		vector<float> mr_lanczos(size_t n);
		vector<float> mr_triangular(size_t n);
		vector<float> mr_bartlett(size_t n);
		vector<float> mr_bartlett_hann(size_t n);
		vector<float> mr_bartlett_hann(size_t n, double a0, double a1, double a2);
		vector<float> mr_blackman(size_t n);
		vector<float> mr_blackman(size_t n, double a0, double a1, double a2);
		vector<float> mr_kaiser(size_t n);
		vector<float> mr_kaiser(size_t n, double a);
		vector<float> mr_hann_poisson(size_t n);
		vector<float> mr_hann_poisson(size_t n, double a);
		vector<float> mr_planck_taper(size_t n);
		vector<float> mr_planck_taper(size_t n, double e);
		vector<float> mr_planck_bessel(size_t n);
		vector<float> mr_planck_bessel(size_t n, double e, double a);
		vector<float> mr_gauss(size_t n);
		vector<float> mr_gauss(size_t n, double a);
		vector<float> mr_ultraspherical(size_t n, double x0, double a);
		vector<float> mr_dolph_chebyshev(size_t n);
		vector<float> mr_dolph_chebyshev(size_t n, double attn);
		vector<float> mr_saramaki(size_t n);
		vector<float> lr_nuttall(size_t n);
		vector<float> lr_blackman_harris(size_t n);
		vector<float> lr_blackman_nuttall(size_t n);
		vector<float> lr_flat_top(size_t n);
		vector<float> lr_generic_4th(size_t n, double a0, double a1, double a2, double a3);
		vector<float> lr_generic_5th(size_t n, double a0, double a1, double a2, double a3, double a4);
};

#endif
