#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/memlayout.h"
#include "kernel/stat.h"
#include "user/user.h"

/* Assignment 5, question 3
 *
 * Modify this program to prevent race conditions. Use semaphore system calls.
 * Semaphore system calls are defined in user/user.h and implemented in kernel/sem.c.
 * Please refer to example programs and lecture notes posted on Brightspace for more detail.
 *
 * This program is a bare-bones example of the thread pool design pattern, which is
 * also called "replicated workers pattern" or "thread crew pattern". The idea is that
 * the application uses a small number of identical "worker" processes for processing a large
 * array of data items. It works as follows: initially all worker processes are idle.
 * An idle worker process tries to get the next portion of data from the data array,
 * and - if successful - immediately starts processing it.  If there is no more data
 * to process, the worker process terminates.
 *
 * Function main() initialises the array as follows:
 *
 * a[0]=0
 * a[1]=1
 * a[2]=2
 * a[3]=3
 * a[4]=4
 * a[5]=5
 * a[6]=6
 * a[7]=7
 * a[8]=8
 * a[9]=9
 *
 * The processing consists in incrementing each item's value by 1 in a loop 100000 times.
 * The correct values *after* processing should be
 *
 * a[0]=100000
 * a[1]=100001
 * a[2]=100002
 * a[3]=100003
 * a[4]=100004
 * a[5]=100005
 * a[6]=100006
 * a[7]=100007
 * a[8]=100008
 * a[9]=100009
 *
 * Please feel free to add/rearrange code and semaphores, as you see fit, to fix this program.
 */

// Number of data items in the array
#define N (10)

// Number of worker processes
#define M (3)

struct data {
  int value;
  char processed;
};

struct shared_space {
  struct data a[N];  // array of N items
};

// situate shared_space in the shared virtual memory page
volatile struct shared_space *s = (volatile struct shared_space*) 0x3FFFFFD000;

// This method gets the next piece of data to be processed
int get_item(int sem)
{
   int i;

   for (i = 0; i < N; i++)
   {
      if (s->a[i].processed == 0)
      {
	 break;
      }
   }

   if (i==N)    // has all data been processed?
   {
     i = -1;    // yes, return -1 in place of the item index
   }

   sem_wait(sem, 1);

   return i;
}

// worker process - keeps getting items from the array until there is no more
void worker(int sem)
{
   int i;

   printf("In worker process\n");

   // Line added to protect data from being accessed by multiple processes
   //sem_wait(sem, 1);

   for(i = get_item(); i>=0; i = get_item())
   {
	// process i-th element of the array a[] by incrementing it 100000 times.


	// --------- do not modify the increment loop  --------
	for (int j=0; j <100000; j++)
	{
	   s->a[i].value++;
       if (j == 99999)
        printf("Value %d processed\n", i);
	}
	// ---------------------------------------------------

	s->a[i].processed = 1;
    sem_post((sem + 1) % M, 1);
   }

   //sem_post((sem + 1) % M, 1);
   //printf("Waiting to exit\n");

   exit(0);
}

int
main(int argc, char *argv[])
{
  int i;

  // initialise shared space
  printf("Initialising shared space...\n");
  for (i=0; i<N; i++)
  {
     s->a[i].value = i;
     s->a[i].processed = 0;
  }

  // Lines added to open semaphores, with the first process getting priority
  sem_open(0, 1);
  sem_open(1, 0);
  sem_open(2, 0);

  // create worker processes
  printf("Creating worker processes ...\n");
  for (i = 0; i<M; i++)
  {
     if (fork() == 0) {
         printf("Process %d starting\n", i);
         worker(i); // start worker process
         printf("Process %d finished\n", i);
     }
  }

  // wait for the child processes to terminate
  for (i=0; i<M; i++)
  {
     wait(0);
  }

  // Closing semaphores at the end when they are not needed anymore
  sem_close(0);
  sem_close(1);
  sem_close(2);

  // print out the processed array
  printf("Processed data in the array:\n");
  for (i=0; i<N; i++)
  {
     printf("a[%d] = %d\n",i,s->a[i].value);
  }

  return 0;
}
