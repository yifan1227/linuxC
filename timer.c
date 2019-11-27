#include <sys/select.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct arg{
		char a[20];
		char b;
};

void milliseconds_sleep(unsigned long mSec){
    struct timeval tv;
    tv.tv_sec=mSec/1000;
    tv.tv_usec=(mSec%1000)*1000;
    int err;
    do{
       err=select(0,NULL,NULL,NULL,&tv);
    }while(err<0 && errno==EINTR);
}

void wake(struct arg *argu)
{
	while(1)
        {
                milliseconds_sleep(1000);
                printf("%s, the secret passed by the main thread is: %d\n", argu->a, argu->b);
        }
}

int main()
{
	pthread_t id;
	struct arg arg1 = {"Hello from thread", 123};
	int res = pthread_create(&id, NULL, (void *)wake, &arg1);
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
