#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

#include "runnable.h"

class Thread {
public:
	Thread(Runnable *runnable, void*data);
	virtual ~Thread();

	void start();

	void interrupt();
private:
	static void* ExecuteFunc(void* data);
private:
	pthread_t m_thread;
	Runnable *mRunnable;
	void *mData;
};

#endif //THREAD_H_
