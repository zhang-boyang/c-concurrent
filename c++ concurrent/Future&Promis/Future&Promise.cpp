#include <thread>
#include <iostream>
#include <functional>
#include <unistd.h>
using namespace std;

template <class T>
class Future {
    public:
    Future(thread&& t, T *ret):t_(move(t)), ret_(ret){};
    Future(Future<T>&&) = default;
    T get() {
        t_.join();
        return *ret_;
    }
    ~Future() {
        delete ret_;
    }
    thread t_;
    T* ret_;
};

template <class T>
class Promise{
    public:
        explicit Promise(function<T (void*)> func, void *arg) {
            ret_ = new T;
            thread t([=](){*ret_ = func(arg);});
            t_ = move(t);
        }
        Future<T> get_furture() {
            return Future<T>(move(t_), ret_);
        }
        thread t_;
        T *ret_;
};

int RetNum(void* arg) {
    int* ret = (int *) arg;
    sleep(*ret);
    return *ret;
}

int main(int argc, char** argv) {
    int a = 3;
    int b = 4;
    Promise<int> promise_a = Promise<int>(RetNum, &a);
    Promise<int> promise_b = Promise<int>(RetNum, &b);

    Future<int> f_a = promise_a.get_furture();
    Future<int> f_b = promise_b.get_furture();

    cout << f_a.get() + f_b.get() << endl;
    return 0;
}