/*
 *	librfsp - RF Signal Processing Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: rfsp_window.cpp
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


#include <cmath>
#include <vector>

#include <boost/math/special_functions/bessel.hpp>

#include "rfsp_window.hpp"
#include "mconsts.hpp"
#include "polynom.hpp"
#include "zero.hpp"

using namespace std;

rfsp_window::rfsp_window() {
	return;
}

rfsp_window::~rfsp_window() {
	return;
}

vector<float> rfsp_window::hr_rectangular(size_t n) {
	vector<float> v(n);
	
	for (unsigned int i = 0; i < n; i ++) {
		v[i] = 1;
	}

	return v;
}

vector<float> rfsp_window::mr_hann(size_t n) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = 0.5 * (1 - cos((2 * MCONST_PI * (double) i) / (double) (n - 1)));
	}

	return v;
}

vector<float> rfsp_window::mr_hamming(size_t n) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = 0.54 - (0.46 * cos((2 * MCONST_PI * (double) i) / (double) (n - 1)));
	}

	return v;
}

vector<float> rfsp_window::mhr_tukey(size_t n) {
	return mhr_tukey(n, 0.5);
}

vector<float> rfsp_window::mhr_tukey(size_t n, double a) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		if (i <= (a * (n - 1)) / 2.0) {
			v[i] = 0.5 * (1 + cos(MCONST_PI * (double) (((2 * i) / (double) (a * (double) (n - 1))) - 1)));
		} else if (i <= ((n - 1) * (1 - (a / 2.0)))) {
			v[i] = 1;
		} else { /* it's, for sure, greater than ((n - 1) * (1 - (a / 2.0))) and lesser than (n - 1) */
			v[i] = 0.5 * (1 + cos(MCONST_PI * (double) (((2 * i) / (double) (a * (double) (n - 1))) - (2.0 / a) + 1)));
		}
	}

	return v;
}

vector<float> rfsp_window::mr_cosine(size_t n) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = sin((MCONST_PI * (double) i) / (double) (n - 1));
	}

	return v;
}

vector<float> rfsp_window::mr_lanczos(size_t n) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = ((2 * (double) i) / (double) (n - 1)) - 1;

		if (v[i] != 0)
			v[i] = sin(MCONST_PI * v[i]) / (MCONST_PI * v[i]);
	}

	return v;
}

vector<float> rfsp_window::mr_triangular(size_t n) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = (2.0 / (double) (n + 1.0)) * (((double) (n + 1) / 2.0) - fabs(i - ((double) (n - 1.0) / 2.0)));
	}

	return v;
}

vector<float> rfsp_window::mr_bartlett(size_t n) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = (2.0 / (double) (n - 1.0)) * (((double) (n - 1) / 2.0) - fabs(i - ((double) (n - 1.0) / 2.0)));
	}

	return v;
}

vector<float> rfsp_window::mr_bartlett_hann(size_t n) {
	return mr_bartlett_hann(n, 0.62, 0.48, 0.38);
}

vector<float> rfsp_window::mr_bartlett_hann(size_t n, double a0, double a1, double a2) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = a0 - (a1 * fabs((((double) i / (double) (n - 1)) - 0.5)) - (a2 * cos((2.0 * MCONST_PI) / (double) (n - 1))));
	}

	return v;
}

vector<float> rfsp_window::mr_blackman(size_t n) {
	return mr_blackman(n, 7938.0 / 18608.0, 9240.0 / 18608.0, 1430.0 / 18608.0);
}

vector<float> rfsp_window::mr_blackman(size_t n, double a0, double a1, double a2) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = a0 - (a1 * cos((2.0 * MCONST_PI * (double) i) / (double) (n - 1))) + (a2 * cos((4.0 * MCONST_PI * (double) i) / (double) (n - 1)));
	}

	return v;
}


vector<float> rfsp_window::mr_kaiser(size_t n) {
	return mr_kaiser(n, 3.0);
}

