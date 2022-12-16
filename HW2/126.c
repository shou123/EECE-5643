
/* -------------------------------------------------------------------------
 * This program simulates a single-server FIFO service node using arrival
 * times and service times read from a text file.  The server is assumed
 * to be idle when the first job arrives.  All jobs are processed completely
 * so that the server is again idle at the end of the simulation.   The
 * output statistics are the average interarrival time, average service
 * time, the average delay in the queue, and the average wait in the service 
 * node. 
 *
 * Name              : ssq1.c  (Single Server Queue, version 1)
 * Authors           : Steve Park & Dave Geyer
 * Language          : ANSI C
 * Latest Revision   : 9-01-98
 * Compile with      : gcc ssq1.c 
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>                              

#define FILENAME   "ac.dat"                  /* input data file */
#define START      0.0

/* =========================== */
   double GetArrival(FILE *fp)                 /* read an arrival time */
/* =========================== */
{ 
  double a;

  fscanf(fp, "%lf", &a);
  return (a);
}

/* =========================== */
   double GetDeparture(FILE *fp)                 /* read a service time */
/* =========================== */
{ 
  double s;

  fscanf(fp, "%lf\n", &s);
  return (s);
}

/* ============== */
   int main(void)
/* ============== */
{
  FILE   *fp;                                  /* input data file      */
  int   index     = 0;                        /* job index            */
  double arrival   = START;                    /* arrival time         */
  double delay;                                /* delay in queue       */
  double service;                              /* service time         */
  double wait;                                 /* delay + service      */
  double departure = START;                    /* departure time       */
  struct {                                     /* sum of ...           */
    double delay;                              /*   delay times        */
    double wait;                               /*   wait times         */
    double service;                            /*   service times      */
    double interarrival;                       /*   interarrival times */
  } sum = {0.0, 0.0, 0.0};

  double previousDeparture = 0;
  double previousArrival = 0;
  double factor = 0;

  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return (1);
  }

  while (!feof(fp))
  {
    index++; 
    arrival = GetArrival(fp);
    departure = GetDeparture(fp);
    if(index == 500)
    {
      factor = departure / arrival;
    }
    if(index>1 && previousDeparture>arrival)
    {
      service = departure - previousDeparture;
      previousDeparture = departure;
    }
    else
    {
      service = departure - arrival;
      previousDeparture = departure;
    }

    // wait = departure - arrival;
    // sum.delay += delay;
    // sum.wait += wait;
    sum.service += service;
  }
  sum.interarrival = arrival - previousArrival;
  previousArrival = arrival;

  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time = %6.2f\n", sum.interarrival / index);
  printf("   average service time .... = %6.2f\n", sum.service / index);
  // printf("   average delay ........... = %6.2f\n", sum.delay / index);
  // printf("   average wait ............ = %6.2f\n", sum.wait / index);
  printf("   The server utilization............ = %0.2f%%\n", (1/(sum.interarrival / index))/(1/(sum.service / index))*100);
  printf("   The traffic intensity............ = %0.2f%%\n", factor*((1/(sum.interarrival / index))/(1/(sum.service / index)))*100);

  fclose(fp);
  return (0);
}
