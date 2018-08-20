/*
 * coroutine.h
 *
 *  Created on: 2018年8月7日
 *      Author: zhangby
 */

#ifndef COROUTINE_H_
#define COROUTINE_H_
#include <stdio.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include <ucontext.h>
#include <list>
#include <functional>

namespace COROUTINE {

enum CorStatus{
	stop = 0,
	suspend = 1,
	running = 2,
};

class CoroutineBase {
 public:

	ucontext_t ctx_;
	ucontext_t* main_ctx_;
	explicit CoroutineBase(ucontext_t*, void *);
	virtual ~CoroutineBase();
	void* arg_;
	CorStatus get_status();
	void* yield_;

	//function zone
	void* Yield(void* yield = NULL);
	void WrapperFunc(void* arg);
	virtual void Func(void* arg) = 0;

 private:
	void *stack_ptr_;
	CorStatus status_;

};

class CoroutineMain {
 public:
	ucontext_t ctx_main_;
	std::list<CoroutineBase*> list_ctx_;
	void CreateCoroutine(CoroutineBase* cor);
	void Run();
	CoroutineMain();
	~CoroutineMain();
 private:
	void *stack_ptr_;
};

}


#endif /* COROUTINE_H_ */
