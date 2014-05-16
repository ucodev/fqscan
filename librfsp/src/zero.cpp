/*
 *	librfsp - RF Signal Processing Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: zero.cpp
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

#include "../include/polynom.hpp"
#include "../include/mconsts.hpp"

using namespace std;

double zero_gegenbauer_largest(double n, double l, double e) {
	/* Newton-Raphson method, controlled by 'e' */
	if (l == 0.0)
		return cos(MCONST_PI / (2.0 * (double) n));

	double y_cur = sqrt((n * n) + (2.0 * n * l) - (2.0 * l) - 1.0) / (n + l);
	double y_next = 0.0;

	do {
		y_next = y_cur - (polynom_gegenbauer_can(l, y_cur, n) / (2.0 * l * polynom_gegenbauer_can(l + 1.0, y_cur, n - 1.0)));

		if ((y_next - y_cur) <= e)
			return y_next;


		y_cur = y_next;
	} while (1);

	return 0;
}

double zero_chebyshev_largest(double n, double a) {
	return cosh((1.0 / (n - 1.0)) * acosh(a));
}

