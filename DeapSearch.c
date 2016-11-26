#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <dirent.h>

#define MAX_DEPTH 2

int Find(DIR *dirp, int depth, char *path);

struct stat buf;
struct dirent *mydirent;

/*
 * Магические числа 40, 150. Нужно объявить константы.
 */

char name[40] = {"DeapSearch.c"};
char path[150] = {"/home/user/Desktop"};
char ret[150];
int size = 0;

int main()
{
	DIR* dirp;
	
	dirp = opendir(path);
	
	if (Find(dirp, MAX_DEPTH, name) == 10)
	{
		printf("File was not found");
	}
	
	return 0;
}


int Find(DIR *dirp, int depth, char *name)
{
	while (1)
	{
		if ((mydirent = readdir(dirp)) == NULL)
		{
			strcpy(ret, "");
			strncpy(ret, path, (strlen(path) - size));
			closedir(dirp);
			strcpy(path,ret);

			break;
		}
    /*
     * Если файл существует, то вы его найдёте, но вот размер его выведите неверный в общем случае, т.к. 
     * Можете добавить проверку возвращаемого значения stat
     */
		stat(mydirent->d_name, &buf);
    
		if ((int)mydirent->d_type == 8)
		{
			if (strcmp(mydirent->d_name, name) == 0)
			{
				printf("file was found by this way: %s\n", path);
				printf("Name = %s; Size = %d;", mydirent->d_name, (int)buf.st_size);
				exit (1);
			}
		}
		if (((int)mydirent->d_type == 4) && (strcmp(mydirent->d_name,".") != 0) && (strcmp(mydirent->d_name,"..") != 0))
		{
			size = strlen(mydirent->d_name) + 1;
			strcat(path, "/");
			strcat(path, mydirent->d_name);
			if (depth == 0)
			{
				printf("The file is too deep to find it");
				exit(1);
			}
			Find(opendir(path) , depth - 1, name);
		}
		
	}
/*
 * что за магические цифры 4, 8, 10?
 * Если ещё нет готовых констант для них, то нужно объявить.
 */
	return 10;
}
