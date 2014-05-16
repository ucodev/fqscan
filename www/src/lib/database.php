<?php

/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: database.php
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


class database extends mysqli {
	protected $config;
	private $_request;

	function __construct($config) {
		parent::__construct($config->db_host, $config->db_user, $config->db_pass, $config->db_name);

		if ($this->connect_errno) {
			$error_str = "Error while connecting to MySQL: (" . $this->connect_errno . ") " . $this->connect_error;
			header("HTTP/1.1 403 Forbidden");
			error_log($error_str);
			die($error_str);
		}

		$this->config = $config;
	}
}

