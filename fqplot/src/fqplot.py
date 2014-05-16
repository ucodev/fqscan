#!/usr/bin/python -O

#
#	fqplot - FQScan Plotter
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: fqplot.py
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

import sys
import math
import time

from mysql import *
from fqchart import *
from config import *

margin_x_tolerance = 0.1	# 10% of total margin. 5% left, 5% right
margin_y_tolerance = 0.4	# 40% of margin (Top only)

args = {}


#db
def db_set_axis_data(setup_id, xo, xoo, yo, yoo):
	db_conn = mysql_connect(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASS, MYSQL_DB)

	query = "UPDATE state SET xo = " + str(xo) + ", xoo = " + str(xoo) + ", yo = " + str(yo) + ", yoo = " + str(yoo) + " WHERE setup_id = " + setup_id + " AND xo IS NULL";

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		print("db_get_values(): db_cursor.execute(\"%s\"): Failed" % query)
		db_cursor.close()
		db_conn.close()
		return

	db_conn.commit()


def db_get_setup_data(setup_id):
	db_conn = mysql_connect(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASS, MYSQL_DB)

	query = "SELECT setup.id,average,coarse_bandwidth,fine_bandwidth,spread,threshold,freq_start,freq_end,sample_rate,fft_width,step,ptime,freq_correction,gain,if_gain,freq_shift,filters_id,filters.filter FROM setup LEFT JOIN filters ON setup.filters_id = filters.id WHERE setup.id = " + str(setup_id)
 
	#query = "SELECT * FROM setup WHERE id = " + str(setup_id)

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		print("db_get_values(): db_cursor.execute(\"%s\"): Failed" % query)
		db_cursor.close()
		db_conn.close()
		return None

	data = []

	try:
		row = db_cursor.fetchone()
	except:
		print("db_get_values(): db_cursor.fetchone(): Failed")
		db_cursor.close()
		db_conn.close()
		return None

	if not row or row == None:
		return None

	"SELECT average,coarse_bandwidth,fine_bandwidth,spread,threshold,freq_start,freq-end,sample_rate,fft_width,step,ptime,freq_correction,gain,if_gain,freq_shift,filters_id,filter"
	entry = {}
	entry['average'] = row[1]
	entry['coarse_bandwidth'] = row[2]
	entry['fine_bandwidth'] = row[3]
	entry['spread'] = row[4]
	entry['threshold'] = row[5]
	entry['freq_start'] = row[6]
	entry['freq_end'] = row[7]
	entry['sample_rate'] = row[8]
	entry['fft_width'] = row[9]
	entry['step'] = row[10]
	entry['ptime'] = row[11]
	entry['freq_correction'] = row[12]
	entry['gain'] = row[13]
	entry['if_gain'] = row[14]
	entry['freq_shift'] = row[15]
	entry['filters_id'] = row[16]
	entry['filter'] = row[17]

	db_cursor.close()
	db_conn.close()

	return entry

def db_get_state_data(setup_id):
	db_conn = mysql_connect(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASS, MYSQL_DB)

	query = "SELECT * FROM state WHERE setup_id = " + str(setup_id) + " ORDER BY id DESC LIMIT 1"

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		print("db_get_values(): db_cursor.execute(\"%s\"): Failed #2" % query)
		db_cursor.close()
		db_conn.close()
		return None

	data = []

	try:
		row = db_cursor.fetchone()
	except:
		print("db_get_values(): db_cursor.fetchone(): Failed #2")
		db_cursor.close()
		db_conn.close()
		return None

	if not row or row == None:
		db_cursor.close()
		db_conn.close()
		return None

	entry = {}
	entry['running'] = row[2]
	entry['freq_range_start'] = row[3]
	entry['freq_range_end'] = row[4]

	db_cursor.close()
	db_conn.close()

	return entry


def db_get_scan_data(setup_id):
	db_conn = mysql_connect(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASS, MYSQL_DB)

	query = "SELECT DISTINCT(freq) AS freq,MAX(width) AS width,MAX(power) AS power,count,completed FROM scan_data WHERE completed = 1 AND setup_id = " + str(setup_id) + " GROUP BY freq UNION SELECT freq,width,power,count,completed FROM scan_data WHERE completed = 0 AND setup_id = " + str(setup_id)

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		print("db_get_values(): db_cursor.execute(\"%s\"): Failed #3" % query)
		db_cursor.close()
		db_conn.close()
		return None

	data = []

	while True:
		try:
			row = db_cursor.fetchone()
		except:
			print("db_get_values(): db_cursor.fetchone(): Failed #3")
			db_cursor.close()
			db_conn.close()
			return None

		if not row or row == None:
			break

		entry = {}
		entry['setup_id'] = setup_id
		entry['freq'] = row[0]
		entry['width'] = row[1]
		entry['power'] = row[2]
		entry['count'] = row[3]
		entry['completed'] = row[4]

		data.append(entry)

	db_cursor.close()
	db_conn.close()

	return data


def usage():
	print "Usage: %s <setup id> <plot height> <plot width> <file> [ <bands> <limits> <max> <cur> <x_coord> <y_coord> <x_grid> <y_grid> ]" % sys.argv[0]


def process_args():
	global args

	args['setup_id'] = sys.argv[1]
	args['height'] = int(sys.argv[2])
	args['width'] = int(sys.argv[3])
	args['file'] = sys.argv[4]
	args['bands'] = True
	args['limits'] = True
	args['max'] = True
	args['cur'] = True
	args['x_coord'] = True
	args['y_coord'] = True
	args['x_grid'] = True
	args['y_grid'] = True

	if len(sys.argv) > 5:
		args['bands'] = sys.argv[5] == '1'
		args['limits'] = sys.argv[6] == '1'
		args['max'] = sys.argv[7] == '1'
		args['cur'] = sys.argv[8] == '1'
		args['x_coord'] = sys.argv[9] == '1'
		args['y_coord'] = sys.argv[10] == '1'
		args['x_grid'] = sys.argv[11] == '1'
		args['y_grid'] = sys.argv[12] == '1'


if __name__ == '__main__':
	if len(sys.argv) < 5:
		usage()
		sys.exit(1)

	process_args()

	setup_data = db_get_setup_data(args['setup_id'])
	scan_data = db_get_scan_data(args['setup_id'])
	state_data = db_get_state_data(args['setup_id'])

	fqchart = fqchart(args['height'], args['width'], margin_x_tolerance, margin_y_tolerance, setup_data, scan_data, state_data, args['bands'], args['limits'], args['max'], args['cur'], args['x_coord'], args['y_coord'], args['x_grid'], args['y_grid'])
	fqchart.do()
	fqchart.export(args['file'])

	db_set_axis_data(args['setup_id'], fqchart.xo, fqchart.xoo, fqchart.yo, fqchart.yoo)

	sys.exit(0)


