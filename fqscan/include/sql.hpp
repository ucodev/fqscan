/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: sql.hpp
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 *
 *	Last Update: 2014/03/28
 * 
 *	This project contains parts of, or links to, the following projects, licensed under, at least, one GPL version:
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

#ifndef FQSCAN_MYSQL_HPP
#define FQSCAN_MYSQL_HPP

#include <iostream>
#include <streambuf>

#include <mysql/mysql.h>

#include "config.hpp"

using namespace std;

class sqlbuf : public streambuf {
	protected:

	static const int _size = CONFIG_MAX_QUERY_SIZE;
	char _buf[CONFIG_MAX_QUERY_SIZE];

	int flush(void);

	virtual int sync(void);

	virtual int_type overflow(int_type c);

	public:

	MYSQL *myconn, myinit;
	MYSQL_RES *myres;
	MYSQL_ROW myrow;
	bool myqstate;	// Last query state

	sqlbuf();
};

class sqlstream : public ostream {
	protected:

	sqlbuf _sqlb;

	public:

	void close(void);
	bool connected(void);
	bool query_status(void);
	bool store_result(void);
	void free_result(void);
	MYSQL_ROW yield_row(void);
	MYSQL_ROW current_row(void);
	int connect(const char *user, const char *pass, const char *host, const char *db);
	
	sqlstream();
	sqlstream(const char *user, const char *pass, const char *host, const char *db);

	~sqlstream(void);
};

/* Prototypes */
bool sql_inject_safe_val(const string& val);

#endif
