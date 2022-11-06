#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t  p_thread[2];   //쓰레드 아이디
int a = 0;

void *thread1(void *data)
{
   int id;
   int i = 0;
   id = *((int *)data);
   while(1)
   {
      printf("Thread1 id[%d] a = %d i = %d \n", id, a, i);
      pthread_exit(0);
 
   }
}

void *thread2(void *data)
{
   int id;
   int i = 0;
   id = *((int *)data);
   while(1)
   {
      printf("Thread2 id[%d] a = %d i = %d \n", id, a, i);
      pthread_exit(0);
   }
}

int main()
{
   int test;
   int thread_id;
   int status;
   int a = 1;
   int b = 2;

   thread_id = pthread_create(&p_thread[0], NULL, thread1, (void *)&a);
   if(thread_id != 0)
      std::cout << "thread 1 create fail: " << thread_id << std::endl;

   thread_id = pthread_create(&p_thread[1], NULL, thread2, (void *)&b);
   if(thread_id != 0)
      std::cout << "thread 2 create fail: " << thread_id << std::endl;


   thread_id = pthread_join(p_thread[0], NULL);
   if(thread_id != 0)
      std::cout << "thread 1 Join fail : " << thread_id << std::endl;

   thread_id = pthread_join(p_thread[1], NULL);
   if(thread_id != 0)
      std::cout << "thread 2 Join fail : " << thread_id << std::endl;

   // pause();
   return 0;
}
