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
#include <sys/sem.h>

int main()
{
	int fd;
    char buff[] = "LOL";
    const char filename[] = "file.txt";
    fd = open(filename, O_WRONLY);
    key_t key;
    int semid;
    const char pathname[] = "write_in_file.c";
    struct sembuf mybuf, mybuf1;
    
    key = ftok(pathname , 0);
    semid = semget(key , 1 , 0666 | IPC_CREAT);
    
    
	mybuf.sem_op = 1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	
	mybuf1.sem_op = 0;
	mybuf1.sem_flg = 0;
	mybuf1.sem_num = 0;
	
	
	while(1){
		if (semop(semid , &mybuf , 1) < 0) {
			printf("Can`t wait for condition\n");
			exit(-1);
		}
		semop(semid , &mybuf1 , 1);
		write(fd, buff, 3*(sizeof(char)));
		printf("Condition is set\n");
	}
    return 0;
}
