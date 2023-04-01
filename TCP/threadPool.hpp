#pragma once

#include <iostream>
#include <unistd.h>
#include <queue>
#include <pthread.h>

#define NUM 5 // define定义后面没有分号

template <class T>
class ThreadPool
{
private:
    bool IsEmpty()
    {
        return _task_queue.size() == 0;
    }

    void LockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void UnlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void Wait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void WakeUp()
    {
        pthread_cond_signal(&_cond);
    }
public:
    ThreadPool(int num = NUM)
        : _thread_num(num) // 初始化列表冒号后边没有分号
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

    // Routine例程不断的去从任务队列中取任务去处理
    static void *Routine(void *arg)
    {
        // 分离线程使其不需要在pthread_join
        pthread_detach(pthread_self());
        ThreadPool *self = (ThreadPool *)arg;
        while (true)
        {
            // 如果任务队列为空则在空条件变量下等待
            // 线程进入到任务队列前加锁，而不是在Pop里加锁
            self->LockQueue();
            while (self->IsEmpty())
            {
                self->Wait();
            }
            T task;
            self->Pop(task);
            self->UnlockQueue();
            // 处理任务
            task.Run();
        }
    }

    void ThreadPoolInit()
    {
        pthread_t tid;
        for (int i = 0; i < _thread_num; i++)
        {
            // Routine例程是一个返回值、参数都是void*的函数
            // 而如果Routine是一个类内成员函数的话，其参数中会增加一个this指针的参数
            // 所以应将其设置为static静态成员函数
            pthread_create(&tid, nullptr, Routine, this);
        }
    }

    // 往任务队列塞任务（主线程调用）
    void Push(const T &task)
    {
        LockQueue();
        _task_queue.push(task);
        UnlockQueue();
        // 唤醒在空条件变量下等待的线程
        WakeUp();
    }

    void Pop(T &task)
    {
        // LockQueue();
        task = _task_queue.front();
        _task_queue.pop();
        // UnlockQueue();
    }

private:
    std::queue<T> _task_queue;
    int _thread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};

///////////////////////////////////////////////////////////////////////////////
#pragma once

// #include <iostream>
// #include <unistd.h>
// #include <queue>
// #include <pthread.h>

// // #define NUM 5

// // 线程池
// template <class T>
// class ThreadPool
// {
// public:
//     ThreadPool(int num = NUM)
//         : _thread_num(num)
//     {
//         pthread_mutex_init(&_mutex, nullptr);
//         pthread_cond_init(&_cond, nullptr);
//     }
//     ~ThreadPool()
//     {
//         pthread_mutex_destroy(&_mutex);
//         pthread_cond_destroy(&_cond);
//     }
//     // 线程池中线程的执行例程
//     static void *Routine(void *arg)
//     {
//         pthread_detach(pthread_self());
//         ThreadPool *self = (ThreadPool *)arg;
//         // 不断从任务队列获取任务进行处理
//         while (true)
//         {
//             self->LockQueue();
//             while (self->IsEmpty())
//             {
//                 self->Wait();
//             }
//             T task;
//             self->Pop(task);
//             self->UnLockQueue();

//             task.Run(); // 处理任务
//         }
//     }
//     void ThreadPoolInit()
//     {
//         pthread_t tid;
//         for (int i = 0; i < _thread_num; i++)
//         {
//             pthread_create(&tid, nullptr, Routine, this); // 注意参数传入this指针
//         }
//     }
//     // 往任务队列塞任务（主线程调用）
//     void Push(const T &task)
//     {
//         // LockQueue();
//         _task_queue.push(task);
//         // UnLockQueue();
//         WakeUp();
//     }
//     // 从任务队列获取任务（线程池中的线程调用）
//     void Pop(T &task)
//     {
//         LockQueue();
//         task = _task_queue.front();
//         _task_queue.pop();
//         UnLockQueue();
//     }
// private:
//     bool IsEmpty()
//     {
//         return _task_queue.size() == 0;
//     }
//     void LockQueue()
//     {
//         pthread_mutex_lock(&_mutex);
//     }
//     void UnLockQueue()
//     {
//         pthread_mutex_unlock(&_mutex);
//     }
//     void Wait()
//     {
//         pthread_cond_wait(&_cond, &_mutex);
//     }
//     void WakeUp()
//     {
//         pthread_cond_signal(&_cond);
//     }
// private:
//     std::queue<T> _task_queue; // 任务队列
//     int _thread_num;           // 线程池中线程的数量
//     pthread_mutex_t _mutex;
//     pthread_cond_t _cond;
// };