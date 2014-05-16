/*
 *	fqscan - RF Signal Scanner and Plotter
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: runtime.cpp
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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <libfqscan/fq_scan.hpp>

#include "args.hpp"
#include "sql.hpp"
#include "runtime.hpp"
#include "hook.hpp"

/* Global runtime object */
runtime run;

int runtime::setup(int argc, char **argv) {
	run.arg.pre_process(argc, argv);
	run.arg.post_process();

	if (run.arg.proc_daemonize)
		run.daemonize();

	if (run.arg.use_mysql) {
		run.sql.connect(run.arg.mysql_user, run.arg.mysql_pass, run.arg.mysql_host, run.arg.mysql_db);

		run.sql <<
			"INSERT INTO setup (" \
			"average, coarse_bandwidth, fine_bandwidth, spread, threshold, freq_start, " \
			"freq_end, sample_rate, fft_width, step, ptime, freq_correction, gain, if_gain, " \
			"freq_shift, filters_id, filter_args";

		if (run.arg.filter_args)
			run.sql << ", filter_arg1, filter_arg2, filter_arg3, filter_arg4, filter_arg5";

		run.sql <<
			") VALUES (" <<
			run.arg.avg_size << "," <<
			run.arg.bandwidth_coarse << "," <<
			run.arg.bandwidth_fine << "," <<
			run.arg.spread << "," <<
			run.arg.threshold << "," <<
			run.arg.freq_start << "," <<
			run.arg.freq_end << "," <<
			run.arg.sample_rate << "," <<
			run.arg.fft_width << "," <<
			run.arg.step << "," <<
			run.arg.ptime << "," <<
			run.arg.freq_correction << "," <<
			run.arg.gain << "," <<
			run.arg.if_gain << "," <<
			run.arg.freq_shift << "," <<
			run.arg.filter << "," <<
			(run.arg.filter_args ? 1 : 0);

		if (run.arg.filter_args) {
			run.sql << "," <<
				run.arg.filter_a1 << "," <<
				run.arg.filter_a2 << "," <<
				run.arg.filter_a3 << "," <<
				run.arg.filter_a4 << "," <<
				run.arg.filter_a5;
		}

		run.sql << ")" << endl;

		run.sql.store_result();
		run.sql.free_result();

		run.sql << "SELECT LAST_INSERT_ID()" << endl;

		run.sql.store_result();

		run.sql.yield_row();

		run.setup_id = atoi(run.sql.current_row()[0]);

		run.sql.free_result();
	}

	return 0;
}

void runtime::daemonize(void) {
	pid_t pid;

	if (!freopen("/dev/zero", "r", stdin))
		return; // TODO: Log failed attempt

	if (!freopen("/dev/null", "a", stdout))
		return; // TODO: Log failed attempt

	if (!freopen("/dev/null", "a", stderr))
		return; // TODO: Log failed attempt

	if ((pid = fork()) > 0) {
		exit(EXIT_SUCCESS);
	} else if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	setsid();
}

void runtime::scan_instance(void) {
#ifdef CONFIG_RTLSDR_SUPPORT
	/* RTLSDR: Hack the hack :( */
	int status = 0;
	pid_t pid = fork();

	if (pid > 0) {
		waitpid(-1, &status, 0);
	} else if (!pid) {
#endif
		fq_scan fqscan(
			run.arg.freq_start,
			run.arg.freq_end,
			run.arg.sample_rate,
			run.arg.fft_width,
			run.arg.bandwidth_fine,
			run.arg.bandwidth_coarse,
			run.arg.step,
			run.arg.avg_size,
			run.arg.spread,
			run.arg.threshold,
			run.arg.ptime,
			run.arg.freq_correction,
			false,	// Gain mode
			run.arg.gain,
			run.arg.if_gain,
			run.arg.filter,
			run.arg.filter_args,
			run.arg.filter_a1,
			run.arg.filter_a2,
			run.arg.filter_a3,
			run.arg.filter_a4,
			run.arg.filter_a5,
			&fq_signal_hook,
			&fq_freq_range_hook);

		fqscan.start();
		fqscan.wait();
		fqscan.stop();
		fqscan.reset();

		fqscan.~fq_scan();
#ifdef CONFIG_RTLSDR_SUPPORT
		exit(EXIT_SUCCESS);
	}
#endif
}

void runtime::scan_update_data(void) {
	run.sql <<
		"UPDATE scan_data SET completed = 1 WHERE " <<
		"count = " << run.scan_count << " AND " <<
		"setup_id = " << run.setup_id <<
		endl;

	run.sql.store_result();
	run.sql.free_result();
}

void runtime::scan(void) {
	/* If nr-scans is 0, loop forever */
	if (!run.arg.nr_scans) {
		for (run.scan_count = 0; ; run.scan_count ++) {
			scan_instance();

			/* Set this scan iteration as completed */
			scan_update_data();
		}

		// TODO: Unreachable for now (implement signals.cpp) */
		return;
	}

	for (run.scan_count = 0; run.scan_count < run.arg.nr_scans; run.scan_count ++) {
		scan_instance();

		/* Set this scan iteration as completed */
		scan_update_data();
	}
}

void runtime::cleanup(void) {
	sql.close();
}

