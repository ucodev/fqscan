<?php

/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: config.php
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 *
 *	Last Update: 2014/04/15
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


class config {
	public $db_host = 'localhost';
	public $db_user = 'fqscan';
	public $db_pass = 'password';
	public $db_name = 'fqscan_v1';
	public $db_port = 3306;

	public $bin_daemonizer = '/usr/local/bin/daemonizer';
	public $bin_fqscan = '/usr/local/bin/fqscan';
	public $bin_fqplotd = '/usr/local/bin/fqplotd.py';
	public $bin_cp = '/bin/cp';
	public $bin_kill = '/bin/kill';
	public $bin_killall = '/usr/bin/killall';
	public $bin_pidof = '/bin/pidof';

	public $file_pid_fqplotd = '/tmp/fqplotd.pid';
	public $file_img_clear = '/var/www/fqscan/images/clear.png';
	public $file_img_current = '/var/www/fqscan/images/current.png';

	public $proc_name_fqplotd = 'fqplotd.py';
	public $proc_name_fqscan = 'fqscan';

	public $name = 'FQScan';
	public $version = 'v0.3';
	public $url = 'http://www.ucodev.org';

	public $author_name = 'Pedro A. Hortas';
	public $author_email = 'pah@ucodev.org';
}

