#include "thread.h"
#include "logger.h"

Thread::Thread(Runnable *runnable) {
	mRunnable = runnable;
}

Thread::~Thread() {

}

void Thread::start() {
	pthread_attr_t  attr;
	int ret = pthread_attr_init(&attr);
	if(ret != 0) {
		// TODO, FAILED
		return;
	}

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0) {
		// TODO, FAILED
		pthread_attr_destroy(&attr);
		return;
	}

	LOGI("CREATE:%x", this);
	ret = pthread_create(&m_thread, &attr, ExecuteFunc, (void *)this);  //启动执行线程

	if (ret != 0) {
		// TODO, FAILED
		pthread_attr_destroy(&attr);
		return;
	}

	pthread_attr_destroy(&attr);
}

void Thread::interrupt() {
}

// private
void* Thread::ExecuteFunc(void* data) {
	Thread *thread = (Thread *)data;
	if (thread && thread->mRunnable) {
		thread->mRunnable->run();
	}
}

