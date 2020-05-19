#include <event2/event-config.h>
 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
 
#include <event2/event.h>
#include <event2/util.h>

struct event_base* base;

#define MAX_TVS 10

struct m_tv{
    struct timeval tv;
    uint8_t used;
};

static struct m_tv m_tvs[MAX_TVS];

static struct m_tv *get_tv(void)
{
    for(int i = 0; i < MAX_TVS; i++){
        if(m_tvs[i].used)  
            continue;
        else{
            return &m_tvs[i];
        }
        printf("No available timeval\n");
        return NULL;
    }
}

static void
fifo_read(evutil_socket_t fd, short event, void *arg)
{
	char buf[255];
	int len;
	struct event *ev = arg;
 
	fprintf(stderr, "fifo_read called with fd: %d, event: %d, arg: %p\n",
	    (int)fd, event, arg);
	len = read(fd, buf, sizeof(buf) - 1);
 
	if (len <= 0) {
		if (len == -1)
			perror("read");
		else if (len == 0)
			fprintf(stderr, "Connection closed\n");
		event_del(ev);
		event_base_loopbreak(event_get_base(ev));
		return;
	}
 
	buf[len] = '\0';
	fprintf(stdout, "Read: %s\n", buf);
}

static void
timer_cb(evutil_socket_t fd, short event, void *arg)
{
	
	printf("Time out callback, %s\n", (char *)arg);
}
 
/* On Unix, cleanup event.fifo if SIGINT is received. */
static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event_base *base = arg;
	event_base_loopbreak(base);
}

int register_timer_event(uint8_t oneshot, event_callback_fn callback, int mSec, void *arg)
{
    struct event *evtimer;
    struct m_tv *m_tv0;
	struct timeval *tv;

    evtimer = event_new(base, -1, oneshot? 0:EV_PERSIST, callback, arg);
    m_tv0 = get_tv();
    if(m_tv0 == NULL)
        return -1;
	tv = &m_tv0->tv;
	evutil_timerclear(tv);
	tv->tv_sec = mSec / 1000;
	tv->tv_usec = (mSec - tv->tv_sec * 1000) * 1000;
    m_tv0->used = 1;
    return event_add(evtimer, &m_tv0->tv);
}

int
main(int argc, char **argv)
{
	struct event *evfifo;
	struct event *signal_int;
	struct event *evtimer;
	struct stat st;
	const char *fifo = "event.fifo";
	int socket;
	struct timeval tv;
 
	if (lstat(fifo, &st) == 0) {
		if ((st.st_mode & S_IFMT) == S_IFREG) {
			errno = EEXIST;
			perror("lstat");
			exit(1);
		}
	}
 
	unlink(fifo);
	if (mkfifo(fifo, 0600) == -1) {
		perror("mkfifo");
		exit(1);
	}
 
	socket = open(fifo, O_RDONLY | O_NONBLOCK, 0);
 
	if (socket == -1) {
		perror("open");
		exit(1);
	}
 
	fprintf(stderr, "Write data to %s\n", fifo);
	fprintf(stderr, "Write data to %s %d\n", fifo, socket);
	/* Initalize the event library */
	base = event_base_new();
 
	/* Initalize one event */
	/* catch SIGINT so that event.fifo can be cleaned up */
	signal_int = evsignal_new(base, SIGINT, signal_cb, base);
	event_add(signal_int, NULL);
 
	evfifo = event_new(base, socket, EV_READ|EV_PERSIST, fifo_read,
                           event_self_cbarg());
 
	/* Add it to the active events, without a timeout */
	event_add(evfifo, NULL);

	evutil_timerclear(&tv);
	tv.tv_sec = 3;
	evtimer = event_new(base, -1, EV_PERSIST, timer_cb, NULL);
	//event_add(evtimer, &tv);

	
	char *s1 = "Timer 1";
	char *s2 = "Timer 2";
	char *s3 = "Timer 3";
	//register_timer_event(0, timer_cb, 1500, s1);
	//register_timer_event(0, timer_cb, 2500, s2);
	//register_timer_event(1, timer_cb, 3000, s3);
 
	event_base_dispatch(base);
	event_base_free(base);
	close(socket);
	unlink(fifo);
	libevent_global_shutdown();
	return (0);
}
