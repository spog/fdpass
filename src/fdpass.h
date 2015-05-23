/*
 *  Copyright (C) 2013  Samo Pogacnik
 */

/*
 * The FD passing module
 */

#ifndef fdpass_h
#define fdpass_h

#ifdef fdpass_c
/* PRIVATE usage of the PUBLIC part. */
#	undef EXTERN
#	define EXTERN
#else
/* PUBLIC usage of the PUBLIC part. */
#	undef EXTERN
#	define EXTERN extern
#endif
/*
 * Here starts the PUBLIC stuff:
 *	Enter PUBLIC declarations using EXTERN!
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libfdpass/fdpass.h"

#ifdef fdpass_c
/*
 * Here is the PRIVATE stuff (within above ifdef).
 * It is here so we make sure, that the following PRIVATE stuff get included into own source ONLY!
 */
#include "log_module.h"
EVMLOG_MODULE_INIT(FD_PASS, 2)

#endif /*fdpass_c*/
/*
 * Here continues the PUBLIC stuff, if necessary.
 */

#endif /*fdpass_h*/

