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

#define TABLE_LIMIT 5 

FILE *posuda_in, *moishik, *vitiralchik;

int main()
{
	posuda_in = fopen("posuda_in.txt", "r");
    moishik = fopen("moishik.txt", "r");
    vitiralchik = fopen("vitiralchik.txt", "r");
	
	int len, maxlen, i = 0, k = 0, semid;
	int msqid;
	char pathname[] = "moika.c";
	key_t key;
	struct sembuf mysem;

	struct posuda
	{
		char dishes[200];
		int number;
	} mydishes;
  
	struct time
	{
		char dishes[200];
		int time;
	} mymoishik[10], myvitiralchik[10];
  
	struct mymsgbuf
	{
		long mtype;
		struct {
			char dishes[200];
			int time;
		} info;
	} mybuf;
	
  
	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}
	
	semid = semget(key , 2 , 0666 | IPC_CREAT);
  
	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}
	
	while (fscanf(moishik, "%s %d", mymoishik[i].dishes, &mymoishik[i].time) != EOF)
	{
		i++;
	}
	i = 0;
	
	while (fscanf(vitiralchik, "%s %d", myvitiralchik[i].dishes, &myvitiralchik[i].time) != EOF)
	{	
		i++;
	}
	
	mysem.sem_op = TABLE_LIMIT;
	mysem.sem_flg = 0;
	mysem.sem_num = 0;
	
	semop(semid , &mysem , 1);
	 
	pid_t pid = fork();
	
	if (pid > 0)// moishik
	{
		while (fscanf(posuda_in, "%s %d", mydishes.dishes, &mydishes.number) != EOF)
		{
			i = 0;
			while (i != mydishes.number)
			{
				k = 0;
				if (strcmp(mydishes.dishes, mymoishik[k].dishes ) != 0)
				{
					k++;
				}
				

				
				mybuf.mtype = 1;
				strcpy(mybuf.info.dishes, mymoishik[k].dishes);
				mybuf.info.time = mymoishik[k].time;
				len = sizeof(mybuf.info);
				
				sleep(mymoishik[k].time);
				mysem.sem_op = -1;
				mysem.sem_flg = 0;
				mysem.sem_num = 0;
				semop(semid , &mysem , 1);
				if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
				{
				  printf("Can\'t send message to queue\n");
				  msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
				  exit(-1);
				}
				
				
				printf("Washer wash up the %s for %d sec\n", mymoishik[k].dishes, mymoishik[k].time);
				printf("There is %d free places on the table\n\n",semctl(semid, 0, GETVAL, 0));
				i++;
			}
		}
		strcpy(mybuf.info.dishes, "end");
		if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
			{
			  printf("Can\'t send message to queue\n");
			  msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
			  exit(-1);
			}
		printf("all dishes were washed\n\n");
	}
	
	else // vitiralshik
	{
		while (1)
		{
			maxlen = sizeof(mybuf.info);

			if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0)) < 0)
			{
			  printf("Can\'t receive message from queue\n");
			  exit(-1);
			}
			k = 0;
			if (strcmp(mybuf.info.dishes, "end") == 0)
			{
				break;
			}
			if (strcmp(mybuf.info.dishes, myvitiralchik[k].dishes) != 0)
				{
					k++;
				}
			sleep(myvitiralchik[k].time);
			mysem.sem_op = 1;
			mysem.sem_flg = 0;
			mysem.sem_num = 0;
			semop(semid , &mysem , 1);
			printf("Wiper wipe up the %s for %d sec\n", myvitiralchik[k].dishes, myvitiralchik[k].time);
			printf("There is %d free places on the table\n\n",semctl(semid, 0, GETVAL, 0));
			
		}
		printf("all dishes were wiped\n");
		fclose(moishik);
		fclose(posuda_in);
		fclose(vitiralchik);
		msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
		semctl(semid, 0, IPC_RMID, 0);
	}
	return 0;
}

