#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE_BUFF 256

void stok (char **cmd, char * p_str, char * sep) 
{
	cmd[0] = p_str;
	
	int i = 1;
	while (p_str != NULL) 
    {
        p_str = strtok (NULL, sep);
		cmd[i] = p_str;
		i++;
	}
}

int main (int argc, char **argv) 
{
	struct sockaddr_in server;
	int sock_fd;

	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		perror("server: socket error");
		exit(EXIT_FAILURE);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(7000);
	server.sin_addr.s_addr = inet_addr("10.0.2.15");

	int server_len = 0;
	server_len = sizeof(server);

	if (bind(sock_fd, (struct sockaddr *)&server, server_len) == -1) 
	{
		perror("server: bind error");
		exit(EXIT_FAILURE);
	}

	char cmd_way[SIZE_BUFF];
	printf("Waiting connection...\n");

	if(recvfrom(sock_fd, &cmd_way, SIZE_BUFF, 0, (struct sockaddr *)&server, &server_len) == -1) 
	{
		perror("server: receive cmd_way error");
		exit(EXIT_FAILURE);
	}

	char cmd[SIZE_BUFF];

	if(recvfrom(sock_fd, &cmd, SIZE_BUFF, 0, (struct sockaddr *)&server, &server_len) == -1) 
	{
		perror("server: receive cmd error");
		exit(EXIT_FAILURE);
	}

	if (close(sock_fd) == -1)
	{
		perror("client: close socket error"); 
	}

	char * sep = " ";
	char * p_str = strtok(cmd, sep);

	char * command[SIZE_BUFF];
	stok(command, p_str, sep);

	pid_t pid;
	int status;

	pid = fork();

	if (pid == -1)
	{
		perror("Fork error");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		execv(cmd_way, command);
		exit(EXIT_SUCCESS);
	}

	else 
	{
		wait(&status);
	}

	exit(EXIT_SUCCESS);
}
