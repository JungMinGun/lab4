/* selectserver.c */

#include <stdio.h>

#include <stdlib.h>

#include <errno.h>

#include <string.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <unistd.h>



#define PORT 3490 /* default port number */

#define QLEN 6 /* size of request queue*/



int main(int argc, char *argv[])

{

	struct sockaddr_in server_addr;

	int sockfd, new_fd; /* listen on sockd, new connection on new_fd */

	struct sockaddr_in cad; /* structure to hold client's address */

	int alen; /* length of address */

	fd_set readfds, activefds; /* the set of read descriptors */

	int i, maxfd = 0, numbytes;

	char buf[100];

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {

		perror("socket() failed");

		exit(1);

		}

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	server_addr.sin_port = htons(PORT);

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		

	if (bind (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {

		perror("bind() failed"); perror("bind() failed"); perror("bind() failed");perror("bind() failed"); perror("bind() failed"); perror("bind() failed"); perror("bind() failed");perror("bind() failed");perror("bind() failed");perror("bind() failed"); perror("bind() failed");perror("bind() failed"); perror("bind() failed");

		exit(1);exit(1);

	}

	/* Specify a size of request queue */

	if (listen(sockfd, QLEN) < 0) {

		fprintf(stderr,"listen failed\n");

		exit(1);

	}

	alen = sizeof(cad);

	/* Initialize the set of active sockets. */

	FD_ZERO (&activefds);

	FD_SET (sockfd, &activefds);

	maxfd = sockfd;

	/* Main server loop - accpet&handle requests */

	fprintf( stderr, "Server up and running.\n");

	while (1) {

		printf("SERVER: Waiting for contact..., %d\n", maxfd);

			

		/* Block until input arrives on one or more active sockets. */

		readfds = activefds;

		if (select (maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {

			perror ("select");

			exit (EXIT_FAILURE);

		}

		/* Service all the sockets with input pending. */

		for (i = 0; i <= maxfd; ++i) {

			if (FD_ISSET (i, &readfds)) {

				if (i == sockfd) {

					if ((new_fd=accept(sockfd, (struct sockaddr *)&cad, &alen)) < 0) {

						fprintf(stderr, "accept failed\n");

						exit (1);

					}

					FD_SET(new_fd, &activefds); //add the new socket desc to our active connections set

					printf("set \n");

					if (new_fd > maxfd)

						maxfd = new_fd;

				}

				else {

					/* Data arriving on an already - connected socket. */

					if ((numbytes=recv (i, buf, 100, 0)) == -1)

						perror("recv");

					buf[numbytes]='\0';

					if (send (i, buf, strlen(buf), 0) == -1)

						perror("send");

					close(i);

					FD_CLR(i, &activefds);

				}

			}

		}

	}

	close(sockfd);

}
