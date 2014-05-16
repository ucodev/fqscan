#!/usr/bin/python -O

#
#	pyGraCha - Graphics and Charts Library
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: pygracha.py
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

import png
import math

class plot:
	height = 0 # matrix resolution (height)
	width = 0 # matrix resolution (width)
	matrix = [[[]]]

	def __init__(self, height, width, bg_color):
		self.height = height
		self.width = width
		self.matrix = self.matrix_create(bg_color)

	def matrix_create(self, bg_color):
		return [ [ bg_color for x in range(self.width) ] for y in range(self.height) ]

	def matrix_transform_png(self):
		return [ x for y in [ x for y in self.matrix for x in y ] for x in y ]

	def matrix_export_png(self, file):
		# Generate png
		fp = open(file, 'wb')
		image = png.Writer(len(self.matrix[0]), len(self.matrix))
		image.write_array(fp, self.matrix_transform_png())
		fp.close()

	# Fonts used to plot strings on image.
	# THey are defined as vectors
	font = { \
		'Matrix Capitalized': { \
			'font_width': 3, \
			'font_height': 5, \
			'1': [[[1, 0], [1, 4]], [[0, 1], [1, 0]]], \
			'2': [[[0, 0], [2, 0]], [[2, 0], [2, 2]], [[2, 2], [0, 2]], [[0, 2], [0, 4]], [[0, 4], [2, 4]]], \
			'3': [[[0, 0], [2, 0]], [[2, 0], [2, 2]], [[2, 2], [0, 2]], [[2, 2], [2, 4]], [[2, 4], [0, 4]]], \
			'4': [[[0, 0], [0, 2]], [[0, 2], [2, 2]], [[2, 0], [2, 4]]], \
			'5': [[[0, 0], [2, 0]], [[0, 0], [0, 2]], [[0, 2], [2, 2]], [[2, 2], [2, 4]], [[2, 4], [0, 4]]], \
			'6': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [2, 2]], [[2, 2], [2, 4]], [[2, 4], [0, 4]]], \
			'7': [[[0, 0], [2, 0]], [[2, 0], [2, 4]]], \
			'8': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 0]], [[0, 2], [2, 2]]], \
			'9': [[[0, 0], [2, 0]], [[0, 0], [0, 2]], [[2, 0], [2, 4]], [[0, 2], [2, 2]], [[0, 4], [2, 4]]], \
			'0': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 0]]], \
			'.': [[[1, 4], [1, 4]]], \
			'A': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 2], [2, 2]]], \
			'a': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 2], [2, 2]]], \
			'B': [[[0, 0], [1, 0]], [[0, 0], [0, 4]], [[0, 2], [1, 2]], [[0, 4], [1, 4]], [[2, 3], [2, 3]], [[2, 1], [2, 1]]], \
			'b': [[[0, 0], [1, 0]], [[0, 0], [0, 4]], [[0, 2], [1, 2]], [[0, 4], [1, 4]], [[2, 3], [2, 3]], [[2, 1], [2, 1]]], \
			'C': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]]], \
			'c': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]]], \
			'D': [[[0, 0], [1, 0]], [[0, 0], [0, 4]], [[0, 4], [1, 4]], [[2, 1], [2, 3]]], \
			'd': [[[0, 0], [1, 0]], [[0, 0], [0, 4]], [[0, 4], [1, 4]], [[2, 1], [2, 3]]], \
			'E': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [1, 2]], [[0, 0], [2, 0]], [[0, 4], [2, 4]]], \
			'e': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [1, 2]], [[0, 0], [2, 0]], [[0, 4], [2, 4]]], \
			'F': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [1, 2]]], \
			'f': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [1, 2]]], \
			'G': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 2]]], \
			'g': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 2]]], \
			'H': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 2], [2, 2]]], \
			'h': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 2], [2, 2]]], \
			'I': [[[0, 0], [2, 0]], [[0, 4], [2, 4]], [[1, 0], [1, 4]]], \
			'i': [[[0, 0], [2, 0]], [[0, 4], [2, 4]], [[1, 0], [1, 4]]], \
			'J': [[[2, 0], [2, 4]], [[2, 4], [0, 4]], [[0, 4], [0, 3]]], \
			'j': [[[2, 0], [2, 4]], [[2, 4], [0, 4]], [[0, 4], [0, 3]]], \
			'K': [[[0, 0], [0, 4]], [[0, 3], [2, 0]], [[0, 2], [2, 4]]], \
			'k': [[[0, 0], [0, 4]], [[0, 3], [2, 0]], [[0, 2], [2, 4]]], \
			'L': [[[0, 0], [0, 4]], [[0, 4], [2, 4]]], \
			'l': [[[0, 0], [0, 4]], [[0, 4], [2, 4]]], \
			'M': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 0], [1, 1]], [[1, 1], [2, 0]]], \
			'm': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 0], [1, 1]], [[1, 1], [2, 0]]], \
			'N': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 0], [2, 4]]], \
			'n': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 0], [2, 4]]], \
			'O': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 0]]], \
			'o': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 0]]], \
			'P': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [2, 2]], [[2, 2], [2, 0]]], \
			'p': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [2, 2]], [[2, 2], [2, 0]]], \
			'Q': [[[0, 0], [2, 0]], [[0, 0], [0, 3]], [[0, 3], [2, 3]], [[2, 3], [2, 0]], [[2, 4], [2, 4]]], \
			'q': [[[0, 0], [2, 0]], [[0, 0], [0, 3]], [[0, 3], [2, 3]], [[2, 3], [2, 0]], [[2, 4], [2, 4]]], \
			'R': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [2, 2]], [[2, 0], [2, 2]], [[1, 2], [2, 4]]], \
			'r': [[[0, 0], [2, 0]], [[0, 0], [0, 4]], [[0, 2], [2, 2]], [[2, 0], [2, 2]], [[1, 2], [2, 4]]], \
			'S': [[[0, 0], [2, 0]], [[0, 0], [0, 2]], [[0, 2], [2, 2]], [[2, 2], [2, 4]], [[2, 4], [0, 4]]], \
			's': [[[0, 0], [2, 0]], [[0, 0], [0, 2]], [[0, 2], [2, 2]], [[2, 2], [2, 4]], [[2, 4], [0, 4]]], \
			'T': [[[0, 0], [2, 0]], [[1, 0], [1, 4]]], \
			't': [[[0, 0], [2, 0]], [[1, 0], [1, 4]]], \
			'U': [[[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 0]]], \
			'u': [[[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 0]]], \
			'V': [[[0, 0], [1, 4]], [[1, 4], [2, 0]]], \
			'v': [[[0, 0], [1, 4]], [[1, 4], [2, 0]]], \
			'W': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[0, 4], [1, 3]], [[1, 3], [2, 4]]], \
			'w': [[[0, 0], [0, 4]], [[2, 0], [2, 4]], [[1, 3], [1, 3]]], \
			'X': [[[0, 0], [2, 4]], [[2, 0], [0, 4]]], \
			'x': [[[0, 0], [2, 4]], [[2, 0], [0, 4]]], \
			'Y': [[[0, 0], [1, 2]], [[1, 2], [2, 0]], [[1, 2], [1, 4]]], \
			'y': [[[0, 0], [1, 2]], [[1, 2], [2, 0]], [[1, 2], [1, 4]]], \
			'Z': [[[0, 0], [2, 0]], [[2, 0], [0, 4]], [[0, 4], [2, 4]]], \
			'z': [[[0, 0], [2, 0]], [[2, 0], [0, 4]], [[0, 4], [2, 4]]], \
			'-': [[[0, 2], [2, 2]]], \
			'+': [[[1, 1], [1, 3]], [[0, 2], [2, 2]]], \
			'[': [[[0, 0], [0, 4]], [[0, 0], [1, 0]], [[0, 4], [1, 4]]], \
			'(': [[[0, 0], [0, 4]], [[0, 0], [1, 0]], [[0, 4], [1, 4]]], \
			']': [[[2, 0], [2, 4]], [[2, 0], [1, 0]], [[2, 4], [1, 4]]], \
			')': [[[2, 0], [2, 4]], [[2, 0], [1, 0]], [[2, 4], [1, 4]]], \
			'{': [[[1, 0], [2, 0]], [[1, 0], [1, 1]], [[0, 2], [1, 1]], [[0, 2], [1, 3]], [[1, 3], [1, 4]], [[1, 4], [2, 4]]], \
			'}': [[[0, 0], [1, 0]], [[1, 0], [1, 1]], [[2, 2], [1, 1]], [[2, 2], [1, 3]], [[1, 3], [1, 4]], [[1, 4], [0, 4]]], \
			'/': [[[0, 4], [2, 0]]], \
			'\\': [[[0, 0], [2, 4]]], \
			'"': [[[0, 0], [0, 1]], [[2, 0], [2, 1]]], \
			'\'': [[[1, 0], [1, 1]]], \
			'%': [[[0, 4], [2, 0]], [[0, 0], [0, 0]], [[2, 4], [2, 4]]], \
			'!': [[[1, 0], [1, 2]], [[1, 4], [1, 4]]], \
			'=': [[[0, 1], [2, 1]], [[0, 3], [2, 3]]], \
			':': [[[1, 1], [1, 1]], [[1, 3], [1, 3]]], \
			';': [[[1, 1], [1, 1]], [[1, 3], [0, 4]]], \
			',': [[[1, 3], [0, 4]]], \
			'_': [[[0, 4], [2, 4]]], \
			'<': [[[0, 2], [2, 0]], [[0, 2], [2, 4]]], \
			'>': [[[2, 2], [0, 0]], [[2, 2], [0, 4]]], \
			'*': [[[0, 0], [0, 0]], [[2, 0], [2, 0]], [[0, 2], [2, 2]], [[0, 4], [0, 4]], [[2, 4], [2, 4]], [[0, 4], [1, 3]], [[0, 0], [1, 1]], [[2, 0], [1, 1]], [[2, 4], [1, 3]]], \
			'|': [[[1, 0], [1, 4]]], \
			'@': [[[0, 0], [1, 0]], [[0, 0], [0, 4]], [[0, 4], [2, 4]], [[2, 4], [2, 2]], [[2, 2], [0, 2]]], \
			'?': [[[0, 0], [2, 0]], [[2, 0], [2, 2]], [[2, 2], [1, 2]], [[1, 4], [1, 4]]], \
			'^': [[[1, 0], [0, 1]], [[1, 0], [2, 1]]], \
			'&': [[[1, 0], [2, 0]], [[0, 1], [2, 3]], [[1, 4], [2, 4]], [[1, 0], [0, 1]], [[0, 3], [1, 2]], [[0, 3], [1, 4]], [[2, 2], [2, 2]]], \
			'~': [[[0, 2], [1, 2]], [[1, 1], [2, 1]]], \
			'$': [[[1, 0], [1, 4]], [[2, 0], [2, 0]], [[0, 1], [0, 1]], [[2, 2], [2, 2]], [[0, 3], [0, 3]]], \
			'#': [[[1, 0], [1, 4]], [[0, 1], [2, 1]], [[0, 3], [2, 3]]]
		}
	}

	def font_rotate_mirror_char(self, font_name, char, angle_x):
		(tchar, height, width) = self.font_rotate_char(font_name, char, angle_x)

		# Mirror Char
		# Subtract font width to x and get the absolute value:
		# abs(x - font.width)
		tchar = map(lambda line: [ [ abs(line[0][0] - (width - 1)), line[0][1] ], [ abs(line[1][0] - (height - 1)), line[1][1] ] ], tchar)

		return (tchar, height, width)

	def font_rotate_char(self, font_name, char, angle_x):
		char = self.font[font_name][char]
		width = self.font[font_name]['font_width']
		height = self.font[font_name]['font_height']

		# For now, only rotations of 90, 180 and 270 are supported
		if angle_x > 45 and angle_x <= 135:
			# Rotate char 90 degrees right.
			# Just swap x,y = y,x 
			# After the swap, subtract height (the new width) to x1
			# and x2 and use the absolute value of the result:
			#	x1 = abs(x1 - height)
			#	x2 = abs(x2 - height)
			#
			char = map(lambda tline: [ [ abs(tline[0][1] - (self.font[font_name]['font_height'] - 1)), tline[0][0] ], [ abs(tline[1][1] - (self.font[font_name]['font_height'] - 1)), tline[1][0] ] ], char)
			width = self.font[font_name]['font_height']
			height = self.font[font_name]['font_width']
		elif angle_x > 135 and angle_x <= 225:
			# Rotate char 180 degrees. This is performed by 2
			# rotations of 90 degrees
			char = map(lambda tline: [ [ abs(tline[0][1] - (self.font[font_name]['font_height'] - 1)), tline[0][0] ], [ abs(tline[1][1] - (self.font[font_name]['font_height'] - 1)), tline[1][0] ] ], char)
			char = map(lambda tline: [ [ abs(tline[0][1] - (self.font[font_name]['font_width'] - 1)), tline[0][0] ], [ abs(tline[1][1] - (self.font[font_name]['font_width'] - 1)), tline[1][0] ] ], char)
			width = self.font[font_name]['font_width']
			height = self.font[font_name]['font_height']
		elif angle_x > 225 and angle_x <= 315:
			# Rotate char 270 degrees. Perform 3 rotations of 90
			# degrees
			char = map(lambda tline: [ [ abs(tline[0][1] - (self.font[font_name]['font_height'] - 1)), tline[0][0] ], [ abs(tline[1][1] - (self.font[font_name]['font_height'] - 1)), tline[1][0] ] ], char)
			char = map(lambda tline: [ [ abs(tline[0][1] - (self.font[font_name]['font_width'] - 1)), tline[0][0] ], [ abs(tline[1][1] - (self.font[font_name]['font_width'] - 1)), tline[1][0] ] ], char)
			char = map(lambda tline: [ [ abs(tline[0][1] - (self.font[font_name]['font_height'] - 1)), tline[0][0] ], [ abs(tline[1][1] - (self.font[font_name]['font_height'] - 1)), tline[1][0] ] ], char)
			width = self.font[font_name]['font_height']
			height = self.font[font_name]['font_width']

		return (char, height, width)

	def plot_vchar(self, font_name, x, y, size, char, color, center_x, center_y, force_angle = 90):
		(tchar, height, width) = self.font_rotate_char(font_name, char, force_angle)

		if center_x:
			# Since char is rotated by 90 degrees, height is the
			# new width
			x = x - ((width * size) / 2)
		if center_y:
			# Since char is rotated by 90 degrees, width is the
			# new height
			y = y - ((height * size) / 2)
		for line in tchar:
			line = map(lambda x: list([x[0] * size, x[1] * size]), line)
			self.plot_line(x + line[0][0], y + line[0][1], x + line[1][0], y + line[1][1], size, color)

	def plot_hchar(self, font_name, x, y, size, char, color, center_x, center_y):
		if center_x:
			x = x - ((self.font[font_name]['font_width'] * size) / 2)
		if center_y:
			y = y - ((self.font[font_name]['font_height'] * size) / 2)

		for line in self.font[font_name][char]:
			# Scale char
			line = map(lambda x: list([x[0] * size, x[1] * size]), line)
			self.plot_line(x + line[0][0], y + line[0][1], x + line[1][0], y + line[1][1], size, color)

	def plot_vstring_up(self, font_name, x, y, size, string, color, center_x, center_y, start_right, start_bottom):
		self.plot_vstring(font_name, x, y, size, string[::-1], color, center_x, center_y, start_right, start_bottom, 270)

	def plot_vstring(self, font_name, x, y, size, string, color, center_x, center_y, start_right, start_bottom, force_angle = 90):
		char_list = list(string)

		if start_right:
			# Character plotting ends at pixel x. This allows right
			# justification
			x = x - (self.font[font_name]['font_height'] * size)

		if start_bottom:
			# String plotting ends at y. This allows top
			# justifiction
			sign = -1
			char_list.reverse()
		else:
			sign = 1

		if center_y:
			# ((len(char_list) - 1) * size) is the spaces between
			# characters
			y_lenght = len(char_list) * self.font[font_name]['font_height'] * size + ((len(char_list) - 1) * size)
			y = y - (x_length / 2)

		# Store y initial position. This will be useful for new lines \n
		y_init = y
		char_nr = 0

		for char in char_list:
			if char == '\n':
				y = y_init
				# We consider height here instead width because
				# characters are rotated by 90 degrees.
				x = x - (self.font[font_name]['font_height'] * size) - (size * 2) # size * 2 is the space between lines
				# Reset char_nr since we're starting in a
				# new line
				char_nr = 0
				continue

			if char != ' ':
				self.plot_vchar(font_name, x, y + ((self.font[font_name]['font_height'] * char_nr * size) * sign), size, char, color, center_x, center_y, force_angle)

			char_nr = char_nr + 1
			# offset between chars is 'size' pixels
			y = y + (size * sign)

	def plot_hstring(self, font_name, x, y, size, string, color, center_x, center_y, start_right, start_bottom):
		char_list = list(string)

		if start_bottom:
			# Char plotting ends at y. This allows top
			# justification
			y = y - (self.font[font_name]['font_height'] * size)

		if start_right:
			# String plotting ends at x. This allows right
			# justification
			sign = -1
			string_list = string.split("\n")
			string_list.reverse()
			char_list = list("\n".join(string_list))
			char_list.reverse()
		else:
			sign = 1

		if center_x:
			x_lenght = len(char_list) * self.font[font_name]['font_width'] * size + ((len(char_list) - 1) * size)
			x = x - (x_lenght / 2)

		# Store x initial position. This will be useful for new lines \n
		x_init = x
		char_nr = 0

		for char in char_list:
			if char == '\n':
				x = x_init
				y = y + (self.font[font_name]['font_height'] * size) + (size * 2) # size * 2 is the space between lines
				# Reset char_nr since we're starting in a
				# new line
				char_nr = 0
				continue

			if char != ' ':
				self.plot_hchar(font_name, x + ((self.font[font_name]['font_width'] * char_nr * size) * sign), y, size, char, color, center_x, center_y)

			char_nr = char_nr + 1
			# offset between chars is 'size' pixels
			x = x + (size * sign)

	def plot_dot(self, x, y, size, color):
		size_x = size
		size_y = size

		if (x < 0) or (y < 0) or (x > self.width) or (y > self.height):
			return

		x = int(x - (size / 2))
		y = int(y - (size / 2))

		# Recompute sizes when the dot is out of borders due to its size
		if x < 0:
			size_x = size_x + x # subtract x (negative) to size
			x = 0
		if y < 0:
			size_y = size_y + y # subtract y (negative) to size
			y = 0
		if (y + size_y) > self.height:
			size_y = (y + size_y) - self.height
			y = self.height - size_y
		if (x + size_x) > self.width:
			size_x = (x + size_x) - self.width
			x = self.width - size_x

		for offset_y in range(size_y):
			if (y + offset_y) > self.height:
				offset_y = 0
			for offset_x in range(size_x):
				if (x + offset_x) > self.width:
					offset_x = 0
				self.matrix[y + offset_y][x + offset_x] = color

	def plot_circle(self, x, y, angle_interval, radius, size, color, filled = False, fill_color = [255, 0, 0]):
		t = 0	# angle tetta

		while t <= 360.0:
			# (x, y) == (cos t, sin t)
			self.plot_dot(x + math.cos((t * (math.pi / 180.0))) * radius, y + math.sin((t * (math.pi / 180.0))) * radius, size, color) 

			if filled:
				self.plot_dotted_line(int(x), int(y), int(x + math.cos((t * (math.pi / 180.0))) * (radius - size)), int(y + math.sin((t * (math.pi / 180.0))) * (radius - size)), 1, size, fill_color)

			t += angle_interval

	def plot_dotted_line(self, x1, y1, x2, y2, interval, size, color):
		# A NOTE FOR WHOM THAT WILL TRY TO UNDERSTAND THIS FUNCTION
		#
		# It's very easy to get confused reading this if you don't
		# have in mind that the matrix is FLIPPED on its x-axis. This
		# means that the vertical paths are computed from top to bottom
		# and not from bottom to top. Think about this a bit before
		# continuing :). This will explain the *weird* delta
		# calculations and the use of absolute values in conditions.
		#

		# Draw always from left to right. This simplifies the algorithm
		if x2 < x1:
			aux = x1
			x1 = x2
			x2 = aux
			aux = y1
			y1 = y2
			y2 = aux

		# Initialize offsets to int(0)
		offset_x = 0
		offset_y = 0

		# Compute delta values
		delta_x = (x2 - x1)
		delta_y = (y2 - y1)

		# We need to use abs() here because the matrix is flipped by x.
		# This means that y pixels are rendered from top to bottom
		if abs(delta_y) > abs(delta_x):
			if not delta_x:
				# if delta is 0, this is a vertical line.
				# So only y-axis will change
				# Also we add 1 (or -1) to delta in order to
				# reach the last dot to be plotted.
				# (Eg: 12 - 10 is 2, but to plot from 10 to 12
				# we need 3 dots (10,11,12). So 2+1 = 3.
				offset_y = delta_y + (delta_y / abs(delta_y))
				pad_y = 0.0
			else:
				# Compute offset for each y value, how many x
				# will be plotted. Alignments will be computed
				# with pad variables
				offset_y = int((float(delta_y) / float(delta_x)))
				# This is the reference pad that will be sum()d
				# to the counter pad variable. Each time the
				# counter pad variable reaches 1, one more
				# pixel is plotted in the column
				pad_y = abs(float(float(delta_y) / float(delta_x))) - abs(offset_y)

		# Same here for abs()
		elif abs(delta_x) >= abs(delta_y):
			if not delta_y:
				# if delta is 0, this is a horizontal line.
				# So only x-axis will change
				# Also we add 1 (or -1) to delta in order to
				# reach the last dot to be plotted.
				# (Eg: 12 - 10 is 2, but to plot from 10 to 12
				# we need 3 dots (10,11,12). So 2+1 = 3.
				offset_x = delta_x + 1
				pad_x = 0.0
			else:
				# Compute offset for each y value, how many x
				# will be plotted. Alignments will be computed
				# with pad variables
				offset_x = int((float(delta_x) / float(delta_y)))
				# This is the reference pad that will be sum()d
				# to the counter pad variable. Each time the
				# counter pad variable reaches 1, one more
				# pixel is plotted in the row
				pad_x = abs(float(float(delta_x) / float(delta_y))) - abs(offset_x)

		# Store x1 and y1 in index variables
		x = x1
		y = y1
		# Assume positive increments and range()s by default
		sign = 1

		# initialize pad counters as float(0)
		pad_y_count = 0.0
		pad_x_count = 0.0

		# We need to compare the absolute value of offsets here because
		# one offset is always 0 and the other may be negative when
		# drawing backwards
		if abs(offset_x) > abs(offset_y):
			if offset_x < 0:
				sign = -1 # Sign is needed to in/decrement
					  # y var accordingly
				# Even though delta_x is always positive,
				# the division by delta_y may change the
				# offset_x sign. Here we revert it to positive
				offset_x = abs(offset_x)
			# We need to execute this loop at least once and
			# perform the break condition before variable
			# increments. This is needed to plot horizontal lines
			# which means that y and y2 are equal (delta is 0)
			while 1:
				if pad_x_count >= 1:
					offset_x_pad = offset_x + 1
					pad_x_count = pad_x_count - 1
				else:
					offset_x_pad = offset_x

				# Plot 'offset_x' dots in this row
				for i in range(offset_x_pad):
					if not ((x + i) % interval):
						self.plot_dot(x + i, y, size, color)
						if (x + i) >= x2:
							break

				# Compare before increment
				if sign > 0:
					if y >= y2:
						return
				else:
					if y <= y2:
						return

				# Update variables accordingly after
				# break condition
				y = y + sign
				x = x + offset_x_pad
				pad_x_count = pad_x_count + pad_x

		else:
			if offset_y < 0:
				sign = -1 # range() should return an array of
					  # negative integers to decrement y
			# This is a do { } while() loop. condition is at the
			# end. We need to execute this loop once to allow the
			# algorithm to plot vertical lines which means that
			# x and x2 are equal (delta is 0)
			while 1:
				if pad_y_count >= 1:
					offset_y_pad = offset_y + sign
					pad_y_count = pad_y_count - 1
				else:
					offset_y_pad = offset_y

				# Plot 'offset_y' dots in this column
				for i in range(0, offset_y_pad, sign):
					if not ((y + i) % interval):
						self.plot_dot(x, y + i, size, color)
						if sign > 0:
							if (y + i) >= y2:
								return
						else:
							if (y + i) <= y2:
								return


				# Update variables accordingly before
				# break condition
				x = x + 1
				y = y + offset_y_pad
				pad_y_count = pad_y_count + pad_y

				# Since we always draw from left to write, x
				# is always lesser than or equal to x2
				if x > x2:
					return

	def plot_line(self, x1, y1, x2, y2, size, color):
		self.plot_dotted_line(x1, y1, x2, y2, 1, size, color)

	def plot_dotted_vline(self, x, y1, y2, interval, size, color):
		self.plot_dotted_line(x, y1, x, y2, interval, size, color)

	def plot_dotted_hline(self, y, x1, x2, interval, size, color):
		self.plot_dotted_line(x1, y, x2, y, interval, size, color)

	def plot_vline(self, x, y1, y2, size, color):
		self.plot_dotted_vline(x, y1, y2, 1, size, color)

	def plot_hline(self, y, x1, x2, size, color):
		self.plot_dotted_hline(y, x1, x2, 1, size, color)

	def plot_full_dotted_hline(self, y, interval, size, color):
		self.plot_dotted_hline(y, 0, self.width, interval, size, color)

	def plot_full_dotted_vline(self, x, interval, size, color):
		self.plot_dotted_vline(x, 0, self.height, interval, size, color)

	def plot_full_hline(self, y, size, color):
		self.plot_full_dotted_hline(y, 1, size, color)

	def plot_full_vline(self, x, size, color):
		self.plot_full_dotted_vline(x, 1, size, color)


