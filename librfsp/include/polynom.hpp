/*
 *	librfsp - RF Signal Processing Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: polynom.hpp
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


#ifndef LIBRFSP_POLYNOM_HPP
#define LIBRFSP_POLYNOM_HPP

double polynom_gegenbauer_ca0(double a, double x);
double polynom_gegenbauer_ca1(double a, double x);
double polynom_gegenbauer_can(double a, double x, int  n);
double polynom_chebyshev_1st(double n, double x);

#endif

