#include "runnable.h"
#include "logger.h"

Runnable::Runnable(RunnbaleCallback *callback) {
	mCallback = callback;
}

Runnable::~Runnable() {

}

void Runnable::run() {
	LOGE("Runnable::run!");
	if (mCallback != NULL) {
		mCallback(NULL);
	} 
}