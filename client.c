#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define max_strlenght 256

pid_t pid;

int main(int argc, char** argv)
{
	int sockfd;
	int n, len;
	char sendline[max_strlenght], recvline[max_strlenght];
	struct sockaddr_in servaddr, cliaddr;
	
	if (argc != 2)
	{
    printf("Usage: ./a.out <IP address>\n");
    exit(1);
	}
	
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
    perror(NULL);
    exit(1);
	}
	
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
	{
    perror(NULL);
    close(sockfd);
    exit(1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51015);
	
	if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
    printf("Invalid IP address\n");
    close(sockfd);
    exit(1);
	}
	printf("Welcom to our server Pleas enter your name: ");
	fgets(sendline, max_strlenght, stdin);
	if (sendto(sockfd, sendline, strlen(sendline) , 0,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
	pid = fork();
	
	if(pid == 0) 
	{
		while (1)
		{
			gets(sendline);
			
			if (strcmp(sendline, "exit ") == 0)
			{
				printf("Прощай, друг!");
				close(sockfd);
				exit(1);
			}
			
			if (sendto(sockfd, sendline, strlen(sendline) + 1, 0,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
			{
				perror(NULL);
				close(sockfd);
				exit(1);
			}
		}
	}
	
	if(pid != 0) 
	{
		while (1)
		{
			if ((n = recvfrom(sockfd, recvline, max_strlenght, 0, (struct sockaddr*) NULL, NULL)) < 0)
			{
				
				perror(NULL);
				close(sockfd);
				exit(1);
  
			}
			printf("%s", recvline);
		}
	}
	close(sockfd);
	
	return 0;
}

