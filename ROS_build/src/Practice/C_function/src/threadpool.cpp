#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop;

public:
    ThreadPool(size_t numThreads) : stop(false)
    {
        for(size_t i=0; i<numThreads; i++)
        {
            workers.emplace_back([this](){
                while(true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this](){return stop || tasks.empty()==false;});

                        if(stop && tasks.empty())
                        {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template <typename F>
    void enqueue(F&& func)
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(func));
        }
        cv.notify_all();
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();

        for(std::thread& worker : workers)
        {
            if(worker.joinable()==true){worker.join();}
        }
    }
}; 

int main(int argc, char** argv)
{
    ThreadPool threadpool(10);

    for(int i=0; i<10; i++)
    {
        threadpool.enqueue([&i](){
            std::cout << "Task " << i << " executed by thread " << std::this_thread::get_id() << std::endl;
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}