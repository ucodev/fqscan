/**
 * @file daemonizer.c
 * @brief daemonizer command line utility
 *
 * Date: 23-10-2012
 * License: BSD 3-Clause
 * 
 * Copyright (c) 2012, Pedro A. Hortas (pah@ucodev.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of the ucodev.org nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 *
 * Project details:
 *
 *  Home Page:   http://www.ucodev.org
 *  Version:     0.9.5
 *  Portability: POSIX.1-2001 and POSIX.1-2008 compliant
 *  Description: Creates and daemonizes a child process in order to execute the
 *		 program, identified by the command line arguments, in
 *		 background. A new session is created and standard file
 *		 descriptors are redirected (see usage). It supports setreuid()
 *		 and setregid() from command line options and the management of
 *		 an optional pid file. The following signals, sent to
 *		 daemonizer, are redirected to the child process: SIGHUP,
 *		 SIGINT, SIGQUIT, SIGABRT, SIGTERM, SIGUSR1, SIGUSR2 and
 *		 SIGCONT. Optional restart flags can be used in order to
 *		 restart the child process based on its exit code value.
 *
 *
 * Compilation instructions:
 *
 *  $ cc -o daemonizer daemonizer.c
 *
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

extern char *optarg;
extern int optind, optopt;


/* Configuration flags */
#define CONFIG_FL_PIDF_CREATE	0x01
#define CONFIG_FL_PIDF_FORCE	0x02
#define CONFIG_FL_REDIR_IN	0x04
#define CONFIG_FL_REDIR_OUT	0x08
#define CONFIG_FL_REDIR_ERR	0x10
#define CONFIG_FL_PROC_RESTART	0x20
#define CONFIG_FL_PROC_RSTIGN	0x40


/* Declarations */
struct cmdline_params {
	uid_t uid;
	gid_t gid;
	pid_t cpid;
	char *inf_name;
	char *outf_name;
	char *errf_name;
	char *pidf_name;
	int fd_pidf;
	int flags;
	const char *binary;
	char *const *args;
} config;


/* Implementation */
static void _sigah(int sig, siginfo_t *si, void *ucontext) {
	kill(config.cpid, sig);
}

static void _close_safe(int fd) {
#if 0
	while (close(fd) < 0) {
		if (errno != EINTR)
			break;
	}
#endif
	/* SA_RESTART set */
	close(fd);
}

static int _strisdigit(const char *str) {
	while (*str) {
		if (!isdigit(*str ++))
			return 0;
	}

	return 1;
}

static void _config_default_init(void) {
	memset(&config, 0, sizeof(struct cmdline_params));

	config.uid = getuid();
	config.gid = getgid();
}

static inline void _config_set_req(const char *binary) {
	config.binary = binary;
}

static inline void _config_set_opt(char *const *argv, int optind) {
	config.args = &argv[optind];
}

static void _config_destroy(void) {
	if (config.flags & CONFIG_FL_PIDF_CREATE)
		free(config.pidf_name);

	if (config.flags & CONFIG_FL_REDIR_ERR)
		free(config.errf_name);

	if (config.flags & CONFIG_FL_REDIR_OUT) 
		free(config.outf_name);

	if (config.flags & CONFIG_FL_REDIR_IN)
		free(config.inf_name);

	_close_safe(config.fd_pidf);

	memset(&config, 0, sizeof(struct cmdline_params));
}

static void _failure(const char *caller) {
	fprintf(stderr, "[daemonizer] %s() error: %s\n", caller, strerror(errno));

	_config_destroy();

	exit(EXIT_FAILURE);
}

