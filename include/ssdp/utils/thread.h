#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

#include "runnable.h"

class Thread {
public:
	Thread(Runnable *runnable);
	virtual ~Thread();

	void start();

	void interrupt();
private:
	static void* ExecuteFunc(void* data);
private:
	pthread_t m_thread;
	Runnable *mRunnable;
};

#endif //THREAD_H_
