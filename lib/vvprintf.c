#include<stdio.h>
#include<stdarg.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<string.h>
#include <config.h>
#define VVPRINTF_PORT 12345

static int vvprintf_init = 0;
static pthread_mutex_t vvprintf_mutex = PTHREAD_MUTEX_INITIALIZER;
static char msgbuf[1024*10];
static int vvprintf_socket;
static struct sockaddr_in vvprintf_serveraddr;

int vvprintf(char *format,...)
{
	va_list myargs;
	va_start(myargs,format);

	pthread_mutex_lock(&vvprintf_mutex);

	if(vvprintf_init == 0 ) {
		vvprintf_socket = socket(AF_INET,SOCK_DGRAM,0);
		vvprintf_serveraddr.sin_family = AF_INET;
		vvprintf_serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /*127.0.0.1*/
		vvprintf_serveraddr.sin_port = htons(VVPRINTF_PORT);
		vvprintf_init = 1;
	}

	vsprintf(msgbuf,format,myargs);
	sendto(vvprintf_socket,msgbuf,strlen(msgbuf),0,(struct sockaddr *)&vvprintf_serveraddr,sizeof(struct sockaddr));
	
	va_end(myargs);
	
	pthread_mutex_unlock(&vvprintf_mutex);
	return 0;
}
