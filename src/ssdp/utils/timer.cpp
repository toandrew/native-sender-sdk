#include "timer.h"
#include "logger.h"

Timer::Timer() {
	mTimerServer = new TimerServer();

	mTimerServer->StartServer();
}

Timer::~Timer() {
	if (mTimerServer != NULL) {
		mTimerServer->StopServer();

		delete mTimerServer;
		mTimerServer = NULL;
	}
}

void Timer::schedule(RTN *start_rtn, long delay, long period, void *data) {
	TimeEvent *event = new TimeEvent();
	event->m_schedule = time(0) + delay;
	event->m_interval = period;
	event->m_repeatable = true;
	event->m_count = 0;
	event->m_callback = start_rtn;
	event->m_data = data;

	printf("interval:%d\n", period);
	mTimerServer->Register(event);
}

void Timer::cancel() {
	if (mTimerServer != NULL) {
		mTimerServer->StopServer();
	}
}

TimerServer::TimerServer() {
	m_shutdown = false;
	pthread_mutex_init(&m_mutex, NULL);
}

TimerServer::~TimerServer() {
	m_shutdown = true;
	//pthread_mutex_destory(&m_mutex);
}

void TimerServer::Register(TimeEvent* eventInst) {
	pthread_mutex_lock(&m_mutex);
	m_events.push_back(eventInst);
	pthread_mutex_unlock(&m_mutex);
}

void TimerServer::Release(TimeEvent* eventInst) {
	pthread_mutex_lock(&m_mutex);
	m_events.remove(eventInst);
	pthread_mutex_unlock(&m_mutex);  
}

void TimerServer::StartServer() {
	pthread_attr_t  attr;
	int ret = pthread_attr_init(&attr);
	if(ret != 0) {
		// TODO, FAILED
		return;
	}

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (ret != 0) {
		// TODO, FAILED
		pthread_attr_destroy(&attr);
		return;
	}

	LOGI("CREATE:%x", this);
	ret = pthread_create(&m_thread, &attr, TimerServer::ExecuteFunc, (void *)this);  //启动执行线程

	if (ret != 0) {
		// TODO, FAILED
		pthread_attr_destroy(&attr);
		return;
	}

	pthread_attr_destroy(&attr);
}

void TimerServer::StopServer() {
	void* status;

	if (m_shutdown) {
		return;
	}

	// remove all events
	pthread_mutex_lock(&m_mutex);
	m_events.clear();
	pthread_mutex_unlock(&m_mutex);  

	m_shutdown = true;
	pthread_cond_signal(&m_cond);
	pthread_join(m_thread, &status);

}

void TimerServer::wait(TimerServer* self, int timeInMs) {
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeInMs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeInMs % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    int n = pthread_cond_timedwait(&self->m_cond, &self->m_mutex, &ts);
    if (n == 0) {
        // TODO: singaled
    } else if (n == ETIMEDOUT) {
        // TODO: Time out.
    }
}

void* TimerServer::ExecuteFunc(void* ptr) {
	TimerServer* self = (TimerServer*)ptr;  //使用C++ static成员函数作为线程函数的典型做法
	//printf("TimeServer:ExecuteFunc![%x]\n", self->m_shutdown);

	while (!self->m_shutdown) { //这里的while循环说明，我们将m_shutdown置true以后并不马上生效，允许把当前这轮执行完
		time_t now = time(0); //拿到当前时间
		std::list<TimeEvent*>::iterator iter = self->m_events.begin();
		//printf("run!\n");
		for(; iter != self->m_events.end(); iter++){  //迭代所有的事件
		//	LOGE("iter!!!");
			TimeEvent* cur = *iter;
		//	printf("now[%x]interval[%x]schedule[%x]\n", now, cur->m_interval, cur->m_schedule);
			if (now < cur->m_schedule) continue; //未来的事件，暂时跳过
		//	LOGE("iter 1!!!");
			if (cur->m_repeatable || cur->m_count <= 0) { //如果不是repeat的任务并且已经执行过了，也跳过
				if(cur->m_callback) cur->m_callback(cur->m_data);
		//		LOGE("HERER!");
				cur->m_count++;
				if(cur->m_repeatable) {
		//			LOGE("sss");
					cur->m_schedule += cur->m_interval;
				}
            }
        }

        //while(!self->m_shutdown){  //我们不能让ExecuteFunc耗尽所有的CPU资源，于是sleep一会儿
        	//LOGE("sleep");
			wait(self, 1000);
			//LOGE("after sleep\n");
        //}
    }
    

    return NULL;
}
