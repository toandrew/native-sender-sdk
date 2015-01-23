#include "runnable.h"
#include "logger.h"

Runnable::Runnable(RunnbaleCallback *callback) {
	mCallback = callback;
}

Runnable::~Runnable() {

}

void Runnable::run(void *param) {
	LOGE("Runnable::run!");
	if (mCallback != NULL) {
		mCallback(param);
	} 
}