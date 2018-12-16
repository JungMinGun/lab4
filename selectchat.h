#ifdef __SMARTSOCK_H__
#include <sys/select.h>
#define __SMARTSOCK_H__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#define PORT 7777
#define IP "127.0.0.1"
#define MAX_USER 5
#define NIC_NAME_SiZE 20
#define NIC_NAME_MSG (9+2)
#define BUF_SIZE 255
#define MSG_SIZE (BUF_SIZE+1+NIC_NAME_MSG)
#define MSG_EnD "\x01\x02\x03"
#endif
