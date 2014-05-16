/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: runtime.hpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 *
 *	Last Update: 2014/03/28
 * 
 *	This project links to, at least, the following libraries, licensed under, at least, one GPL version:
 *	  - libfqscan - http://libfqscan.ucodev.org/
 *	  - librfsp - http://librfsp.ucodev.org/
 *	  - gnuradio - http://gnuradio.org/
 *	  - osmocomsdr - http://sdr.osmocom.org/trac/
 *	  - libmysqlclient - http://www.mysql.com/
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


#ifndef FQSCAN_RUNTIME_HPP
#define FQSCAN_RUNTIME_HPP

#include <libfqscan/fq_scan.hpp>

#include "args.hpp"
#include "sql.hpp"

/* Prototype */
class runtime {
	public:
		int setup(int argc, char **argv);
		void daemonize(void);
		void scan_instance(void);
		void scan_update_data(void);
		void scan(void);
		void cleanup(void);

		sqlstream sql;
		args arg;

		int setup_id;
		unsigned long scan_count;
};

extern runtime run;

#endif
