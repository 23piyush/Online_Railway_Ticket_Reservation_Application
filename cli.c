#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<time.h>
#include"client.h"

int main(int argc,char *argv[])
{
	
	struct sockaddr_in server;
	int sd;
	char buffer[80];
	sd = socket(AF_INET, SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8086);

	connect(sd, (struct sockaddr *)(&server), sizeof(server));
	
	mainMenu(sd);
	close(sd);
	return 0;
}
