#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE_BUFF 256

int main (int argc, char **argv) {

        char cmd[SIZE_BUFF];
        int i;

        for (i = 1; i < argc; i++) {

                strcat(cmd, argv[i]);
                strcat(cmd, " ");
        }
        strcat(cmd, "\0");

        char cmd_way[SIZE_BUFF] = "/bin/";
        strcat(cmd_way, argv[1]);

        struct sockaddr_in client;
        int sock_fd;

        if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {

                perror("client: socket error");
                exit(EXIT_FAILURE);
        }

        client.sin_family = AF_INET;
        client.sin_port = htons(7000);
        client.sin_addr.s_addr = inet_addr("10.0.2.15");

        int client_len = 0;
        client_len = sizeof(client);

        if (sendto(sock_fd, &cmd_way, SIZE_BUFF, 0, (struct sockaddr *)&client, client_len) == -1)
        {
                perror("client: send cmd_way error");
                exit(EXIT_FAILURE);
        } 

        if (sendto(sock_fd, &cmd, SIZE_BUFF, 0, (struct sockaddr *)&client, client_len) == -1)
        {
                perror("client: send cmd_way error");
                exit(EXIT_FAILURE);
        }

        if (close(sock_fd) == -1)
        {
               perror("client: close socket error"); 
        }

        exit(EXIT_SUCCESS);

}
