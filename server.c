#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define max_user_num 100
#define max_strlenght 256

struct user
{
	int id;
	char name[max_strlenght];
	int port;
};


struct user users[max_user_num];
int sockfd;
int n;
socklen_t clilen;
int usnum = 0;
char line[max_strlenght];
char message[max_strlenght];
char priv_user_name[max_strlenght];
struct sockaddr_in servaddr, cliaddr;

void Mysend(int current_user, int priv_user, int f)
{
	strcat(message, users[current_user].name);
	if (f == 1)
		strcat(message, "told you: ");
	if (f == 0)
		strcat(message, "told to all: ");
	strcat(message, line);
	strcat(message, "\n");
	
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = users[priv_user].port;
	cliaddr.sin_addr.s_addr = users[priv_user].id;
	if (sendto(sockfd, message, strlen(message) + 1, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	
	bzero(message, strlen(message));
  
	
}

int main()
{  
	int i, j;
	int current_user;
	bzero(message, strlen(message));
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51015);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	} else
	{
		printf("Sock fd:%d\n", sockfd);
	}
  
	if (bind(sockfd, (struct sockaddr*) &servaddr, 
		sizeof(servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
  
	while (1)
	{
		clilen = sizeof(cliaddr);
		
		bzero(line, max_strlenght);
		
		if ((n = recvfrom(sockfd, line, max_strlenght, 0, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
		perror(NULL);
		close(sockfd);
		exit(1);
		}
		
		//registration
		current_user = -1;
		for(i = 0; i < usnum; i++)
		{
			if ((cliaddr.sin_addr.s_addr == users[i].id) && (cliaddr.sin_port == users[i].port))
			{
				current_user = i;
			}
		}
		if(current_user == -1)
		{
			users[i].id = cliaddr.sin_addr.s_addr;
			users[i].port = cliaddr.sin_port;
			strcat(users[i].name, line);
			usnum++;
			printf(" %s user conected with \n port %d \n id %d \n", line, users[i].port, users[i].id);
		}
		else 
		{
			if(line[0] == '!')
			{
				int name_lenght;
				name_lenght = strcspn(line, " ");
				strncpy(priv_user_name, line +1, name_lenght -1);
				strcpy(line, line + name_lenght  +1);
	
				j = 0;
				while (j < usnum)
				{
					if(strncmp(priv_user_name, users[j].name, name_lenght - 1) == 0)
					{
						Mysend(current_user, j, 1);
					}
					j++;
				}
			}
			else
			{
				for(i = 0; i < usnum; i++)
				{
					if(i != current_user)
					{
						Mysend(current_user, i, 0);
					}
				}
				
			}
		}
		
		
	}
	
	return 0;
}