vector<float> rfsp_window::mr_kaiser(size_t n, double a) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = boost::math::cyl_bessel_i(0, MCONST_PI * a * sqrt(1 - pow((((double) (2 * i) / (double ) (n - 1)) - 1.0), 2))) / boost::math::cyl_bessel_i(0, MCONST_PI * a);
	}

	return v;
}

vector<float> rfsp_window::mr_hann_poisson(size_t n) {
	return mr_hann_poisson(n, 2.0);
}

vector<float> rfsp_window::mr_hann_poisson(size_t n, double a) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = 0.5 * (1 - cos((2 * MCONST_PI * (double) i) / (double) (n - 1))) * exp((-a * fabs((double) (n - 1) - (double) (2 * n))) / (double) (n - 1));
	}

	return v;
}

vector<float> rfsp_window::mr_planck_taper(size_t n) {
	return mr_planck_taper(n, 0.1);
}

vector<float> rfsp_window::mr_planck_taper(size_t n, double e) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		if ((double) i < (e * (double) (n - 1.0))) {
			v[i] = 1.0 / (exp((2.0 * e) * ((1.0 / (1.0 + (double) (2.0 * i) / (double) (n - 1))) + (1.0 / (1.0 - (2.0 * e) + (double) (2.0 * i) / (double) (n - 1))))) + 1.0);
		} else if ((double) i < (double) (((1.0 - e) * (double) (n - 1.0)))) {
			v[i] = 1.0;
		} else if (i <= (n - 1)) {
			v[i] = 1.0 / (exp((2.0 * e) * ((1.0 / (1.0 - (double) (2.0 * i) / (double) (n - 1))) + (1.0 / (1.0 - (2.0 * e) - (double) (2.0 * i) / (double) (n - 1))))) + 1.0);
		} else {
			// Unreachable.
			v[i] = 0.0;
		}
	}

	return v;
}

vector<float> rfsp_window::mr_planck_bessel(size_t n) {
	vector<float> v1;
	vector<float> v2;

	v1 = mr_planck_taper(n);
	v2 = mr_kaiser(n);

	for (unsigned int i = 0; i < n; i ++) {
		v1[i] *= v2[i];
	}

	return v1;
}

vector<float> rfsp_window::mr_planck_bessel(size_t n, double e, double a) {
	vector<float> v1;
	vector<float> v2;

	v1 = mr_planck_taper(n, e);
	v2 = mr_kaiser(n, a);

	for (unsigned int i = 0; i < n; i ++) {
		v1[i] *= v2[i];
	}

	return v1;
}

vector<float> rfsp_window::mr_gauss(size_t n) {
	return mr_gauss(n, 0.4);
}

vector<float> rfsp_window::mr_gauss(size_t n, double a) {
	vector<float> v(n);

	// a <= 0.5

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = exp(-0.5 * pow(((i - (double) (n - 1) / 2.0) / (a * (double) (n - 1) / 2.0)), 2));
	}

	return v;
}

vector<float> rfsp_window::mr_ultraspherical(size_t n, double x0, double a) {
	vector<float> v(n);
	vector<float> v_ret(n);

	double polynom_x0 = polynom_gegenbauer_can(a, x0, n - 1);

	for (unsigned int i = 0; i < n; i ++) {
		double polynom_sum = 0.0;

		for (unsigned int k = 1; k <= ((n - 1) / 2); k ++) {
			polynom_sum += (polynom_gegenbauer_can(a, x0 * cos(((double) k * MCONST_PI) / (double) n), n - 1) * cos((2.0 * (double) i * MCONST_PI * (double) k) / (double) n));
		}

		v[i] = (1.0 / (double) n) * (polynom_x0 + polynom_sum);
	}

	/* Normalize */
	for (unsigned int i = 0; i < n; i ++) {
		v_ret[i] = (v[i] / v[n / 2.0]);
	}

	return v_ret;
}

vector<float> rfsp_window::mr_dolph_chebyshev(size_t n) {
	return mr_dolph_chebyshev(n, -60.0);
}

