#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_LENGHT 150

char buf[] = "God watches on you.";
char result[MAX_LENGHT]; 
pid_t ppid;
pid_t pid;
int lenght;
int bitNumber = 0;

void reseive1(int nsig)
{
	int byteNumber = bitNumber / 8;
	int ptr = bitNumber % 8;
	
	result[byteNumber] = result[byteNumber] | (1 << ptr);
	bitNumber++;
		
	if (byteNumber == lenght)
	{
	
		printf("We get: %s\n", result);
		exit(0);
	}
	
	kill(pid, SIGINT);
}

void reseive0(int nsig)
{
	int byteNumber = bitNumber / 8;
	
	bitNumber++;
		
	if (byteNumber == lenght)
	{
		printf("We get: %s\n", result);
		exit(0);
	}
		
	kill(pid, SIGINT);
}

void send(int nsig)
{
	int byteNumber = bitNumber / 8;
	int ptr = bitNumber % 8;
	
	bitNumber++;
	if ((buf[byteNumber] & (1 << ptr)) == (1 << ptr))
		kill(ppid, SIGUSR2);
	else 
		kill(ppid, SIGUSR1);

	if (byteNumber == lenght)
		exit(1);
}

int main()
{	

	lenght = strlen(buf);
		
	ppid = getpid();
	pid = fork();
	
	signal(SIGUSR2, reseive1);
	signal(SIGUSR1, reseive0);
	signal(SIGINT, send);
	
	if (pid == 0)
	{
		send(1);
	}
	
	while(1);
	return 0;
}


