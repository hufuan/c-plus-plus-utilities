#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

#define NUM_THREADS     5

 int stats[NUM_THREADS] = {0};
////300 s, 100*1000000 
 int nsec = 1000;
 int freq = 15000;  

 void showCurrentTime(long tid, ofstream &ostream)
{
    time_t time_seconds = time(0);                        
    struct tm currentTime;
    struct timeval tv;
    localtime_r(&time_seconds, &currentTime);
    char timeStr[256];
    sprintf(timeStr, "%04i/%02i/%02i-%02i:%02i/%02i\n",
            currentTime.tm_year + 1900, currentTime.tm_mon + 1,
            currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
    char tidStr[256];
    sprintf(tidStr, "Tid: [%03i] ", tid);
    printf("%s is alive @: %s", tidStr, timeStr); 
    if (ostream)
    {
        ostream << tidStr << " is alive @: " <<  timeStr;
	    ostream.flush();
    }
}

 std::string generateDumpFileName(long tid)
 {
     time_t time_seconds = time(0);                        
     struct tm currentTime;
     localtime_r(&time_seconds, &currentTime);
     char timeStr[256];
     sprintf(timeStr, "logs/tid_%03i_%04i%02i%02i_%02i%02i_%02i.log",
             tid,
             currentTime.tm_year + 1900, currentTime.tm_mon + 1,
             currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min,
             currentTime.tm_sec);
     return std::string(timeStr); 
 }

void wrapper_nanosleep1(struct timespec &ts)
{
    nanosleep(&ts ,NULL);
}

 void wrapper_nanosleep2( struct timespec &ts)
 {
     nanosleep(&ts ,NULL);
 }
 
 void wrapper_nanosleep_intf(int count, struct timespec &ts) 
 {
    void (*p)(struct timespec &);
    if (count & 1)
    {
        p = wrapper_nanosleep2;
    }
    else
    {
        p = wrapper_nanosleep1;
    }
    p(ts);
 }

 void *nnsleep(void *threadid)
 {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = nsec;
    long tid;
    tid = (long)threadid;
    std::string drDumpFileName = generateDumpFileName(tid);
    ofstream outputfile;
    outputfile.open(drDumpFileName.c_str(), std::ios::out);
    if(!outputfile)
    {
        printf("open file error\n");
        //return -1;
    }
    else
    {
        char tmpStr[256];
        sprintf(tmpStr, "Start: nano sec is %d,  freq is %d.\n", nsec, freq);
        outputfile << tmpStr;
        sprintf(tmpStr, "Pid: %u, ThreadId: %u\n", (unsigned int)getpid(), (unsigned int)pthread_self () );
        outputfile << tmpStr;
        printf("%s", tmpStr);
        showCurrentTime(tid, outputfile);
    }

    int count = 0;
    while(1)
    {
        wrapper_nanosleep_intf(count, ts);
        stats[tid] += 1;
        if(stats[tid] > freq)
        {
          showCurrentTime(tid, outputfile);
          stats[tid] = 0;
          count = 0;
        }
        count++;
    }
    printf("Thread #%ld quits!\n", tid);
    if(outputfile)
    {
       outputfile.flush();
       outputfile.close();
    }
    pthread_exit(NULL);
 }

 int main (int argc, char *argv[])
 {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    if(argc > 2)
    { 
      nsec = atoi(argv[1]);
      freq = atoi(argv[2]);
    }
    printf("nano sec is %d,  freq is %d\n", nsec, freq);

    for(t=0; t<NUM_THREADS; t++){
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, nnsleep, (void *)t);
       if (rc)
       {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
       }
       usleep(100*1000);
    }

    /* Last thing that main() should do */
    for(t=0;t<NUM_THREADS;t++)
    {
        pthread_join(threads[t], NULL);
    }
 }
