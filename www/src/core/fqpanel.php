<?php

/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fqpanel.php
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


include('core/fqcore.php');

class fqpanel extends fqcore {
	public $filters;

	function __construct() {
		parent::__construct();

		$this->filters = array();

		$this->populate();
	}

	function db_get_filters() {
		$res = $this->db->query('SELECT filter,def FROM filters');

		while ($row = $res->fetch_assoc())
			array_push($this->filters, $row);
	}

	function populate() {
		$this->db_get_filters();
	}
}


