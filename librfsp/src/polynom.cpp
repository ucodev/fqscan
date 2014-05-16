/*
 *	librfsp - RF Signal Processing Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: polynom.cpp
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

double polynom_gegenbauer_ca0(double a, double x) {
	return 1;
}

double polynom_gegenbauer_ca1(double a, double x) {
	return 2.0 * a * x;
}

double polynom_gegenbauer_ca2(double a, double x) {
	return -a + (((2 * a) * (1 + a)) * (x * x));
}

double polynom_gegenbauer_can(double a, double x, int n) {
	if (n == 2) {
		return polynom_gegenbauer_ca2(a, x);
	} else if (n == 1) {
		return polynom_gegenbauer_ca1(a, x);
	} else if (n == 0) {
		return polynom_gegenbauer_ca0(a, x);
	}

	return (1.0 / (double) n) * ((2.0 * x) * ((double) n + a - 1.0) * polynom_gegenbauer_can(a, x, n - 1) - (double) (n + (2.0 * a) - 2.0) * polynom_gegenbauer_can(a, x, n - 2));
}

double polynom_chebyshev_1st(double n, double x) {
	return fabs(x) <= 1.0 ? cos(n * acos(x)) : cosh(n * acosh(x));
}

