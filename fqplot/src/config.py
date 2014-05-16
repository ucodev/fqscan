#!/usr/bin/python

#
#	fqplot - FQScan Plotter
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: config.py
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

MYSQL_HOST = "localhost"
MYSQL_PORT = 3306
MYSQL_USER = "fqscan"
MYSQL_PASS = "password"
MYSQL_DB   = "fqscan_v1"

CHART_FONT = "Matrix Capitalized"

CP_BIN = "/bin/cp"
FQPLOT_BIN = "/usr/local/bin/fqplot.py"
PATH_FQPLOT_IMG_TMP = "/tmp/fqplottmp.png"
PATH_FQPLOT_IMG_WWW = "/var/www/fqscan/images/current.png"

LOOP_INTERVAL = 0.0001

FQSCAN_VERSION = "0.31"

