
/* ------------------------------------------------------------------------- 
 * gcc 334.c rng.c -o 334 -lm
 */

#include <stdio.h>
#include <math.h>    
#include <limits.h>                                         
#include "rng.h"

#define LAST         100000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 
#define CAP           1

// A structure to represent a queue
struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};
 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    //printf("%d enqueued to queue\n", item);
}
 
// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}
   double Exponential(double m)                 
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{                                       
  return (-m * log(1.0 - Random()));     
}


   double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}


   double GetArrival(void)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 */ 
{       
  static double arrival = START;                                        

  arrival += Exponential(2.0);
  return (arrival);
}
long Geometric(double p) /* use 0.0 < p < 1.0 */
{
    return ((long) (log(1.0 - Random()) / log(p)));
}

   double GetService(void)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
  return (Uniform(1.0, 3.0));
}


  int main(void)
{
  long   index     = 0;                         /* job index            */
  double arrival   = START;                     /* time of arrival      */
  double delay;                                 /* delay in queue       */
  double service;                               /* service time         */
  double wait;                                  /* delay + service      */
  double departure = START;                     /* time of departure    */
  struct {                                      /* sum of ...           */
    double delay;                               /*   delay times        */
    double wait;                                /*   wait times         */
    double service;                             /*   service times      */
    double interarrival;                        /*   interarrival times */
  } sum = {0.0, 0.0, 0.0};  

  PutSeed(123456789);
  int drop=0,drops=0;
  struct Queue* queue=createQueue(CAP);

  while (index < LAST) {
    drop=0;
    int inqueue=0;
    index++;
    arrival      = GetArrival();
    while(front(queue)<arrival&& !isEmpty(queue)){
      dequeue(queue);
      //printf(".\n");
    }
    if (arrival < departure){
      if(isFull(queue)){
        drop=1;
        drops++;
      }else{
        delay      = departure - arrival;         /* delay in queue    */
        inqueue=1;
      }
    }
    
      
    else
      delay      = 0.0;                         /* no delay          */
    if(!drop){
      service      = GetService();
      wait         = delay + service;
      departure    = arrival + wait;              /* time of departure */
      sum.delay   += delay;
      sum.wait    += wait;
      sum.service += service;
      if(inqueue) enqueue(queue, departure);
    }
    
  } 
  sum.interarrival = arrival - START;
  printf("Geometric arrival\n");
  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time = %6.2f\n", sum.interarrival / index);
  printf("   average wait ............ = %6.2f\n", sum.wait / index);
  printf("   average delay ........... = %6.2f\n", sum.delay / index);
  printf("   average service time .... = %6.2f\n", sum.service / index);
  printf("   average # in the node ... = %6.2f\n", sum.wait / departure);
  printf("   average # in the queue .. = %6.2f\n", sum.delay / departure);
  printf("   utilization ............. = %6.2f\n", sum.service / departure);
  printf("   probability of drop...... = %6.2f\n", (double)drops/index);

  return (0);
}
