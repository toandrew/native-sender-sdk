#ifndef TIMER_H_
#define TIMER_H_

#include <pthread.h>
#include <error.h>
#include <sys/time.h>

#include <list>
using namespace std;

#define ETIMEDOUT 110

typedef void* (RTN)(void*);

class TimerServer;

class Timer {
private:
	TimerServer *mTimerServer;
public:
	Timer();
	virtual ~Timer();

	void schedule(RTN *start_rtn, long delay, long period, void *data);

	void cancel();
};

class TimeEvent{
public:
    time_t  m_schedule;   					//预计下一次的执行时间
    time_t  m_interval;     				//如果是repeatable的事件，事件的执行间隔
    bool    m_repeatable; 					//是不是需要重复执行
    int     m_count;        				//该事件被触发的次数
    RTN    *m_callback; 					//绑定的事件回调类实例 
    void   *m_data;
};

class TimerServer{
public:
	TimerServer();
	virtual ~TimerServer();

    void Register(TimeEvent* eventInst);  	//将一个事件加入执行队列
    void Release(TimeEvent* eventInst);  	//将一个事件从执行队列中移走
    void StartServer();                     //启动Timer
    void StopServer();                        //关闭Timer

private:
    std::list<TimeEvent*> m_events;  		//事件的执行队列
    pthread_t m_thread;    				    //保存执行函数的thread id,以便执行join操作
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex; 				//用于保护eventQueue
    bool m_shutdown;    				    //关闭标志
private:
	static void wait(TimerServer *self, int timeInMs);				// time in ms
    static void* ExecuteFunc(void* ptr);  	//执行函数        
};

#endif //TIMER_H_