#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"

SsdpService *gSsdpService;

int ssdp_discovery()
{
    struct sockaddr_in   addrin     ;
    struct timeval       rtime      ;
    int                  newsock    ;
    int                  ret        ;
    char                 buf[1024]  ;
    int                  i=0        ;
    int                  yes=1      ;

    rtime.tv_sec  = 2 ;
    rtime.tv_usec = 0 ;

    bzero(&addrin, sizeof(addrin));
    addrin.sin_family = AF_INET;
    addrin.sin_addr.s_addr = inet_addr("239.255.255.250"); //htonl(INADDR_ANY)
    addrin.sin_port = htons(1900);

    newsock=socket(AF_INET,SOCK_DGRAM,0);
    if( newsock < 0) {perror("1"); return -1;}

    setsockopt( newsock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&rtime, sizeof(struct timeval));
    setsockopt( newsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ;

    ret=bind( newsock, (struct sockaddr *)&addrin, sizeof(addrin));
    if( ret < 0 )   {perror("2"); return -1;}

    while(i<8)
    {
        i++;
        yes=sizeof(struct sockaddr_in);

        memset(buf, 0, sizeof(buf));
        ret=recvfrom( newsock, &buf,sizeof(buf), 0, (struct sockaddr *)&addrin, &yes);
        if( ret < 0 ) {perror("3"); continue;}
        printf("ip:%s\n",inet_ntoa( addrin.sin_addr));
        printf("%s\n",buf);
      

    }

    close(newsock);
    return 0;
}

void exit_handler(int sig) {
    LOGI("exit_handler[%d]", sig);

    if (gSsdpService != NULL) {
    	gSsdpService->stop();
    	delete gSsdpService;
    	gSsdpService = NULL;
    }

	exit(0);
}

void *enter_mainLoop(void *args) {
	int stoploop = 0;
	while (!stoploop) {
//ssdp_discovery();
		sleep(1);
	}
	LOGI("");
	return NULL;
}

int main(int argc, char **argv) {
	gSsdpService = new SsdpService();

	// ready to run in loop mode
	gSsdpService->start();

    pthread_t cmdloop_thread;
    int sig;
    sigset_t sigs_to_catch;
    struct sigaction action;
    int code;

    code = pthread_create(&cmdloop_thread, NULL, enter_mainLoop, NULL);

    if (code !=  0) {
        return -1;
    }

    sigemptyset(&sigs_to_catch);
    sigaddset(&sigs_to_catch, SIGINT);
    sigaddset(&sigs_to_catch, SIGKILL);
    if(sigismember(&sigs_to_catch, SIGINT) || sigismember(&sigs_to_catch, SIGKILL)) {
        sigemptyset(&action.sa_mask);
        action.sa_handler = exit_handler;
        action.sa_flags = 0;
        sigaction(SIGINT, &action, NULL);
    }
    sigwait(&sigs_to_catch, &sig);

    LOGI("Shutting down on signal %d...\n", sig);

	// quit from loop.
	gSsdpService->stop();

	delete gSsdpService;
	gSsdpService = NULL;

    return 0;
}
