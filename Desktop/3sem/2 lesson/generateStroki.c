#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void GenerateString(int n, char* string);

int main()
{
	char* a;
	int n;
	scanf("%d", &n);
	a = malloc((1<<n)*sizeof(char));
	GenerateString(n, a);
	return 0;
}

void GenerateString(int n, char* string)
{
	char* b; 
	b= malloc((1<<n)*sizeof(char));
	char* c;
	c= malloc(1*sizeof(char));
	char* d;
	d= malloc((1<<n)*sizeof(char));
	int i = 1;
	b[0]='a';
	while (i < n)
	{
		strcpy(d,b);
		c[0] = 'a'+i;
		strcat(b,c);
		strcat(b,d);
		i++;
	}
	strcpy(string,b);
	printf("%s\n",string);
}

