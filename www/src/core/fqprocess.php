<?php

/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fqprocess.php
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


include('core/fqctrl.php');

class fqprocess extends fqctrl {
	function __construct($REQUEST) {
		parent::__construct($REQUEST);

		$this->op();
	}

	function fsm() {
		/* FSM */
		if ($_REQUEST['opers_button_stop'] == 'on') {
			if ($this->fqscan_running())
				$this->fqscan_stop();
		}

		if ($_REQUEST['opers_button_start'] == 'on') {
			if (!$this->fqscan_running())
				$this->fqscan_start();
		}

		if ($_REQUEST['opers_button_clear'] == 'on') {
			if (!$this->fqscan_running()) {
				$this->fqscan_clear();
			} else {
				header("HTTP/1.1 403 Forbidden");
				error_log("Can't clear data while scanner is running.");
				die("Can't clear data while scanner is running.");
			}
		}
	}

	function op() {
		if ($_REQUEST['op'] == 'commit') {
			$this->db_commit_data();

			$this->fsm();
		} else if ($_REQUEST['op'] == 'retrieve') {
			$this->db_retrieve_data();
		}
	}
}

