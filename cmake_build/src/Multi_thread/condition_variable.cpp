#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <deque>
#include <string>
#include <thread>

void talker(std::deque<std::string>* msg, int thread_idx, std::mutex* my_mtx, std::condition_variable* cv)
{
    for(int i=0; i<5; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * thread_idx));
        std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
        int start_count = time.time_since_epoch().count();
        std::string msg_content = "idx[" + std::to_string(thread_idx) 
                                  + "][" + std::to_string(start_count) + "] Hello world!! \n";
        my_mtx->lock();
        msg->emplace_back(msg_content);
        my_mtx->unlock();

        cv->notify_one();
    }
}

void listener(std::deque<std::string>* msg, int* count, std::mutex* my_mtx, std::condition_variable* cv)
{
    while(*count<25)
    {
        std::unique_lock<std::mutex> ul(*my_mtx);
        cv->wait(ul, [&](){return msg->empty()==false || (*count)==25;});
        
        if(*count == 25)
        {
            ul.unlock();
            return;
        }
        std::string content = msg->front();
        msg->pop_front();

        (*count)++;
        ul.unlock();

        std::cout << content;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main()
{
    std::deque<std::string> msgs;
    std::mutex my_mtx;
    std::condition_variable cv;
    
    std::deque<std::thread> talkers;
    for(int i=0; i<5; i++)
    {
        talkers.emplace_back(std::thread([&](){
            talker(&msgs, i+1, &my_mtx, &cv);
        }));
    }

    int count(0);
    std::deque<std::thread> listeners;
    for(int i=0; i<3; i++)
    {
        listeners.emplace_back(std::thread([&](){
            listener(&msgs, &count, &my_mtx, &cv);
        }));
    }

    for(int i=0; i<5; i++)
    {
        talkers[i].join();
    }
    
    cv.notify_all();
    for(int i=0; i<3; i++)
    {
        listeners[i].join();
    }
    return 0;
}