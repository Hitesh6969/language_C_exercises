#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#define SERVER_PORT 40000
#define SOCKET_ERROR ((int)-1)
#define DIMBUFF 512

int main(int argc, char *argv[])
{

	struct sockaddr_in servizio, rem_indirizzo;
	int pid, nread, soa, socketfd, client_len, fd, on = 1, fromlen = sizeof(servizio);
	char risposta, nome_file[20];

	memset((char *)&servizio, 0, sizeof(servizio));

	servizio.sin_family = AF_INET;
	servizio.sin_addr.s_addr = htonl(INADDR_ANY);
	servizio.sin_port = htons(SERVER_PORT);

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	// Bind
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	bind(socketfd, (struct sockaddr *)&servizio, sizeof(servizio));

	listen(socketfd, 10);

	// attesa del client
	for (;;)
	{
		printf("\n\nServer in ascolto...\n");
		fflush(stdout);

		// accept
		soa = accept(socketfd, (struct sockaddr *)&rem_indirizzo, &fromlen);

		pid = fork();

		if (pid == 0)
		{
			read(soa, nome_file, sizeof(nome_file));
			printf("nome file: %s\n", nome_file);
			fflush(stdout);
			execl("/usr/bin/cat", "cat", nome_file, (char *)0);
			return -1;
		}
		// risoluzione del client
		// printf("\n\n Stabilita la connessione con il client");

		// close(socketfd);
	}

	return 0;
}