/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: sql.cpp
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

#include <iostream>
#include <streambuf>

#include <mysql/mysql.h>

#include "config.hpp"
#include "sql.hpp"

using namespace std;

/*
 * sqlbuf class
 */
int sqlbuf::flush(void) {
	int len = pptr() - pbase();	// length of buffer to be flushed

	myqstate = (mysql_query(myconn, _buf) == 0);

	pbump(-len);	// reset buffer position

	return len;
}

int sqlbuf::sync(void) {
	int len = pptr() - pbase();	// length of buffer to be flushed

	/* Remove trailing \n */
	if (_buf[len - 1] == '\n')
		_buf[len - 1] = 0;

	return flush() > 0 ? 0 : -1;
}

int sqlbuf::overflow(int c) {
	/* insert overflowing char into buffer (1 slot available due to
	 * (_size - 1) in the constructor
	 */
	*pptr() = c;
	pbump(1);

	/* Flush buffer */
	flush();

	/* Return overflowing char */
	return c;
}

sqlbuf::sqlbuf() {
	myconn = NULL;
	myres = NULL;
	myqstate = false;

	/* -1 is to grant that the overflowing char can still be
	 * put in the buffer
	 */
	setp(_buf, _buf + (_size - 1));
}

/*
 * sqlstream class
 */
bool sqlstream::connected(void) {
	/* Validate connection result */
	return (_sqlb.myconn != NULL);
}

bool sqlstream::query_status(void) {
	/* Validate last query result */
	return _sqlb.myqstate;
}

bool sqlstream::store_result(void) {
	/* Grant that the last query was executed sucessfully */
	if (_sqlb.myqstate == true) {
		/* Store query result */
		_sqlb.myres = mysql_store_result(_sqlb.myconn);

		return true;
	}

	return false;
}

void sqlstream::free_result(void) {
	mysql_free_result(_sqlb.myres);
}

MYSQL_ROW sqlstream::current_row(void) {
	/* Return the current row */
	return _sqlb.myrow;
}

MYSQL_ROW sqlstream::yield_row(void) {
	/* Yield a new row, if available. If not, NULL is returned */
	_sqlb.myrow = mysql_fetch_row(_sqlb.myres);

	return _sqlb.myrow;
}

void sqlstream::close(void) {
	/* Close database connection */
	mysql_close(_sqlb.myconn);
}

int sqlstream::connect(const char *user, const char *pass, const char *host, const char *db) {
	/* Initialize mysql client */
	mysql_init(&_sqlb.myinit);

	/* Connect to mysql server */
	_sqlb.myconn = mysql_real_connect(&_sqlb.myinit, host, user, pass, db, 0, 0, 0);

	rdbuf(&_sqlb);

	return 0;
}
	
sqlstream::sqlstream() {
	return;
}

sqlstream::sqlstream(const char *user, const char *pass, const char *host, const char *db) {
	/* Initialize mysql client */
	mysql_init(&_sqlb.myinit);

	/* Connect to mysql server */
	_sqlb.myconn = mysql_real_connect(&_sqlb.myinit, host, user, pass, db, 0, 0, 0);

	rdbuf(&_sqlb);
}

sqlstream::~sqlstream() {
	mysql_close(_sqlb.myconn);
}

bool sql_inject_safe_val(const string& val) {
	/* TODO: This function will fail on some cases where valid data
	 * is tested but this will incorrectly identify inject unsafe val.
	 * This function shall be improved in a near future.
	 */

	/* Check for character data string delimiter */
	if (val.find("\'", 0) != val.npos)
		return false;

	/* Check for comment delimiter */
	if (val.find("--", 0) != val.npos)
		return false;

	/* Check for query delimiter */
	if (val.find(";", 0) != val.npos)
		return false;

	/* Check for open comment delimiter */
	if (val.find("/*", 0) != val.npos)
		return false;

	/* Check for close comment delimiter */
	if (val.find("*/", 0) != val.npos)
		return false;

	/* Check for data string delimiter */
	if (val.find("\"", 0) != val.npos)
		return false;

	/* Check for stored procedures prefix (SQLSERVER) */
	if (val.find("xp_", 0) != val.npos)
		return false;

	return true;
}

