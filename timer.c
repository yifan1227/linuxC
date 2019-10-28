#include <sys/select.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void milliseconds_sleep(unsigned long mSec){
    struct timeval tv;
    tv.tv_sec=mSec/1000;
    tv.tv_usec=(mSec%1000)*1000;
    int err;
    do{
       err=select(0,NULL,NULL,NULL,&tv);
    }while(err<0 && errno==EINTR);
}

void wake()
{
	while(1)
        {
                milliseconds_sleep(1000);
                printf("Wake up!\n");
        }
}

int main()
{
	pthread_t id;
	int res = pthread_create(&id, NULL, (void *)wake, NULL);
	if(res)
	{
		printf("Create thread error\n");
		return -1;
	}
	while(1)
	{
		printf("I'm in main\n");
		sleep(3);
	}
}
