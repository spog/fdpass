/*
 * The hello_fdpass demo program
 *
 * Copyright (C) 2014 Samo Pogacnik <samo_pogacnik@t-2.net>
 * All rights reserved.
 *
 * This file is part of the FDPASS software project.
 * This file is provided under the terms of the BSD 3-Clause license,
 * available in the LICENSE file of the EVM software project.
*/

#ifndef hello_fdpass_h
#define hello_fdpass_h

#ifdef hello_fdpass_c
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

#ifdef hello_fdpass_c
/*
 * Here is the PRIVATE stuff (within above ifdef).
 * It is here so we make sure, that the following PRIVATE stuff get included into own source ONLY!
 */
#include "../src/log_module.h"
EVMLOG_MODULE_INIT(DEMOFDP, 3);

#endif /*hello_fdpass_c*/
/*
 * Here continues the PUBLIC stuff, if necessary.
 */


#endif /*hello_fdpass_h*/
