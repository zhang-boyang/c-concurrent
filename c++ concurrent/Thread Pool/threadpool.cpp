#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include "thread_pool.h"

using namespace std;

void func(void * arg) {
    int i = *(int *)arg;
    sleep(i); 
    cout << "this is func, arg:" << i   << "\n";
}
int vec_num[] = {1,2,3,4,5,6,7,8,9,10,11};
int main(int argc, char** argv) 
{
    ThreadPool thread_pool(10);
    for(int i = 0; i < 11; i++){
        Task task;
        task.f = func;
        task.arg = &vec_num[i];
        thread_pool.AddTask(task);
    }
    sleep(15);
    thread_pool.StopPool();
}