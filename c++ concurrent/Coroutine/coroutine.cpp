/*
 * coroutine.cpp
 *
 *  Created on: 2018年8月8日
 *      Author: zhangby
 */
#include "coroutine.h"

namespace COROUTINE{

CoroutineBase::CoroutineBase(ucontext_t* main_ctx, void* arg):
		status_(CorStatus::suspend), stack_ptr_(NULL), main_ctx_(main_ctx),
		arg_(arg), yield_(NULL)
{
	if (main_ctx_ == NULL) return;
	stack_ptr_ = new char[1024 * 32];
	getcontext(&ctx_);
	ctx_.uc_stack.ss_sp = stack_ptr_;
	ctx_.uc_stack.ss_size = 1024 * 32;
	ctx_.uc_stack.ss_flags = 0;
	ctx_.uc_link = main_ctx;
	makecontext(&ctx_, (void (*) (void)) &CoroutineBase::WrapperFunc, 2, this, arg_);
}
CoroutineBase::~CoroutineBase() {
	delete[] stack_ptr_;
}

CorStatus CoroutineBase::get_status() {
	return status_;
}
void CoroutineBase::WrapperFunc(void *arg) {
	Func(arg);
	status_ = CorStatus::stop;
}

void* CoroutineBase::Yield(void * yield) {
	if (yield != NULL) yield = yield_;
	status_ = CorStatus::suspend;
	swapcontext(&ctx_, main_ctx_);
	status_ = CorStatus::running;
	return yield_;
}

void CoroutineMain::CreateCoroutine(CoroutineBase *cor) {
	if (cor != NULL) list_ctx_.push_back(cor);
}

CoroutineMain::CoroutineMain() {
	stack_ptr_ = new char[1024 * 32];
	getcontext(&ctx_main_);
	ctx_main_.uc_stack.ss_sp = stack_ptr_;
	ctx_main_.uc_stack.ss_size = 1024 * 32;
	ctx_main_.uc_stack.ss_flags = 0;
	makecontext(&ctx_main_, (void (*) (void)) &CoroutineMain::Run, 1, this);
}
void CoroutineMain::Run() {
	static int x = 8;
	auto list_it = list_ctx_.begin();
	while(!list_ctx_.empty()) {
		if ((*list_it)->get_status() == CorStatus::stop) {
			list_it = list_ctx_.erase(list_it);
		} else if ((*list_it)->get_status() == CorStatus::suspend) {
			ucontext_t* ctx = &(*list_it)->ctx_;
			(*list_it)->yield_ = (void *)&x;
			swapcontext(&ctx_main_, ctx);
			list_it++;
			if (list_it == list_ctx_.end()) list_it = list_ctx_.begin();
		}
	}
}
CoroutineMain::~CoroutineMain() {
	delete [] stack_ptr_;
}

}


