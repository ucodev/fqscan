<?php

/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fqctrl.php
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

class fqctrl extends fqcore {
	private $_request;

	private function _sanitize_user_data($data) {
		foreach ($data as $k => $v) {
			if (!preg_match('/^[a-z0-9\-\.]+$/', $v)) {
				header('HTTP/1.1 403 Forbidden');
				error_log('Invalid form data detected: ' . $k . ' => ' . $v);
				die('Invalid form data. Please check the inserted values.');
			}
		}
	}

	function __construct($request) {
		parent::__construct();

		$this->_sanitize_user_data($request);

		$this->_request = $request;
	}

	function db_commit_data() {
		$ret = $this->db->query(
			"INSERT INTO control (" .
			"chart_button_bands,chart_button_limits,chart_button_max,chart_button_cur," .
			"chart_button_x_coord,chart_button_y_coord,chart_button_x_grid,chart_button_y_grid," .
			"params_freq_range_start,params_freq_range_end,params_coarse_bw,params_threshold," .
			"params_freq_corr,params_freq_shift,params_average_count,params_fine_bw,params_fft_width," .
			"params_gain,params_filter,params_sample_rate,params_spread,params_step,params_if_gain," .
			"params_p_time," .
			"params_filter_a1,params_filter_a2,params_filter_a3,params_filter_a4,params_filter_a5," .
			"params_band_info," .
			"opers_button_basic,opers_button_advanced,opers_button_clear,opers_button_reset," .
			"opers_button_update,opers_button_start,opers_button_stop," .
			"res_chart_width,res_chart_height," .
			"interface_button_header,interface_button_footer,interface_button_full) " .
			"VALUES (" .
			($this->_request['chart_button_bands'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_limits'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_max'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_cur'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_x_coord'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_y_coord'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_x_grid'] == 'on' ? '1' : '0')					. "," .
			($this->_request['chart_button_y_grid'] == 'on' ? '1' : '0')					. "," .
			$this->_request['params_freq_range_start']							. "," .
			$this->_request['params_freq_range_end']							. "," .
			$this->_request['params_coarse_bw']								. "," .
			$this->_request['params_threshold']								. "," .
			$this->_request['params_freq_corr']								. "," .
			$this->_request['params_freq_shift']								. "," .
			$this->_request['params_average_count']								. "," .
			$this->_request['params_fine_bw']								. "," .
			$this->_request['params_fft_width']								. "," .
			$this->_request['params_gain']									. "," .
			"'" . $this->_request['params_filter'] . "'"							. "," .
			$this->_request['params_sample_rate']								. "," .
			$this->_request['params_spread']								. "," .
			$this->_request['params_step']									. "," .
			$this->_request['params_if_gain']								. "," .
			$this->_request['params_p_time']								. "," .
			($this->_request['params_filter_a1'] == '-' ? 'NULL' : $this->_request['params_filter_a1'])	. "," .
			($this->_request['params_filter_a2'] == '-' ? 'NULL' : $this->_request['params_filter_a2'])	. "," .
			($this->_request['params_filter_a3'] == '-' ? 'NULL' : $this->_request['params_filter_a3'])	. "," .
			($this->_request['params_filter_a4'] == '-' ? 'NULL' : $this->_request['params_filter_a4'])	. "," .
			($this->_request['params_filter_a5'] == '-' ? 'NULL' : $this->_request['params_filter_a5'])	. "," .
			"'" . $this->_request['params_band_info'] . "'"							. "," .
			($this->_request['opers_button_basic'] == 'on' ? '1' : '0')					. "," .
			($this->_request['opers_button_advanced'] == 'on' ? '1' : '0')					. "," .
			($this->_request['opers_button_clear'] == 'on' ? '1' : '0')					. "," .
			($this->_request['opers_button_reset'] == 'on' ? '1' : '0')					. "," .
			($this->_request['opers_button_update'] == 'on' ? '1' : '0')					. "," .
			($this->_request['opers_button_start'] == 'on' ? '1' : '0')					. "," .
			($this->_request['opers_button_stop'] == 'on' ? '1' : '0')					. "," .
			$this->_request['res_chart_width']								. "," .
			$this->_request['res_chart_height']								. "," .
			($this->_request['interface_button_header'] == 'on' ? '1' : '0')				. "," .
			($this->_request['interface_button_footer'] == 'on' ? '1' : '0')				. "," .
			($this->_request['interface_button_full'] == 'on' ? '1' : '0')					. ")"
		);

		if (!$ret) {
			$error_str = "Error while inserting data into MySQL: (" . $this->db->errno . ") " . $this->db->error;
			header("HTTP/1.1 403 Forbidden");
			error_log($error_str);
			die($error_str);
		}

		echo($this->current_scan_count());
	}

