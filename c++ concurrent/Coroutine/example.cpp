/*
 * example.cpp
 *
 *  Created on: 2018年8月10日
 *      Author: zhangby
 */

#include "coroutine.h"
#include <iostream>

using namespace std;
using namespace COROUTINE;

class CorA : public CoroutineBase {
 public:
	CorA(ucontext_t* main_ctx, void* arg) : CoroutineBase(main_ctx, arg){}
	virtual void Func (void * arg) {
		int* x;
		cout << "start CorA " << (char *)arg << endl;
		x = (int *)Yield();
		cout << "x = " << *x << endl;
		cout << "end CorA" << endl;
		return;
	}
};

class CorB : public CoroutineBase {
 public:
	CorB(ucontext_t* main_ctx, void* arg) : CoroutineBase(main_ctx, arg){}
	virtual void Func (void * arg) {
		cout << "start CorB " << (char *)arg << endl;
		Yield();
		cout << "end CorB" << endl;
		return;
	}
};

int main(int argc, char* argv[]) {

	CoroutineMain ctx_main;

	CorA* A = new CorA(&ctx_main.ctx_main_, (void*)"Hello A");
	CorB* B = new CorB(&ctx_main.ctx_main_, (void*)"Hello B");

	ctx_main.CreateCoroutine(A);
	ctx_main.CreateCoroutine(B);

	ctx_main.Run();

	return 0;
}