class chart:
	xo = 0 # origin x in pixels
	yo = 0 # origin y in pixels
	orig_x = 0 # in amplitude units
	orig_y = 0 # in amplitude units
	x_limit = 0 # X amplitude
	y_limit = 0 # Y amplitude
	vborder = 0 # Vertical borders size in pixels
	hborder = 0 # Horizontal borders size in pixels
	axis_size = 0

	# Image lib
	image = {}

	# Control variables
	axis_lines_full = False		# If True, full axis lines are plotted
	axis_x_positive_lines = False	# If True, x-axis positive lines
					# are plotted
	axis_x_negative_lines = False	# If True, x-axis negative lines
					# are plotted
	axis_y_positive_lines = False	# If True, y-axis positive lines
					# are plotted
	axis_y_negative_lines = False	# If True, y-axis negative lines
					# are plotted


	def __init__(self, x_amp, y_amp, orig_x, orig_y, height, width, vborder_size, hborder_size, bg_color):
		self.image = plot(height + (hborder_size * 2), width + (vborder_size * 2), bg_color)

		self.vborder = vborder_size
		self.hborder = hborder_size
		self.chart_height = height
		self.chart_width = width
		self.x_limit = x_amp
		self.y_limit = y_amp
		self.orig_x = orig_x
		self.orig_y = orig_y

		if orig_x:
			self.xo = int((float(self.chart_width) * float(orig_x)) / float(self.x_limit)) + self.vborder
		else:
			self.xo = 0 + self.vborder
		if orig_y:
			self.yo = self.chart_height - int((float(self.chart_height) * float(orig_y)) / float(self.y_limit)) + self.hborder
		else:
			self.yo = self.chart_height + self.hborder

	def chart_plot_x_axis_lines(self, size, color, positive = True, negative = True):
		self.axis_x_positive_lines = positive
		self.axis_x_negative_lines = negative

		if self.axis_x_positive_lines:
			self.image.plot_hline(self.yo, self.xo, self.chart_width + self.vborder, size, color)

		if self.axis_x_negative_lines:
			self.image.plot_hline(self.yo, 0 + self.hborder, self.xo, size, color)

	def chart_plot_y_axis_lines(self, size, color, positive = True, negative = True):
		self.axis_y_positive_lines = positive
		self.axis_y_negative_lines = negative

		if self.axis_y_positive_lines:
			self.image.plot_vline(self.xo, self.yo, 0 + self.vborder, size, color)

		if self.axis_y_negative_lines:
			self.image.plot_vline(self.xo, self.yo, self.chart_height + self.hborder, size, color)

	def chart_plot_full_axis_lines(self, size, color):
		self.axis_lines_full = True
		self.axis_size = size
		self.chart_plot_x_axis_lines(size, color, True, True)
		self.chart_plot_y_axis_lines(size, color, True, True)

	def chart_plot_x_axis_values(self, font, format, space, y_offset, size, color, x_val_offset = 0.0, plot_origin = False):
		x = 0.0
		limit = self.x_limit

		if y_offset > 0:
			justify_top = True
		else:
			justify_top = False

		if self.axis_x_positive_lines and not self.axis_x_negative_lines:
			x = float(self.orig_x)

		if self.axis_x_negative_lines and not self.axis_x_positive_lines:
			limit = self.orig_x

		while float(format % x) <= (limit + (space / 2.0)):
			if float(format % x) != self.orig_x or plot_origin == True:
				self.image.plot_hstring(font, int((x * self.chart_width) / self.x_limit) + self.vborder, self.chart_height - int(((self.orig_y + y_offset) * self.chart_height) / self.y_limit) + self.hborder, size, format % float(x_val_offset + x - self.orig_x), color, True, False, False, justify_top)

			x = x + space

	def chart_plot_y_axis_values(self, font, format, space, x_offset, size, color, y_val_offset = 0.0, plot_origin = False):
		y = 0.0
		limit = self.y_limit

		if x_offset < 0:
			justify_right = True
		else:
			justify_right = False

		if self.axis_y_positive_lines and not self.axis_y_negative_lines:
			y = float(self.orig_y)

		if self.axis_y_negative_lines and not self.axis_y_positive_lines:
			limit = self.orig_y

		while float(format % y) <= (limit + (space / 2.0)):
			if float(format % y) != self.orig_y or plot_origin == True:
				self.image.plot_hstring(font, int(((self.orig_x + x_offset) * self.chart_width) / self.x_limit) + self.vborder, self.chart_height - int((y * self.chart_height) / self.y_limit) + self.hborder, size, format % float(y_val_offset + y - self.orig_y), color, False, True, justify_right, False)

			y = y + space

	def chart_plot_hgrid_lines(self, space, interval, size, color):
		# 'space' is the space between bars
		# 'interval' is the *blank* interval between pixels of the line
		if not self.axis_y_positive_lines and not self.axis_y_negative_lines:
			return

		y1 = y2 = x1 = x2 = 0

		if self.axis_y_positive_lines:
			if self.axis_x_positive_lines:
				y2 = self.y_limit - self.orig_y
				x2 = self.x_limit - self.orig_x

			if self.axis_x_negative_lines:
				y2 = self.y_limit - self.orig_y
				x1 = 0 - self.orig_x
	
		if self.axis_y_negative_lines:
			if self.axis_x_positive_lines:
				y1 = 0 - self.orig_y
				x2 = self.x_limit - self.orig_x

			if self.axis_x_negative_lines:
				y1 = 0 - self.orig_y
				x1 = 0 - self.orig_x

		while True:
			self.chart_plot_dotted_line(x1, y1, x2, y1, interval, size, color)

			y1 = y1 + space

			if y1 > (y2 + (space / 2.0)):
				break


	def chart_plot_vgrid_lines(self, space, interval, size, color):
		# 'space' is the space between bars
		# 'interval' is the *blank* interval between pixels of the line
		if not self.axis_x_positive_lines and not self.axis_x_negative_lines:
			return

		x1 = x2 = y1 = y2 = 0

		if self.axis_x_positive_lines:
			if self.axis_y_positive_lines:
				x2 = self.x_limit - self.orig_x
				y2 = self.y_limit - self.orig_y

			if self.axis_y_negative_lines:
				x2 = self.x_limit - self.orig_x
				y1 = 0 - self.orig_y
	
		if self.axis_x_negative_lines:
			if self.axis_y_positive_lines:
				x1 = 0 - self.orig_x
				y2 = self.y_limit - self.orig_y

			if self.axis_y_negative_lines:
				x1 = 0 - self.orig_x
				y1 = 0 - self.orig_y

		while True:
			self.chart_plot_dotted_line(x1, y1, x1, y2, interval, size, color)

			x1 = x1 + space

			if x1 > (x2 + (space / 2.0)):
				break

	def chart_plot_dot(self, x, y, size, color):
		if x > (self.x_limit + self.orig_x) or x < (0 - self.orig_x):
			return

		if y > (self.y_limit + self.orig_y) or y < (0 - self.orig_y):
			return

		x = int(round((float(x) * float(self.chart_width)) / float(self.x_limit)))
		y = int(round((float(y) * float(self.chart_height)) / float(self.y_limit)))

		self.image.plot_dot(x + self.xo, self.yo - y, size, color)

	def chart_plot_dotted_line(self, x1, y1, x2, y2, interval, size, color):
		x1 = int(round((float(x1) * float(self.chart_width)) / float(self.x_limit)))
		x2 = int(round((float(x2) * float(self.chart_width)) / float(self.x_limit)))
		y1 = int(round((float(y1) * float(self.chart_height)) / float(self.y_limit)))
		y2 = int(round((float(y2) * float(self.chart_height)) / float(self.y_limit)))

		self.image.plot_dotted_line(x1 + self.xo, self.yo - y1, x2 + self.xo, self.yo - y2, interval, size, color)

	def chart_plot_line(self, x1, y1, x2, y2, size, color):
		self.chart_plot_dotted_line(x1, y1, x2, y2, 1, size, color)

	def chart_plot_vline(self, x, y1, y2, size, color):
		x = int(round((float(x) * float(self.chart_width)) / float(self.x_limit)))
		y1 = int(round((float(y1) * float(self.chart_height)) / float(self.y_limit)))
		y2 = int(round((float(y2) * float(self.chart_height)) / float(self.y_limit)))

		self.image.plot_vline(x + self.xo, self.yo - y1, self.yo - y2, size, color)

	def chart_plot_hline(self, y, x1, x2, size, color):
		y = int(round((float(y) * float(self.chart_height)) / float(self.y_limit)))
		x1 = int(round((float(x1) * float(self.chart_width)) / float(self.x_limit)))
		x2 = int(round((float(x2) * float(self.chart_width)) / float(self.x_limit)))

		self.image.plot_hline(self.yo - y, x1 + self.xo, x2 + self.xo, size, color)

	def chart_plot_vbar(self, x1, height, size, color):
		y1 = 0
		y2 = height
		x1 = int(round((float(x1) * float(self.chart_width)) / float(self.x_limit)))
		x1 = int(x1 - (size / 2))
		x2 = x1 + size
		# sum axis_size to the bar start at the pixel after de x-axis
		y1 = int(round((float(y1) * float(self.chart_height)) / float(self.y_limit)) + int((self.axis_size / 2) + 1))
		# compute absulute value of y2 in pixels
		y2 = int(round((float(y2) * float(self.chart_height)) / float(self.y_limit)))

		x = x1

		while x <= x2:
			self.image.plot_vline(x + self.xo, self.yo - y1, self.yo - y2, 1, color)
			x = x + 1

	def chart_plot_hbar(self, y1, width, size, color):
		x1 = 0
		x2 = width
		y1 = int(round((float(y1) * float(self.chart_height)) / float(self.y_limit)))
		y1 = int(y1 - (size / 2))
		y2 = y1 + size
		x1 = int(round((float(x1) * float(self.chart_width)) / float(self.x_limit)) + int(self.axis_size / 2))
		x2 = int(round((float(x2) * float(self.chart_width)) / float(self.x_limit)))

		y = y1

		while y <= y2:
			self.image.plot_hline(self.yo - y, self.xo + x1, self.xo + x2, 1, color)
			y = y + 1


