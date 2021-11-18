#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void client();
void server();

static const int port = 52000;

int main()
{
	if (fork())
	{
		client();
		printf("Client: all right!\n");
	}
	else
	{
		server();
		printf("Server: all right!\n");
	}

	return 0;
}

#define broken \
{\
	perror(0);\
	close(sockfd);\
	exit(1);\
}

void client()
{
	// Create socket
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) { perror(0); exit(1); }

	// Setup address structure
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if ( inet_aton("127.0.0.1", &servaddr.sin_addr) == 0) broken;

	if ( connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
		broken;

	const char text[] = "ping";
	write(sockfd, text, strlen(text) + 1);
	close(sockfd);


	close(sockfd);
}

void server()
{
	// Create socket
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) { perror(0); exit(1); }

	// Setup address structure
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) broken;
	if ( listen(sockfd, 10) < 0 ) broken;

	{
		struct sockaddr_in cliaddr;
		socklen_t clilen = sizeof(cliaddr);
		int newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
		if ( newsockfd < 0 ) broken;

		int n;
		const size_t bufsize = 100;
		char buf[bufsize];
		while ( (n = read(newsockfd, buf, bufsize-1)) > 0)
		{
			printf("%s", buf);
			bzero(buf, bufsize);
		}

		close(newsockfd);
	}

	close(sockfd);
}
