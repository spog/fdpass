/*
 *  Copyright (C) 2013  Samo Pogacnik
 */

/*
 * The U2UP FD passing module
 */

#ifndef public_fdpass_h
#define public_fdpass_h

#ifdef fdpass_c
/* PRIVATE usage of the PUBLIC part. */
#	undef EXTERN
#	define EXTERN
#else
/* PUBLIC usage of the PUBLIC part. */
#	undef EXTERN
#	define EXTERN extern
#endif

EXTERN int fdpass_init_server(const char *path);
EXTERN int fdpass_init_client(const char *path);
EXTERN int fdpass_accept(int listen_sd);
EXTERN int fdpass_send(int fdpass_sd, int fd);
EXTERN int fdpass_recv(int sd);
EXTERN void fdpass_close(int sd);

#endif /*public_fdpass_h*/

