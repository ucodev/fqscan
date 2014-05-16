/*
 *	librfsp - RF Signal Processing Librabry
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: zero.hpp
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


#ifndef LIBRFSP_ZERO_HPP
#define LIBRFSP_ZERO_HPP

/* Prototypes */
double zero_gegenbauer_largest(double n, double l, double e);
double zero_chebyshev_largest(double n, double a);

#endif

