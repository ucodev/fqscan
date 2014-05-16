#!/usr/bin/python -O

#
#	fqplot - FQScan Plotter
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: fqchart.py
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

import math
import time

from pygracha import *
from bands import *
from compute import *
from config import *

#graphs
class fqchart:
	def __init__(self, height, width, margin_x_tolerance, margin_y_tolerance, setup_data, scan_data, state_data, bands = True, limits = True, max_values = True, cur_values = True, x_coord = True, y_coord = True, x_grid = True, y_grid = True):
		self.chart_width = width
		self.chart_height = height
		self.setup_data = setup_data
		self.scan_data = scan_data
		self.state_data = state_data
		self.compute = compute(self.chart_height, self.chart_width, margin_x_tolerance, margin_y_tolerance, setup_data, scan_data)
		self.bands = bands
		self.limits = limits
		self.max_values = max_values
		self.cur_values = cur_values
		self.x_coord = x_coord
		self.y_coord = y_coord
		self.x_grid = x_grid
		self.y_grid = y_grid
		self.xo = 0.0
		self.xoo = 0.0
		self.yo = 0.0
		self.yoo = 0.0

		y_offset = self.compute.delta_y_abs()
		x_offset = self.compute.delta_x_abs()

		self.graph = chart(x_offset, y_offset, 0, 0, self.chart_height, self.chart_width, 100, 100, [0,0,0])

		# Compute grid spacing
		y_grid_spacing = y_offset / 10.0
		x_grid_spacing = x_offset / 10.0

		# Plot axis lines
		self.graph.chart_plot_full_axis_lines(2, [0, 255, 0])

		if y_grid:
			self.graph.chart_plot_hgrid_lines(y_grid_spacing, 2, 1, [0, 255, 0])

		if x_grid:
			self.graph.chart_plot_vgrid_lines(x_grid_spacing, 2, 1, [0, 255, 0])

		# Compute offsets to origin
		y_val_offset = self.compute.limit_yo()
		x_val_offset = (self.setup_data['freq_start'] / 1000000.0) - self.compute.delta_x_margin_xo()

		# Plot axis values
		self.xo = self.compute.limit_xo()
		self.xoo = self.compute.limit_xoo()
		self.yo = self.compute.limit_yo()
		self.yoo = self.compute.limit_yoo()

		if x_coord:
			self.graph.chart_plot_x_axis_values(CHART_FONT, "%.2f", x_grid_spacing, -y_offset * 0.015, 2, [0, 255, 0], x_val_offset, True)

		if y_coord:
			self.graph.chart_plot_y_axis_values(CHART_FONT, "%.2f", y_grid_spacing, -x_offset * 0.0075, 2, [0, 255, 0], y_val_offset)


	def plot_bands(self):
		pxl_data = self.compute.band_to_pxl(band_range)

		for entry in pxl_data:
			print entry
			self.graph.image.plot_dotted_line(entry[1], 101, entry[1], self.chart_height + 100, 3, 1, [255, 255, 255])
			self.graph.image.plot_hstring(CHART_FONT, entry[1], 200 + self.chart_height - 60, 2, "|", [0, 255, 0], False, False, False, False)
			self.graph.image.plot_hstring(CHART_FONT, int(entry[1] + ((entry[2] - entry[1]) / 2)), 200 + self.chart_height - 60, 2, entry[0], [0, 255, 0], False, False, False, False)
			self.graph.image.plot_hstring(CHART_FONT, entry[2], 200 + self.chart_height - 60, 2, "|", [0, 255, 0], False, False, False, False)


	def plot_app_bands(self):
		pxl_data = self.compute.band_to_pxl(app_band_range)

		count = 1

		for entry in pxl_data:
			self.graph.image.plot_dotted_line(entry[1], 101, entry[1], self.chart_height + 100, 5, 1, [0, 255, 255])
			self.graph.image.plot_hstring(CHART_FONT, int(entry[1] + 1), 100 + (int(self.chart_height * 0.05) * count), 1, entry[0], [255, 255, 0], False, False, False, False)
			self.graph.image.plot_dotted_line(entry[2], 101, entry[2], self.chart_height + 100, 5, 1, [0, 255, 255])

			count += 1
			if count >= 6:
				count = 1

			print entry

	def plot_tunning_freq(self):
		entry = self.state_data

		if entry['freq_range_end'] == None:
			return

		self.graph.image.plot_circle(int((self.chart_width / self.compute.delta_x_abs()) * ((entry['freq_range_end'] / 1000.0) - (self.setup_data['freq_start'] / 1000000.0) + self.compute.delta_x_margin_xo()) + 100), 100, 1, 4, 1, [200, 200, 200])

	def plot_info(self):
		# Frequency Range, Average Count, Sample Rate
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.1415), 30, 2, "Freq. Range:\nAverage Count:\nSample Rate:", [0, 255, 0], False, False, True, False)

		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.1415) + 20, 30, 2, "%.2f - %.2f MHz\n%d Samples\n%.2f MSps" % (self.setup_data['freq_start'] / 1000000.0, self.setup_data['freq_end'] / 1000000.0, self.setup_data['average'], self.setup_data['sample_rate'] / 1000000.0), [0, 255, 0], False, False, False, False)

		# Coarse bandwidth, Fine bandwidth, Spread
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.3302), 30, 2, "Coarse BW:\nFine BW:\nSpread:", [0, 255, 0], False, False, True, False)

		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.3302) + 20, 30, 2, "%.3f MHz\n%.3f MHz\n%.3f MHz" % (self.setup_data['coarse_bandwidth'] / 1000000.0, self.setup_data['fine_bandwidth'] / 1000000.0, self.setup_data['spread'] / 1000000.0), [0, 255, 0], False, False, False, False)

		# Threshold, FFT Width, Step
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.4717), 30, 2, "Threshold:\nFFT Width:\nStep:", [0, 255, 0], False, False, True, False)

		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.4717) + 20, 30, 2, "%.2f dBm\n%d\n%.3f MHz" % (self.setup_data['threshold'], self.setup_data['fft_width'], self.setup_data['step'] / 1000000.0), [0, 255, 0], False, False, False, False)

		# Freq Correction, Gain, IF Gain
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.6132), 30, 2, "Freq. Corr.\nGain:\nIF Gain:", [0, 255, 0], False, False, True, False)

		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.6132) + 20, 30, 2, "%d ppm\n%.2f dB\n%.2f dB" % (self.setup_data['freq_correction'], self.setup_data['gain'], self.setup_data['if_gain']), [0, 255, 0], False, False, False, False)

		# Freq Shift, Filter, Time
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.7547), 30, 2, "Freq. Shift:\nFilter:\nP. Time:", [0, 255, 0], False, False, True, False)

		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.7547) + 20, 30, 2, "%.3f MHz\n%s\n%d sec" % (self.setup_data['freq_shift'], self.setup_data['filter'], self.setup_data['ptime']), [0, 255, 0], False, False, False, False)

		# Plot legend
		self.graph.image.plot_dot(int((self.chart_width + 200) * 0.8774), 30, 15, [255, 0, 0])
		self.graph.image.plot_dot(int((self.chart_width + 200) * 0.8774), 60, 15, [0, 0, 255])

		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.8774) + 20, 26, 2, "Cur Values", [0, 255, 0], False, False, False, False)
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) * 0.8774) + 20, 56, 2, "Max Values", [0, 255, 0], False, False, False, False)

		# Plot y-axis legend
		self.graph.image.plot_vstring_up(CHART_FONT, 20, (200 + self.chart_height) / 2, 2, "dBm", [0, 255, 0], False, False, False, True)

		# Plot bottom x-axis legend
		self.plot_bands()

		# Plot top x-axis legend
		if self.bands:
			self.plot_app_bands()

		# Plot tunning freq
		self.plot_tunning_freq()

		# Plot creation date
		self.graph.image.plot_hstring(CHART_FONT, int((self.chart_width + 200) / 2), 200 + self.chart_height - 15, 2, time.strftime("%b %d, %Y - %H:%M:%S %Z"), [0, 255, 0], True, False, False, False)

		# Plot Copyright
		self.graph.image.plot_vstring_up(CHART_FONT, self.chart_width + 200 - 5, self.chart_height + 200 - 10, 1, "COPYRIGHT - FQSCAN v" + FQSCAN_VERSION + " by Pedro A. Hortas - www.ucodev.org", [0, 255, 0], False, False, True, True)

	def plot_data(self):
		freq_pwmax = None
		freq_pwmin = None

		for entry in self.scan_data:
			color = [0, 0, 255]

			if entry['completed'] != 1:
				if not self.cur_values:
					continue

				color = [255, 0, 0]
			else:
				if not self.max_values:
					continue

			# Store x values of peak and min power
			if entry['power'] == self.compute.power_peak():
				freq_pwmax = entry['freq']

			if entry['power'] == self.compute.power_min():
				freq_pwmin = entry['freq']


			self.graph.chart_plot_vbar((entry['freq'] / 1000.0) - (self.setup_data['freq_start'] / 1000000.0) + self.compute.delta_x_margin_xo(), entry['power'] - self.compute.limit_yo(), (self.chart_width / self.compute.delta_x_abs()) * (entry['width'] / 1000.0), color)

		# Plot limits with circles and legend
		if self.limits and freq_pwmax != None and freq_pwmin != None:
			x_pwmax = int((self.chart_width / self.compute.delta_x_abs()) * ((freq_pwmax / 1000.0) - (self.setup_data['freq_start'] / 1000000.0) + self.compute.delta_x_margin_xo()) + 100)
			y_pwmax = int(self.chart_height - (self.chart_height / float(self.compute.delta_y_abs())) * (self.compute.power_peak() - self.compute.limit_yo()) + 100)

			x_pwmin = int((self.chart_width / self.compute.delta_x_abs()) * ((freq_pwmin / 1000.0) - (self.setup_data['freq_start'] / 1000000.0) + self.compute.delta_x_margin_xo()) + 100)
			y_pwmin = int(self.chart_height - (self.chart_height / float(self.compute.delta_y_abs())) * (self.compute.power_min() - self.compute.limit_yo()) + 100)

			self.graph.image.plot_circle(x_pwmax, y_pwmax, 1, 10, 1, [200, 200, 200])
			self.graph.image.plot_line(x_pwmax + 8, y_pwmax + 8, x_pwmax + 40, y_pwmax + 40, 1, [200, 200, 200])
			self.graph.image.plot_line(x_pwmax + 40, y_pwmax + 40, x_pwmax + 140, y_pwmax + 40, 1, [200, 200, 200])
			self.graph.image.plot_hstring(CHART_FONT, x_pwmax + 42, y_pwmax + 32, 1, "%s dB, %s MHz" % (self.compute.power_peak(), freq_pwmax / 1000.0), [200, 200, 200], False, False, False, False)

			self.graph.image.plot_circle(x_pwmin, y_pwmin, 1, 10, 1, [200, 200, 200])
			self.graph.image.plot_line(x_pwmin + 8, y_pwmin - 8, x_pwmin + 40, y_pwmin - 40, 1, [200, 200, 200])
			self.graph.image.plot_line(x_pwmin + 40, y_pwmin - 40, x_pwmin + 140, y_pwmin - 40, 1, [200, 200, 200])
			self.graph.image.plot_hstring(CHART_FONT, x_pwmin + 42, y_pwmin - 48, 1, "%s dB, %s MHz" % (self.compute.power_min(), freq_pwmin / 1000.0), [200, 200, 200], False, False, False, False)


	def do(self):
		self.plot_info()
		self.plot_data()

	def export(self, filename):
		self.graph.image.matrix_export_png(filename)


