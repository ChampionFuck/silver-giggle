#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t m;
pthread_cond_t v;

int balance=0;

void *routine(void *arg)
{
    pthread_mutex_lock(&m);

    while(balance<100)
        pthread_cond_wait(&v,&m);


    fprintf(stderr,"[%d]取走了100，剩余%d\n",(int)arg,balance);
    balance-=100;

    pthread_mutex_unlock(&m);
    pthread_exit(NULL);

}



int main()
{
    pthread_cond_init(&v,NULL);
    pthread_mutex_init(&m,NULL);

    pthread_t tid;
    int number = 10;
    for(int i=0;i<number;i++)
    {
        pthread_create(&tid,NULL,routine,(void *)i);
    }
    sleep(1);
    pthread_mutex_lock(&m);

    balance+=400;
    pthread_cond_broadcast(&v);

    pthread_mutex_unlock(&m);

    pause();
    return 0;
}
