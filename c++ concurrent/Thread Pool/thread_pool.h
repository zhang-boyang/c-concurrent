#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <queue>
#include <functional>

using namespace std;

struct Task{
    function<void (void *) > f;
    void * arg;
};


class ThreadPool {
 public:
    ThreadPool(int thread_size);
    ~ThreadPool();
    void AddTask(Task &);
    void StopPool();

 private:
    int thread_size_;
    pthread_mutex_t mutex_;
    pthread_cond_t empty_cond_;
    bool is_stop;
    queue<Task> task_list_;
    vector<thread *> thread_vec_;
};

ThreadPool::ThreadPool(int thread_size) 
{
    is_stop = false;
    thread_size_ = thread_size;
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&empty_cond_, NULL);
    for(int i = thread_size_; i > 0; i--) {
        thread *t = new thread([=](){
            while(true){
                pthread_mutex_lock(&mutex_);
                while(task_list_.empty() && !is_stop) {
                    pthread_cond_wait(&empty_cond_, &mutex_);
                }
                if (is_stop) {
                    pthread_mutex_unlock(&mutex_);
                    return;
                }
                Task task = task_list_.front();
                task_list_.pop();
                pthread_mutex_unlock(&mutex_);
                
                task.f(task.arg);
            }
        });
        thread_vec_.push_back(move(t));
    }
}

void ThreadPool::AddTask(Task& task) 
{
    pthread_mutex_lock(&mutex_);
    task_list_.push(task);
    pthread_mutex_unlock(&mutex_);
    pthread_cond_signal(&empty_cond_);
}

ThreadPool::~ThreadPool() 
{
    cout << "~ThreadPool start\n";
    for(auto* item : thread_vec_) {
        if (item->joinable()) item->join();
        delete item;
    }
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&empty_cond_);
    cout << "~ThreadPool end\n";
}

void ThreadPool::StopPool() 
{
    pthread_mutex_lock(&mutex_);
    is_stop = true;
    pthread_mutex_unlock(&mutex_);
    pthread_cond_broadcast(&empty_cond_);
}