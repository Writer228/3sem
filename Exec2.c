#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 

int main() 
{ 
	FILE *f = fopen("exec.txt", "r"); 
	int n = 0; 
	char s1[15];
	char s2[15];
	char s3[15]; 

	fscanf(f, "%d\n", &n); 
 
	fscanf(f, "%s ", s1); 
	fscanf(f, "%s ", s2);  
	pid_t a = fork();
	if (a == 0)
	{
		sleep(1);
		execlp(s1, s1, s2, NULL); 
		exit(1);
	}


	fscanf(f, "%s ", s1); 
	pid_t b = fork();
	if (b == 0)
	{
		sleep(2);
		execlp(s1, s1, NULL); 
		exit(1);
	}


	fscanf(f, "%s ", s1); 
	fscanf(f, "%s ", s2); 
	fscanf(f, "%s ", s3); 
	strcat(s2, s3); 
	pid_t c = fork();
	if (c == 0)
	{
		sleep(3);
		execlp(s1, s1, s2, NULL); 
		exit(1);
	}

	fclose(f); 
	return 0; 
}