vector<float> rfsp_window::mr_dolph_chebyshev(size_t n, double attn) {
	vector<float> v(n);

	// attn is the attenuation of side-lobes, in dB
	double a = pow(10, attn / -20.0);
	double x0 = zero_chebyshev_largest(n, a);
	double peak = -100000.0;
	double hl = ((((double) n) - 1.0) / 2.0) + ((n % 2) ? 0 : 0.5);	// Half Length

	/* NOTE:
	 *
	 * Since 'i' must satisfy the expression -n/2 <= i <= n/2, we'll subtract 'hl' from 'i' (((double) i) - hl) in the
	 * polynom summation loop.
	 */
	for (unsigned int i = 0; i <= (n / 2); i ++) {
		double polynom_sum = 0.0;

		for (unsigned int k = 1; k <= hl; k ++) {
			polynom_sum += polynom_chebyshev_1st(((double) n) - 1.0, x0 * cos(((double) k * MCONST_PI) / (double) n)) * cos((2.0 * MCONST_PI * (((double) i) - hl) * (double) k) / (double) n);
		}

		v[n - i - 1] = v[i] = (1.0 / (double) n) * (a + (2.0 * polynom_sum));

		if (v[i] > peak)
			peak = v[i];
	}

	/* Normalize */
	for (unsigned int i = 0; i < n; i ++)
		v[i] /= peak;

	return v;
}

vector<float> rfsp_window::mr_saramaki(size_t n) {
	vector<float> v(n);
	vector<float> v_ret(n);

	double e = 0.000001;
	double a = 1.0; // An ultraspherical window with u == 1 is a Saramäki window
	double x0 = zero_gegenbauer_largest(n, a, e);

	double polynom_x0 = polynom_gegenbauer_can(a, x0, n - 1);

	for (unsigned int i = 0; i < n; i ++) {
		double polynom_sum = 0.0;

		for (unsigned int k = 1; k <= ((n - 1) / 2); k ++) {
			polynom_sum += (polynom_gegenbauer_can(a, x0 * cos(((double) k * MCONST_PI) / (double) n), n - 1) * cos((2.0 * (double) i * MCONST_PI * (double) k) / (double) n));
		}

		v[i] = (1.0 / (double) n) * (polynom_x0 + polynom_sum);
	}

	/* Normalize */
	for (unsigned int i = 0; i < n; i ++) {
		v_ret[i] = (v[i] / v[n / 2.0]);
	}

	return v_ret;
}

vector<float> rfsp_window::lr_nuttall(size_t n) {
	return lr_generic_4th(n, 0.355768, 0.487396, 0.144232, 0.012604);
}

vector<float> rfsp_window::lr_blackman_harris(size_t n) {
	return lr_generic_4th(n, 0.35875, 0.48829, 0.14128, 0.01168);
}

vector<float> rfsp_window::lr_blackman_nuttall(size_t n) {
	return lr_generic_4th(n, 0.3635819, 0.4891775, 0.1365995, 0.0106411);
}

vector<float> rfsp_window::lr_flat_top(size_t n) {
	return lr_generic_5th(n, 1.0, 1.93, 1.29, 0.388, 0.032);
}

vector<float> rfsp_window::lr_generic_4th(size_t n, double a0, double a1, double a2, double a3) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = a0 - (a1 * cos((2.0 * MCONST_PI * (double) i) / (double) (n - 1))) + (a2 * cos((4.0 * MCONST_PI * (double) i) / (double) (n - 1))) - (a3 * cos((6.0 * MCONST_PI * (double) i) / (double) (n - 1)));
	}

	return v;
}

vector<float> rfsp_window::lr_generic_5th(size_t n, double a0, double a1, double a2, double a3, double a4) {
	vector<float> v(n);

	for (unsigned int i = 0; i < n; i ++) {
		v[i] = a0 - (a1 * cos((2.0 * MCONST_PI * (double) i) / (double) (n - 1))) + (a2 * cos((4.0 * MCONST_PI * (double) i) / (double) (n - 1))) - (a3 * cos((6.0 * MCONST_PI * (double) i) / (double) (n - 1))) + (a4 * cos((8.0 * MCONST_PI * (double) i) / (double) (n - 1)));
	}

	return v;
}

