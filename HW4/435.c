
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
 * Compile with      : gcc -std=c99 435.c -o 435 -lm
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>                              

#define FILENAME   "ac.dat"                  /* input data file */
#define START      0.0
#define t          400


double GetArrival(FILE *fp)   
{ 
    double a;

    fscanf(fp, "%lf", &a);
    return (a);
}


double GetDeparture(FILE *fp)  //new  
{ 
    double s;

    fscanf(fp, "%lf\n", &s);
    return (s);
}


int main(void)
{
    FILE   *fp;                                  /* input data file      */
    long   index     = 0;                        /* job index            */
    double arrival   = START;                    /* arrival time         */
    double delay;                                /* delay in queue       */
    double service;                              /* service time         */
    double wait;                                 /* delay + service      */
    double departure = START;                    /* departure time       */

    double arrivalLast   = 0;                    /* arrival time         *///new
    double delayLast;                                /* delay in queue       *///new
    double serviceLast;                              /* service time         *///new
    double waitLast;                                 /* delay + service      *///new
    double max_service=0;
    int* hist=malloc(sizeof(int)*16);
    int i;
    for (i = 0; i < 16; i++)
        hist[i] = 0;
    double max_delay=0; //new
    int delayCount=0; //new
    int atwhattime=0; //new
    double lastDeparture = 0;
    double mean=3.03;
    struct {                                     /* sum of ...           */
        double delay;                              /*   delay times        */
        double wait;                               /*   wait times         */
        double service;                            /*   service times      */
        double interarrival;                       /*   interarrival times */
    } sum = {0.0, 0.0, 0.0};

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open input file %s\n", FILENAME);
        return (1);
    }

    while (!feof(fp)) {
        index++;
        arrival      = GetArrival(fp);
        if (arrival < lastDeparture) {
            delay      = lastDeparture - arrival;        /* delay in queue    */
            delayCount++; //new
        }
        
        else delay      = 0.0; /* no delay          */
        departure    = GetDeparture(fp);             /* time of departure */                      
        service      = (arrival>lastDeparture)? departure-arrival : departure-lastDeparture;//new
        if(max_service<service) max_service=service;
        wait         = delay + service;
        
        lastDeparture=departure;
        if(arrival<t && departure>t) atwhattime++; //new
        if(delay>max_delay) max_delay=delay; //new
        sum.delay   += delay;
        sum.wait    += wait;
        sum.service += service;

        hist[(int)service]++;


    }
    
    double trafficInensity=sum.service/lastDeparture*(departure/arrival);//new
    sum.interarrival = arrival - START;
    double proportion=(double)delayCount/1000.0*100;//new
    printf("\nfor %ld jobs\n", index);
    printf("   average interarrival time = %6.2f\n", sum.interarrival / index);
    printf("   average service time .... = %6.2f\n", sum.service / index);
    printf("   average delay ........... = %6.2f\n", sum.delay / index);
    printf("   average wait ............ = %6.2f\n", sum.wait / index);
    printf("   Longest service time .....= %6.2f\n", max_service);
    //printf("   Max delay experienced ... = %6.2f\n", max_delay); //new
    //printf("   Job in service node at %d = %d\n", t,atwhattime); //new
    //printf("   Proporton of jobs delayed = %6.2f%%\n", proportion); //new
    //printf("   Server utilization ...... = %6.2f%%\n", sum.service/lastDeparture*100); //new
    //printf("   Traffic Intensity ....... = %6.2f\n", trafficInensity); //new

    for (i = 0; i < 16; i++)
    {
        printf("Bin[%2d]: %4d %7.2f\n",i,hist[i],(double)hist[i]/index);
    }

    double hist_mean=0;
    double std_temp=0;
    for(i=0;i<15;i++){
        double mj=(double)i+0.5;
        double pj=(double)hist[i]/index;
        hist_mean+=mj*pj;
        std_temp+=mj*mj*pj;
    }
    std_temp=std_temp-hist_mean*hist_mean+(1/12);
    double std=sqrt(std_temp);
    printf("histogram mean=%5.2f\n",hist_mean);
    printf("Std=%5.2f\n",std);
    

    fclose(fp);
    return (0);
}
