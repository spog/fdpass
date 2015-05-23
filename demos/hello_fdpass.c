/*
 * The hello_fdpass demo program
 *
 * Copyright (C) 2015 Samo Pogacnik <samo_pogacnik@t-2.net>
 * All rights reserved.
 *
 * This file is part of the FDPASS software project.
 * This file is provided under the terms of the BSD 3-Clause license,
 * available in the LICENSE file of the "daemonize" software project.
*/

/*
 * This demo shows fdpass library usage.
*/

#ifndef hello_fdpass_c
#define hello_fdpass_c
#else
#error Preprocesor macro hello_fdpass_c conflict!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "libfdpass/fdpass.h"
#include "hello_fdpass.h"

unsigned int log_mask;
unsigned int evmlog_normal = 1;
unsigned int evmlog_verbose = 1;
unsigned int evmlog_trace = 1;
unsigned int evmlog_debug = 1;
unsigned int evmlog_use_syslog = 0;
unsigned int evmlog_add_header = 1;

#define FDPASS_SOCK_PATH "/tmp/hello_fdpass"

int fdpass_sender(void)
{
	int pass_sd, fd;

	fd = open("fdpass.tmp", O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0)
		return fd;
	evm_log_info("FD to be sent: %d!\n", fd);

	pass_sd = fdpass_init_client(FDPASS_SOCK_PATH);
	if (pass_sd < 0)
		return pass_sd;

	return fdpass_send(pass_sd, fd);
}

int fdpass_receiver(int listen_sd)
{
	int pass_sd, fd_passed;

	pass_sd = fdpass_accept(listen_sd);
	if (pass_sd < 0)
		return pass_sd;

	fd_passed = fdpass_recv(pass_sd);
	if (fd_passed < 0)
		return fd_passed;

	evm_log_info("Received FD: %d!\n", fd_passed);

	return 0;
}

static int hello_fork(void)
{
	int ret;
	pid_t child_pid;

	child_pid = fork();
	switch (child_pid) {
	case -1: /*error*/
		evm_log_return_system_err("fork()\n");
	case 0: /*child*/
		if ((ret = fdpass_sender()) < 0) {
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

	/*parent*/
	return child_pid;
}

int main(int argc, char *argv[])
{
	int listen_sd, ret;

	evm_log_info("This is ""hello_fdpass""!\n");

	listen_sd = fdpass_init_server(FDPASS_SOCK_PATH);
	if (listen_sd < 0)
		return listen_sd;

	if ((ret = hello_fork()) < 0) {
		exit(EXIT_FAILURE);
	}

	if ((ret = fdpass_receiver(listen_sd)) < 0) {
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

