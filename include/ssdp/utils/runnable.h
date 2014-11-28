#ifndef RUNNABLE_H_
#define RUNNABLE_H_

typedef void* (RunnbaleCallback)(void*);

class Runnable {
public:
	Runnable() {};
	Runnable(RunnbaleCallback *callback);
	virtual ~Runnable();

	void run();
private:
	RunnbaleCallback *mCallback;
};

#endif //RUNNABLE_H_