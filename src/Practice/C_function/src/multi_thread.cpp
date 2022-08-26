#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

void *PrintHello(void *threadid) {
   long tid;
   tid = (long)threadid;
   std::cout << "Hello World! Thread ID, " << tid << std::endl;
   pthread_exit(NULL);
}

int main () {
   pthread_t threads[NUM_THREADS];
   int rc;
   int i;
   
   for( i = 0; i < NUM_THREADS; i++ ) {
      std::cout << "main() : creating thread, " << i << std::endl;
      rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
      
      if (rc) {
         std::cout << "Error:unable to create thread," << rc << std::endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}