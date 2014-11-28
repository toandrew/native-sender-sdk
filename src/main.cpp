#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"

SsdpService *gSsdpService;

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
