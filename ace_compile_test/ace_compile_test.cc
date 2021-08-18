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
/*
struct timeval
{
__time_t tv_sec;        
__suseconds_t tv_usec;  
};

*/
//typedef  int time_t;
//typedef  long suseconds_t;

class ACE_Time_Value
{
    public:
    ACE_Time_Value (time_t sec, suseconds_t usec = 0);
    ACE_Time_Value();
    ACE_Time_Value(const ACE_Time_Value& other);
    void sec (time_t sec);
    time_t sec ();
    void usec (suseconds_t sec);
    suseconds_t usec ();
    operator timespec_t () const;
    public:
          timeval tv_;
};

ACE_Time_Value::ACE_Time_Value(time_t sec, suseconds_t usec)
{
    tv_.tv_sec = sec;
    tv_.tv_usec = usec;
    printf("ACE_Time_Value::ACE_Time_Value(time_t sec, suseconds_t usec) construct\n");
}

ACE_Time_Value::ACE_Time_Value()
{
    tv_.tv_sec = 0;
    tv_.tv_usec = 0;
    printf("ACE_Time_Value::ACE_Time_Value() construct\n");
}
ACE_Time_Value::ACE_Time_Value(const ACE_Time_Value& other)
{
    tv_.tv_sec = other.tv_.tv_sec;
    tv_.tv_usec = other.tv_.tv_usec;
    printf("ACE_Time_Value::ACE_Time_Value(const ACE_Time_Value& other) construct\n");
}


inline ACE_Time_Value::operator timespec_t () const
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator timespec_t");
  timespec_t tv;
  tv.tv_sec = this->tv_.tv_sec;
  // Convert microseconds into nanoseconds.
  tv.tv_nsec = this->tv_.tv_usec * 1000;
  printf("call timespec_t() operator\n");
  return tv;
}
void ACE_Time_Value::sec (time_t sec)
{
    this->tv_.tv_sec = sec;
}
time_t ACE_Time_Value::sec ()
{
    return this->tv_.tv_sec;
}
void ACE_Time_Value::usec (suseconds_t sec)
{
    this->tv_.tv_usec = sec;
}
suseconds_t ACE_Time_Value::usec ()
{
    return this->tv_.tv_usec;
}


 inline int sleep ( ACE_Time_Value &tv)
    {
        static int print_count = 0;
        timespec_t rqtp;
        rqtp.tv_sec = tv.sec();
        rqtp.tv_nsec = tv.usec()*1000;
        if (print_count==0)
        {
            printf("set sleep tv_sec: %lld, tv_nsec: %lld\n",
                (long long)rqtp.tv_sec, (long long)rqtp.tv_nsec);
        }
        nanosleep (&rqtp, 0);
        print_count++;
        return 0;
    }

 double difftimeval(const struct timeval *start, const struct timeval *end)
 {
         double d;
         time_t s;
         long u;
         s = end->tv_sec - start->tv_sec;
         u = end->tv_usec - start->tv_usec;
         if (u < 0)
         {
            --s;
            u+= 1000000;
         }
         d = s + u/1000000.0;
         return d;
}

 int main (int argc, char *argv[])
 {
    int nsec =100*1000*1000; //100ms
    int ticks = 100;  //default 100ms * 100 = 10s
    if(argc > 2)
    {
      nsec = atoi(argv[1]);
      ticks = atoi(argv[2]);
    }
    #if 1
    ACE_Time_Value instant(2, 1); // 1us
    timespec_t rqtp = instant;
    printf("begin to sleep\n");
    nanosleep (&rqtp, 0);
    printf("end to sleep\n");
    #else
    printf("nano sec is %d,  ticks are %d\n", nsec, ticks);
    struct timeval tv1, tv2;
    printf("#1: size of struct timeval (tv_sec)= %d, size of (tv_usec) = %d\n", 
        sizeof(tv1.tv_sec), sizeof(tv1.tv_usec));
    //exit(1);
    timespec_t rqtp;
    printf("#2: size of struct timespec_t (tv_sec)= %d, size of (tv_usec) = %d\n", 
        sizeof(tv1.tv_sec), sizeof(tv1.tv_usec));


    printf("#3: size of (long) = %d\n", sizeof(long));
    printf("#3: size of (long long) = %d\n", sizeof(long long));
    gettimeofday(&tv1, NULL);
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = nsec; //100ms
    
    int sleep_count = 100 *1000 * 1000;
    ACE_Time_Value instant(0, 1); // 1us
    instant.sec(2);
    instant.usec(sleep_count);
    while(ticks > 0)
    {
        //nanosleep(&ts ,NULL);
        sleep(instant);
        ticks--;
    }
    double res = gettimeofday(&tv2, NULL);
    res = difftimeval(&tv1, &tv2);
    printf("Accumulate sleep time: %.3lf (s)\n",  res);
    #endif
 }
