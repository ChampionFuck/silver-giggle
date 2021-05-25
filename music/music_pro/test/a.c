#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/shm.h>
#include <fcntl.h>

sem_t *space;
sem_t *data;

int main()
{
	int shmid = shmget(ftok("/home/gec",1),2,IPC_CREAT|0666);
	char *p = shmat(shmid,NULL,0);

	space = sem_open("/space",O_CREAT,0666,1);
	data  = sem_open("/data",O_CREAT,0666,0);

	while(1)
	{
		sem_wait(data);

		printf("收到的数据为：%s\n",p);
		sleep(1);
		sem_post(space);

	}
	return 0;

}
