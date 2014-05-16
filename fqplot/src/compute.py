#!/usr/bin/python

#
#	fqplot - FQScan Plotter
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: compute.py
#
#	Authors:
#		- Pedro A. Hortas (pah@ucodev.org)
#
#	Last Update: 2014/03/28
# 
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#	
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#


class compute:
	def __init__(self, height, width, margin_x_tolerance, margin_y_tolerance, setup_data, scan_data):
		self.margin_x_tolerance = margin_x_tolerance
		self.margin_y_tolerance = margin_y_tolerance
		self.chart_height = height
		self.chart_width = width
		self.setup_data = setup_data
		self.scan_data = scan_data
		self.pwmax = None
		self.pwmin = None

	def band_to_pxl(self, x_range):
		scan_last_limit = (self.setup_data['freq_start'] / 1000000.0) - self.delta_x_margin_xo()
		scan_cur_range = (self.setup_data['freq_start'] / 1000000.0) - self.delta_x_margin_xo()
		delta = self.delta_x_abs()

		pxl_data = []

		while scan_cur_range < ((self.setup_data['freq_end'] / 1000000.0) + self.delta_x_margin_xoo()):
			for band in x_range:
				if scan_cur_range < band[2]:
					if scan_last_limit > band[1]:
						band_segment_start = scan_last_limit - ((self.setup_data['freq_start'] / 1000000.0) - self.delta_x_margin_xo())
					else:
						band_segment_start = band[1] - ((self.setup_data['freq_start'] / 1000000.0) - self.delta_x_margin_xo())

					if band[2] < (self.setup_data['freq_end'] / 1000000.0):
						band_segment_end = band[2] - ((self.setup_data['freq_start'] / 1000000.0) - self.delta_x_margin_xo())
					else:
						band_segment_end = delta

					if ((int(self.chart_width) / delta) * band_segment_start + 100) < (self.chart_width + 100):
						pxl_data.append((band[0], (int(self.chart_width) / delta) * band_segment_start + 100, (int(self.chart_width) / delta) * band_segment_end + 100))

					scan_last_limit = band[2]
					scan_cur_range = band[2]

					break

		return pxl_data


	def delta_x_vanilla(self):
		return (self.setup_data['freq_end'] / 1000000.0) - (self.setup_data['freq_start'] / 1000000.0)


	def delta_x_abs(self):
		return self.delta_x_vanilla() + self.delta_x_margin()


	def delta_x_margin(self):
		return self.delta_x_vanilla() * self.margin_x_tolerance


	def delta_x_margin_xo(self):
		delta_margin = self.delta_x_margin()

		if (delta_margin / 2.0) > (self.setup_data['freq_start'] / 1000000.0):
			return (self.setup_data['freq_start'] / 1000000.0) / 2.0

		return delta_margin / 2.0


	def delta_x_margin_xoo(self):
		return self.delta_x_margin() - self.delta_x_margin_xo()


	def limit_xo(self):
		return (self.setup_data['freq_start'] / 1000000.0) - self.delta_x_margin_xo()


	def limit_xoo(self):
		return (self.setup_data['freq_end'] / 1000000.0) + self.delta_x_margin_xoo()


	def power_peak(self):
		if self.pwmax != None:
			return self.pwmax

		self.pwmax = -10000.0

		for entry in self.scan_data:
			if entry['power'] > self.pwmax:
				self.pwmax = entry['power']

		return self.pwmax

	def power_min(self):
		if self.pwmin != None:
			return self.pwmin

		self.pwmin = 10000.0

		for entry in self.scan_data:
			if entry['power'] < self.pwmin:
				self.pwmin = entry['power']

		return self.pwmin

	def delta_y_vanilla(self):
		return abs(self.peak_power() - self.min_power())

	def delta_y_abs(self):
		return abs(self.limit_yoo() - self.limit_yo())

	def limit_yo(self):
		yo_limit = -80.0

		min_power = self.power_min()

		while yo_limit >= min_power:
			yo_limit -= 10.0

		return yo_limit

	def limit_yoo(self):
		return self.power_peak() + (abs(self.power_peak()) * self.margin_y_tolerance)