static void _file_pid_create(const char *file) {
	config.fd_pidf = open(file, O_RDONLY);

	if ((config.fd_pidf >= 0) && (config.flags & CONFIG_FL_PIDF_FORCE)) {
		if (unlink(file) < 0)
			_failure("unlink");
	} else if (config.fd_pidf >= 0) {
		fprintf(stderr, "PID file %s already exists.\n", file);
		_config_destroy();
		exit(EXIT_FAILURE);
	}

	_close_safe(config.fd_pidf);

	if ((config.fd_pidf = open(file, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) < 0) {
		fprintf(stderr, "Cannot create PID file %s.\n", file);
		_failure("open");
	}
}

static int _file_pid_write(pid_t pid) {
	int ret = 0;
	char spid[12];

	memset(spid, 0, sizeof(spid));

	snprintf(spid, 11, "%u", pid);

	ret = write(config.fd_pidf, spid, strlen(spid)) > 0 ? 0 : -1;

	_close_safe(config.fd_pidf);

	return ret;
}

static int _daemonize(void) {
	pid_t pid = 0;

	if ((pid = fork()) > 0) {
		_config_destroy();
		exit(EXIT_SUCCESS);
	} else if (pid == (pid_t) -1) {
		return -1;
	}

	if (!(freopen(config.flags & CONFIG_FL_REDIR_IN ? config.inf_name : "/dev/zero", "r", stdin)))
		return -1;

	if (!(freopen(config.flags & CONFIG_FL_REDIR_OUT ? config.outf_name : "/dev/null", "a", stdout)))
		return -1;

	if (!(freopen(config.flags & CONFIG_FL_REDIR_ERR ? config.errf_name : "/dev/null", "a", stderr)))
		return -1;

	if (setsid() == (pid_t) -1)
		return -1;

	return 0;
}

static int _bexec(
		const char *file,
		char *const *args,
		char *const *envp)
{
	int status = 0;

	if ((config.cpid = fork()) > 0) {
		if ((config.flags & CONFIG_FL_PIDF_CREATE) && (_file_pid_write(config.cpid) < 0))
			return -1;

		/* SA_RESTART set */
		if ((config.cpid = wait(&status)) == (pid_t) -1)
			return -1;

		if (WEXITSTATUS(status) == EXIT_SUCCESS)
			return 0;
	} else if (!config.cpid) {
		_config_destroy();

		if (setsid() == (pid_t) -1)
			_failure("setsid");

		if (execve(file, args, envp) < 0)
			_failure("execve");
	}

	return -1;
}

static void _usage(char *const *argv) {
	fprintf(stderr, "Usage: %s [OPTIONS] COMMAND [ARG]...\n" \
			"\n" \
			"  COMMAND    \tFull path to command line executable\n"\
			"  ARG...     \tCommand line arguments to the " \
			"executable\n" \
			"  OPTIONS\n" \
			"    -u <uid>   \tSet UID\n" \
			"    -g <gid>   \tSet GID\n" \
			"    -i <stdin> \tSource file for stdin redirection\n" \
			"    -o <stdout>\tTarget file for stdout redirection\n"\
			"    -e <stderr>\tTarget file for stderr redirection\n"\
			"    -r         \tRestart process if the child exit " \
			"code is different than zero.\n" \
			"    -R         \tRestart process regardless the " \
			"child exit code.\n" \
			"    -f         \tForce PID file rewrite\n" \
			"    -p <file>  \tPID file name\n" \
			"\n" \
			"Examples:\n" \
			"\n" \
			"  # %s /usr/local/sbin/example\n" \
			"  # %s -u 1000 -g 50 -f -p /var/run/example.pid -- " \
			"/usr/local/sbin/example -y opt\n" \
			"  # %s -u 1000 -g 50 -f -p /var/run/example.pid -o " \
			"/tmp/stdout.log -e /tmp/stderr.log -- " \
			"/usr/local/sbin/example -y opt\n" \
			"\n", argv[0], argv[0], argv[0], argv[0]);

	exit(EXIT_FAILURE);
}

static void _usage_invalid_opt(int opt, char *const *argv) {
	fprintf(stderr, "Unrecognized option: -%c\n", (char) opt);
	_usage(argv);
}

static void _usage_invalid_opt_arg(
		int opt,
		const char *arg,
		char *const *argv)
{
	fprintf(stderr, "Invalid value for option -%c: %s\n", (char) opt, arg);
	_usage(argv);
}

static void _cmdline_process(int argc, char *const *argv) {
	int opt = 0;

	while ((opt = getopt(argc, argv, "frRu:g:i:o:e:p:")) != -1) {
		if (opt == 'u') {
			if (!_strisdigit(optarg))
				_usage_invalid_opt_arg(opt, optarg, argv);

			config.uid = strtoul(optarg, NULL, 10);
		} else if (opt == 'g') {
			if (!_strisdigit(optarg))
				_usage_invalid_opt_arg(opt, optarg, argv);

			config.gid = strtoul(optarg, NULL, 10);
		} else if (opt == 'i') {
			config.flags |= CONFIG_FL_REDIR_IN;
			if (!(config.inf_name = malloc(strlen(optarg) + 1)))
				_failure("malloc");

			strcpy(config.inf_name, optarg);
		} else if (opt == 'o') {
			config.flags |= CONFIG_FL_REDIR_OUT;
			if (!(config.outf_name = malloc(strlen(optarg) + 1)))
				_failure("malloc");

			strcpy(config.outf_name, optarg);
		} else if (opt == 'e') {
			config.flags |= CONFIG_FL_REDIR_ERR;
			if (!(config.errf_name = malloc(strlen(optarg) + 1)))
				_failure("malloc");

			strcpy(config.errf_name, optarg);
		} else if (opt == 'p') {
			config.flags |= CONFIG_FL_PIDF_CREATE;
			if (!(config.pidf_name = malloc(strlen(optarg) + 1)))
				_failure("malloc");

			strcpy(config.pidf_name, optarg);
		} else if (opt == 'f') {
			config.flags |= CONFIG_FL_PIDF_FORCE;
		} else if (opt == 'r') {
			config.flags |= CONFIG_FL_PROC_RESTART;
		} else if (opt == 'R') {
			config.flags |= CONFIG_FL_PROC_RESTART;
			config.flags |= CONFIG_FL_PROC_RSTIGN;
		} else {
			_usage_invalid_opt(optopt, argv);
		}
	}

	if ((argc - optind) < 1)
		_usage(argv);

	_config_set_req(argv[optind]);
	_config_set_opt(argv, optind);
}

static int _signal_init(void) {
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));

	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &_sigah;

	if (sigaction(SIGHUP, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGINT, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGQUIT, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGABRT, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGTERM, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGUSR1, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGUSR2, &sa, NULL) < 0)
		return -1;

	if (sigaction(SIGCONT, &sa, NULL) < 0)
		return -1;

	return 0;
}

int main(int argc, char *argv[], char *envp[]) {
	int ret = 0;

	_config_default_init();

	_cmdline_process(argc, argv);

	if (setregid(config.gid, config.gid) < 0)
		_failure("setregid");

	if (setreuid(config.uid, config.uid) < 0)
		_failure("setreuid");

	if (_signal_init() < 0)
		_failure("_signal_init");

	if (_daemonize() < 0)
		_failure("_daemonize");

	for (;;) {
		if (config.flags & CONFIG_FL_PIDF_CREATE)
			_file_pid_create(config.pidf_name);

		ret = _bexec(config.binary, config.args, envp);

		if (!(config.flags & CONFIG_FL_PROC_RESTART) || !ret) {
			if (!(config.flags & CONFIG_FL_PROC_RSTIGN))
				break;
		}

		if (config.flags & CONFIG_FL_PIDF_CREATE) {
			if (unlink(config.pidf_name) < 0)
				_failure("unlink");
		}
	}

	if (ret < 0)
		_failure("_bexec");

	if (config.flags & CONFIG_FL_PIDF_CREATE) {
		if (unlink(config.pidf_name) < 0)
			_failure("unlink");
	}

	_config_destroy();

	return 0;
}

