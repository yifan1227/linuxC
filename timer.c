#include <sys/select.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

struct arg{
		char a[20];
		bool b;			// 1 for periodical and 0 for non-periodical
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
	if(argu->b)
	{
	while(1)
        {
			pthread_testcancel();	
            milliseconds_sleep(1000);
            printf("%s, the secret passed by the main thread is: %d\n", argu->a, argu->b);
        }
	}
	else
	{
		{
                milliseconds_sleep(1000);
                printf("%s, the secret passed by the main thread is: %d\n", argu->a, argu->b);
        }
	}
}

int main()
{
	pthread_t id1, id2;
	struct arg arg1 = {"Hello from thread1", true};
	struct arg arg2 = {"Hello from thread2", false};
	int res = pthread_create(&id1, NULL, (void *)wake, &arg1);
	int res2 = pthread_create(&id2, NULL, (void *)wake, &arg2);
	if(res)
	{
		sleep(1);
		printf("Create thread error\n");
		return -1;
	}
	int i = 2;
	while(i >= 0)
	{
		printf("I'm in main\n");
		sleep(3);
		i--;
	}
	pthread_cancel(id1);
	while(1)
	{
		printf("no thread now\n");
		sleep(1);
	}
	return 1;
}
