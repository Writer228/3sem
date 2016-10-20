#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main()
{
    char     buffer[50000];
    char 	 *str;
    int     shmid;
    int i = 0;
    const char pathname[] = "2programmes.c";
    key_t   key;
  

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((shmid = shmget(key, 10000 * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if(errno != EEXIST){
            printf("Can't create shared memory\n");
            exit(-1);
		}
    } 
        
	str = (char *)shmat(shmid, NULL, 0);
	
    int fd;
    fd = open(pathname, O_RDONLY);
	int bytesRead; 
	
	while ((bytesRead = read(fd, buffer, 80)) > 0)
	{
		
			strncat(str, (buffer), bytesRead);
			i++;
	}
	close(fd);
	
    if(shmdt(str) < 0){
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    
	
    return 0;
}
