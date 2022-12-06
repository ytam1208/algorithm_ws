#include <iostream>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

std::vector<int> test_array(10000000000);
pthread_t  p_thread[2];

void *thread1(void *data)
{
    int id;
    id = *((int *)data);
    int count = 0;

    struct timespec  begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    while(count > test_array.size()){
        count++;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
    printf("Thread id[%d] Time (Nano): %lf\n", id, (double)time);

    pthread_exit(0);
    pthread_detach(p_thread[id]);
}

void *thread2(void *data)
{
    int id;
    id = *((int *)data);
    int count = 0;

    int size = test_array.size();
    struct timespec  begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    while(count > size){
        count++;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long time = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
    printf("Thread id[%d] Time (Nano): %lf\n", id, (double)time);

    pthread_exit(0);
    pthread_detach(p_thread[id]);
}

int main()
{
    int thread_id, thread_id2;
    int status;
    int a = 1;
    int b = 2;
    int count = 0;
    int size = test_array.size();

    thread_id = pthread_create(&p_thread[0], NULL, thread1, (void *)&a);
    if(thread_id != 0)
        std::cout << "thread 1 create fail: " << thread_id << std::endl;

    thread_id2 = pthread_create(&p_thread[1], NULL, thread2, (void *)&b);
    if(thread_id2 != 0)
        std::cout << "thread 2 create fail: " << thread_id2 << std::endl;    

    // struct timespec  begin, end;
    // clock_gettime(CLOCK_MONOTONIC, &begin);
    // for(int i = 0; i < test_array.size(); i++){
    //     count++;
    // }
    // clock_gettime(CLOCK_MONOTONIC, &end);
    // long time = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
    // printf("For.size() Time (Nano): %lf\n", (double)time);

    // clock_gettime(CLOCK_MONOTONIC, &begin);
    // for(int i = 0; i < size; i++){
    //     count++;
    // }
    // clock_gettime(CLOCK_MONOTONIC, &end);
    // time = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
    // printf("For2 int size Time (Nano): %lf\n", (double)time);


    return 0;
}