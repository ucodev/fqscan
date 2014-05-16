#!/usr/bin/python -O

#
#	fqplot - FQScan Plotter
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: fqplotd.py
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


import time
import os
import commands
import syslog

from mysql import *
from config import *

def db_get_control_status():
	db_conn = mysql_connect(MYSQL_HOST, MYSQL_PORT, MYSQL_USER, MYSQL_PASS, MYSQL_DB)

	query = "SELECT * FROM control WHERE processed = 0 LIMIT 1"

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		syslog.syslog("db_get_control_status(): db_cursor.execute(\"%s\"): Failed" % query)
		db_cursor.close()
		db_conn.close()
		return None

	try:
		row = db_cursor.fetchone()
	except:
		syslog.syslog("db_get_control_status(): db_cursor.fetchone(): Failed")
		db_cursor.close()
		db_conn.close()
		return None

	if not row or row == None:
		return None

	entry = {}

	entry['id']				= row[0]
	entry['chart_button_bands']		= row[1]
	entry['chart_button_limits']		= row[2]
	entry['chart_button_max']		= row[3]
	entry['chart_button_cur']		= row[4]
	entry['chart_button_x_coord']		= row[5]
	entry['chart_button_y_coord']		= row[6]
	entry['chart_button_x_grid']		= row[7]
	entry['chart_button_y_grid']		= row[8]
	entry['params_freq_range_start']	= row[9]
	entry['params_freq_range_end']		= row[10]
	entry['params_coarse_bw']		= row[11]
	entry['params_threshold']		= row[12]
	entry['params_freq_corr']		= row[13]
	entry['params_freq_shift']		= row[14]
	entry['params_average_conut']		= row[15]
	entry['params_fine_bw']			= row[16]
	entry['params_fft_width']		= row[17]
	entry['params_gain']			= row[18]
	entry['params_filter']			= row[19]
	entry['params_sample_rate']		= row[20]
	entry['params_spread']			= row[21]
	entry['params_step']			= row[22]
	entry['params_if_gain']			= row[23]
	entry['params_p_time']			= row[24]
	entry['params_filter_a1']		= row[25]
	entry['params_filter_a2']		= row[26]
	entry['params_filter_a3']		= row[27]
	entry['params_filter_a4']		= row[28]
	entry['params_filter_a5']		= row[29]
	entry['params_band_info']		= row[30]
	entry['opers_button_basic']		= row[31]
	entry['opers_button_advanced']		= row[32]
	entry['opers_button_clear']		= row[33]
	entry['opers_button_reset']		= row[34]
	entry['opers_button_update']		= row[35]
	entry['opers_button_start']		= row[36]
	entry['opers_button_stop']		= row[37]
	entry['res_chart_width']		= row[38]
	entry['res_chart_height']		= row[39]
	entry['interface_button_header']	= row[40]
	entry['interface_button_footer']	= row[41]
	entry['interface_button_full']		= row[42]

	# Get last_setup_id
	query = "SELECT id FROM setup ORDER BY id DESC LIMIT 1"

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		syslog.syslog("db_get_control_status(): db_cursor.execute(\"%s\"): Failed #2" % query)
		db_cursor.close()
		db_conn.close()
		return None

	try:
		row = db_cursor.fetchone()
	except:
		syslog.syslog("db_get_control_status(): db_cursor.fetchone(): Failed #2")
		db_cursor.close()
		db_conn.close()
		return None

	if not row or row == None:
		return None

	entry['setup_id'] = row[0]
	#entry['setup_id'] = 15

	# Set row as processed
	query = "UPDATE control SET processed = 1 WHERE id = " + str(entry['id'])

	db_cursor = db_conn.cursor()

	try:
		db_cursor.execute(query)
	except:
		syslog.syslog("db_get_control_status(): db_cursor.execute(\"%s\"): Failed #3" % query)
		db_cursor.close()
		db_conn.close()
		return None

	db_conn.commit()
	db_cursor.close()
	db_conn.close()

	return entry

def do_loop():
	# Wait for some data ...
	while True:
		entry = db_get_control_status()

		if entry:
			break

		time.sleep(LOOP_INTERVAL)

	# Regenerate charts every LOOP_INTERVAL seconds. Update entry data if control changes occur.
	while True:
		(status, output) = commands.getstatusoutput(	\
			FQPLOT_BIN 				+ " " + \
			str(entry['setup_id']) 			+ " " + \
			str(entry['res_chart_height'])		+ " " + \
			str(entry['res_chart_width'])		+ " " + \
			PATH_FQPLOT_IMG_TMP			+ " " + \
			str(entry['chart_button_bands'])	+ " " + \
			str(entry['chart_button_limits'])	+ " " + \
			str(entry['chart_button_max'])		+ " " + \
			str(entry['chart_button_cur'])		+ " " + \
			str(entry['chart_button_x_coord'])	+ " " + \
			str(entry['chart_button_y_coord'])	+ " " + \
			str(entry['chart_button_x_grid'])	+ " " + \
			str(entry['chart_button_y_grid']) 	\
		)

		if not os.WEXITSTATUS(status):
			commands.getstatusoutput(		\
				CP_BIN				+ " " + \
				PATH_FQPLOT_IMG_TMP		+ " " + \
				PATH_FQPLOT_IMG_WWW		\
			)

		time.sleep(LOOP_INTERVAL)

		entry_new = db_get_control_status()

		if entry_new != None:
			entry = entry_new


### MAIN ###
if __name__ == "__main__":
	do_loop()


