/*
 *  Copyright (C) 2013  Samo Pogacnik
 */

/*
 * The FD passing module
 */
#ifndef fdpass_c
#define fdpass_c
#else
#error Preprocesor macro fdpass_c conflict!
#endif

#include "fdpass.h"

int fdpass_init_server(const char *path)
{
	int status, listen_sd;
	struct stat st;
	struct sockaddr_un addr;

	if (path == NULL)
		evm_log_return_err("FD passing server socket name NOT PROVIDED (NULL)!\n");

	if (*path == '\0')
		evm_log_return_err("FD passing server socket name is empty string!\n");

	/* Initialize FD passing socket address... */
	status = stat(path, &st);
	if (status == 0) {
		/* Fle exists - check if socket */
		if ((st.st_mode & S_IFMT) == S_IFSOCK) {
			status = unlink(path);
			if (status != 0) {
				evm_log_return_err("Error unlinking the socket node");
			}
		} else {
			/* Not a socket, so do not unlink */
			evm_log_return_err("The path already exists and is not a socket node.\n");
		}
	} else {
		if (errno != ENOENT) {
			evm_log_return_err("Error stating the socket node path");
		}
	}

	/* There is nothing in our way to bind a Unix socket. */
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, path);

	/* Socket operations */
	listen_sd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (listen_sd == -1)
		evm_log_return_err("socket()\n");

	status = bind(listen_sd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
	if (status == -1)
		evm_log_return_err("bind() sockfd=%d\n", listen_sd);

	status = listen(listen_sd, 1024);
	if (status == -1)
		evm_log_return_err("listen() sockfd=%d\n", listen_sd);

	evm_log_debug("FD passing listening socket ready (bind FD: %d).\n", listen_sd);

	return listen_sd;
}

int fdpass_init_client(const char *path)
{
	int status, fdpass_sd;
	struct stat st;
	struct sockaddr_un addr;

	if (path == NULL)
		evm_log_return_err("FD passing client socket name NOT PROVIDED (NULL)!\n");

	if (*path == '\0')
		evm_log_return_err("FD passing client socket name is empty string!\n");

	/* Initialize FD passing socket address... */
	status = stat(path, &st);
	if (status == 0) {
		/* Fle exists - check if socket */
		if ((st.st_mode & S_IFMT) != S_IFSOCK) {
			/* Not a socket, so do not unlink */
			evm_log_return_err("The path already exists and is not a socket node.\n");
		}
	} else {
		if (errno != ENOENT) {
			evm_log_return_err("Error stating the socket node path");
		}
	}

	/* There is nothing in our way to connect a Unix socket. */
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, path);

	/* Socket operations */
	fdpass_sd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fdpass_sd == -1)
		evm_log_return_err("socket()\n");

	status = connect(fdpass_sd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
	if (status == -1)
		evm_log_return_err("connect() sockfd=%d\n", fdpass_sd);

	evm_log_debug("FD passing client socket ready (connect FD: %d).\n", fdpass_sd);

	return fdpass_sd;
}

int fdpass_accept(int listen_sd)
{
	int fdpass_sd;

	do {
		fdpass_sd = accept(listen_sd, (struct sockaddr *)NULL, NULL);
	} while ((fdpass_sd < 0) && (errno == EINTR));

	if (fdpass_sd < 0)
		evm_log_return_err("accept()\n");

	evm_log_debug("FD passing server socket ready (connect FD: %d).\n", fdpass_sd);
	return fdpass_sd;
}

int fdpass_send(int fdpass_sd, int fd)
{
	char data[1] = {'\0'}; /*this payload size defines sendmsg/recvmsg return value*/
	struct iovec iov[1];
	struct msghdr msg;
	struct cmsghdr *cmsg;
	char cbuf[CMSG_SPACE(sizeof(int)/* * NUM_FD*/)];

	memset(&msg, 0, sizeof(struct msghdr));
	memset(iov, 0, sizeof(iov));
	memset(cbuf, 0, sizeof(cbuf));

	data[0] = ' ';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cbuf;
	msg.msg_controllen = sizeof(cbuf);

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int)/* * NUM_FD*/);

	/* Setup the FD payload: */
	*((int *)CMSG_DATA(cmsg)) = fd;
	evm_log_debug("Sending descriptor = %d\n", fd);

	return sendmsg(fdpass_sd, &msg, 0);
}

int fdpass_recv(int sd)
{
	int ret;
	char data[1]; /*we expect a minimal single byte payload size (see fdpass_send)*/
	struct iovec iov[1];
	struct msghdr msg;
	struct cmsghdr *cmsg;
	char cbuf[CMSG_SPACE(sizeof(int)/* * NUM_FD*/)];
	int *fdptr; /* Contains the file descriptors passed. */

	/* Initialize structures */
	memset(&msg, 0, sizeof(msg));
	memset(iov, 0, sizeof(iov));
	memset(cbuf, 0, sizeof(cbuf));

	/* If data payload size is zero, the recvmsg() call will NOT block. */
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_iov = iov;
	msg.msg_iovlen  = 1;
	msg.msg_control = cbuf;
	msg.msg_controllen = sizeof(cbuf);

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int)/* * NUM_FD*/);

	/* Initialize the FD payload as an ERROR: */
	*(int*)CMSG_DATA(cmsg) = -1;

	evm_log_debug("Waiting on recvmsg\n");
	if ((ret = recvmsg(sd, &msg, 0)) < 0) {
		evm_log_error("recvmsg() failed - errno %d", errno);
		fdpass_close(sd);
		return -1;
	} else {
		cmsg = CMSG_FIRSTHDR(&msg);
		if (
			(cmsg == NULL) ||
			(cmsg->cmsg_level != SOL_SOCKET) ||
			(cmsg->cmsg_type != SCM_RIGHTS) ||
			(cmsg->cmsg_len < CMSG_LEN(sizeof(int)))
		) {
			if (cmsg) {
			  evm_log_error("Protocol failure: %d %d %d\n", cmsg->cmsg_len, cmsg->cmsg_level, cmsg->cmsg_type);
			} else {
			  evm_log_error("Protocol failure: NULL cmsghdr*\n");
			}
			fdpass_close(sd);
			return -1;
		} else {
			fdptr = (int*)CMSG_DATA(cmsg);
		}
		evm_log_debug("Received descriptor = %d\n", fdptr[0]);
	}

	fdpass_close(sd);
	return fdptr[0];
}

void fdpass_close(int sd)
{
	/*
	 * do not wait for the peer to close the connection.
	 */
	shutdown(sd, SHUT_RDWR);

	close(sd);
}

