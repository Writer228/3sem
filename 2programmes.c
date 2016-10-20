#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{ 
   char*    str;
   int     shmid; 
   const char pathname[] = "2programmes.c"; 

   key_t   key;
   

   if((key = ftok(pathname , 0)) < 0){ 
     printf("Can\'t generate key\n");
     exit(-1);
   }

  
   if((shmid = shmget(key, 10000 * sizeof(char), 0)) < 0){
		printf("Can't find shared memory\n");
		exit(-1);
	}

   str = (char *)shmat(shmid, NULL, 0);
    

   printf("%s", str);


   if(shmdt(str) < 0){
      printf("Can't detach shared memory\n");
      exit(-1);
   }
   shmctl(shmid, IPC_RMID, NULL);
   return 0;
}

