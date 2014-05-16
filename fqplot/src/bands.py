#!/usr/bin/python

#
#	fqplot - FQScan Plotter
#	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
# 
#	File: bands.py
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


band_range = [
	('TLF', 0.0000003, 0.000003),
	('ELF', 0.000003, 0.00003),
	('SLF', 0.00003, 0.0003),
	('ULF', 0.0003, 0.003),
	('VLF', 0.003, 0.03),
	('LF',  0.03, 0.3),
	('MF',  0.3, 3),
	('HF',  3, 30),
	('VHF', 30, 300),
	('UHF', 300, 3000),
	('SHF', 3000, 30000),
	('EHF', 30000, 300000),
	('THF', 300000, 3000000)
]

app_band_range = [
	('ITU-R2-HR-220000', 0.1357, 0.1378),
	('ITU-R2-HR-16000', 1.8, 2.0),
	('ITU-R2-HR-8000', 3.5, 4.0),
	('ITU-R2-HR-6000', 5.332, 5.405), # Channels 5.332, 5.348, 5.3585, 5.373, 5.405 MHz
	('ITU-R2-HR-4000', 7.0, 7.3),
	('ITU-R2-HR-3000', 10.1, 10.15),
	('ITU-R2-HR-2000', 14, 14.35),
	('ITU-R2-HR-1700', 18.068, 18.168),
	('ITU-R2-HR-1500', 21, 21.45),
	('ITU-R2-HR-1200', 24.89, 24.99),
	('ITU-R2-HR-1000', 28, 29.7),
	('ITU-R2-HR-600', 50.0, 54.0),
	('FM-Radio', 87.0, 108.0),
	('Airband', 108.0, 137.0),
	('NOAA Sat', 137.0, 138.0),
	('ITU-R2-HR-200', 144.0, 146.0),
	('ITU-R2-HR-125-LE', 219.0, 220.0),
	('ITU-R2-HR-125-UE', 222.0, 225.0),
	('ITU-R2-HR-70', 420.0, 450.0),
	#('T-GSM-380-UL', 380.2, 389.8),
	#('T-GSM-380-DL', 390.2, 399.8),
	#('T-GSM-410-UL', 410.2, 419.8),
	#('T-GSM-410-DL', 420.2, 429.8),
	#('GSM-450-UL', 450.6, 457.6),	# Very Uncommon
	#('GSM-450-DL', 460.6, 467.6),	# Very Uncommon
	('GSM-480-UL', 479.0, 486.0),
	('GSM-480-DL', 489.0, 496.0),
	('GSM-710-UL', 698.2, 716.2),
	('GSM-710-DL', 728.2, 746.2),
	('GSM-750-UL', 747.2, 762.2),
	('GSM-750-DL', 777.2, 792.2),
	#('T-GSM-810-UL', 806.2, 821.2),
	#('T-GSM-810-DL', 851.2, 866.2),
	('GSM-850-UL', 824.2, 849.2),
	('GSM-850-DL', 869.2, 894.2),
	#('P-GSM-900-UL', 890.0, 915.0),
	#('P-GSM-900-DL', 935.0, 960.0),
	#('E-GSM-900-UL', 880.0, 915.0),
	#('E-GSM-900-DL', 925.0, 960.0),
	('R-GSM-900-UL', 876.0, 915.0),
	#('ITU-R2-HR-33', 902.0, 928.0),
	('R-GSM-900-DL', 921.0, 960.0),
	#('T-GSM-900-UL', 870.4, 876.0),
	#('T-GSM-900-DL', 915.4, 921.0),
	#('DCS-1800-UL', 1710.2, 1784.8),
	#('DCS-1800-DL', 1805.2, 1879.8),
	#('PCS-1900-UL', 1850.2, 1909.8),
	#('PCS-1900-DL', 1930.2, 1989.8),
	('GPS-L5', 1176.0, 1177.0), # 1176.45
	('GPS-L2', 1227.0, 1228.0), # 1227.6
	('ITU-R2-HR-23', 1240.0, 1300.0),
	('GPS-L4', 1379.5, 1380.5), # 1379.913
	('GPS-L3', 1380.6, 1381.6), # 1381.05
	('GPS-L1', 1575.0, 1576.0), # 1575.42
	#('DCS-1800-UL', 1710.2, 1784.8),
	#('DCS-1800-DL', 1805.2, 1879.8),
	#('PCS-1900-UL', 1850.2, 1909.8),
	#('PCS-1900-DL', 1930.2, 1989.8),
	('GSM-xCS-1x00', 1710.2, 1989.8),
	('ITU-R2-HR-13-LE', 2300.0, 2310.0),
	#('ITU-R2-HR-13-UE', 2390.0, 2450.0),
	('802.11-bgn', 2400.0, 2483.5),
	('ITU-R2-HR-9-LE', 3300.0, 3500.0),
	('MLS', 5000.0, 5150.0),
	('TDWR', 5150.0, 5250.0),
	('ITU-R2-HR-5-LE', 5650.0, 5925.0),
	('ITU-R2-HR-3-LE', 10000.0, 10500.0)
]