	function db_retrieve_data() {
		$data = $this->current_scan_state();
		$data['nr_scans'] = $this->current_scan_count();

		echo(json_encode($data));
	}

	function current_scan_state() {
		$res = $this->db->query('SELECT xo,xoo,yo,yoo FROM state WHERE xo IS NOT NULL ORDER BY id DESC LIMIT 1');

		$row = $res->fetch_assoc();

		if (!isset($row)) {
			$row['xo'] = 0;
			$row['xoo'] = 0;
			$row['yo'] = 0;
			$row['yoo'] = 0;
		}

		return $row;
	}

	function current_scan_count() {
		$res = $this->db->query('SELECT count FROM scan_data ORDER BY id DESC LIMIT 1');

		$row = $res->fetch_assoc();

		if (!isset($row)) {
			$row['count'] = 0;
		}

		return $row['count'];
	}

	function fqscan_running() {
		exec($this->config->bin_pidof . ' ' . $this->config->proc_name_fqscan, $pid_list);

		if (empty($pid_list)) {
			return false;
		}

		return true;
	}

	function fqscan_clear() {
		$this->db->query("DELETE FROM scan_data");
		$this->db->query("DELETE FROM control");
		$this->db->query("DELETE FROM state");
		$this->db->query("DELETE FROM setup");

		exec($this->config->bin_cp . ' ' . $this->config->file_img_clear . ' ' . $this->config->file_img_current);
	}

	function fqscan_stop() {
		exec($this->config->bin_kill . ' -TERM `' . $this->config->bin_pidof . ' ' . $this->config->proc_name_fqscan . '`');
		exec($this->config->bin_killall . ' -TERM ' . $this->config->proc_name_fqplotd);
	}

	function fqscan_start() {
		$cmd =	$this->config->bin_fqscan 				. " " .
			"-x " . $this->_request['params_freq_range_start']	. " " .
			"-y " . $this->_request['params_freq_range_end']	. " " .
			"-c " . $this->_request['params_coarse_bw']		. " " .
			"-t " . $this->_request['params_threshold']		. " " .
			"-m " . $this->_request['params_freq_corr']		. " " .
			"-i " . $this->_request['params_freq_shift']		. " " .
			"-a " . $this->_request['params_average_count']		. " " .
			"-f " . $this->_request['params_fine_bw']		. " " .
			"-w " . $this->_request['params_fft_width']		. " " .
			"-g " . $this->_request['params_gain']			. " " .
			"-r " . $this->_request['params_sample_rate']		. " " .
			"-s " . $this->_request['params_spread']		. " " .
			"-z " . $this->_request['params_step']			. " " .
			"-n " . $this->_request['params_if_gain']		. " " .
			"-p " . $this->_request['params_p_time']		. " " .
			"-k 0"							. " " .
			"--use-mysql"						. " " .
			"--mysql-db=" . $this->config->db_name			. " " .
			"--mysql-host=" . $this->config->db_host		. " " .
			"--mysql-passwd=" . $this->config->db_pass		. " " .
			"--mysql-port=" . $this->config->db_port		. " " .
			"--mysql-user=" . $this->config->db_user		. " " .
			"--filter-" . $this->_request['params_filter'];

		$custom_filter = false;

		if (is_numeric($this->_request['params_filter_a1'])) {
			$custom_filter = true;
			$cmd .= "-custom";
			$cmd .= " --filter-param-1=" . $this->_request['params_filter_a1'];
		}

		for ($i = 2; $i <= 5; $i ++) {
			if (is_numeric($this->_request['params_filter_a' . $i])) {
				if (!$custom_filter) {
					$custom_filter = true;
					$cmd .= "-custom";
				}

				$cmd .= " --filter-param-" . $i . "=" . $this->_request['params_filter_a' . $i];
			}
		}

		exec($cmd . " --daemonize");
		exec($this->config->bin_daemonizer . ' -p ' . $this->config->file_pid_fqplotd . ' ' . $this->config->bin_fqplotd);
	}
}

