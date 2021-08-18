#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<vector>
using namespace std;

#define DEBUG_FUHU(format,...) {printf("FUHU>>Line: %d ", __LINE__);printf(format, ##__VA_ARGS__);printf("\n");fflush(stdout);}

#define CHUNK_SIZE (35*1024)
// from https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
    
}
int getRAMUsage(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
vector<void*> g_pointerList;

void myFree()
{
    int num = 0;
    vector<void*>::iterator it;
    for(it=g_pointerList.begin();it!=g_pointerList.end();it++)
   {
        free(*it);
        num++;
    }
    DEBUG_FUHU("total freed: %d nodes", num);
}

int main(int argc, char ** argv) {
  int N = argc > 1 ? atoi(argv[1]) : 100;
  int maxmem = getRAMUsage();
  int total_size = N * 1024*1024;
  int remain_size = total_size;
  int loop = (total_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
  char * data;
  for(int k = 0; k < loop; k++) 
  {
    int current_size = 0;
    if (k == (loop -1)) 
    {
        current_size = remain_size;
    }
    else
    {
        current_size = CHUNK_SIZE;
    }
    
    if (current_size == 0)
    {
         printf("current_size = 0\n");
         exit(1);
    }

    data = (char*) malloc((current_size+ k*2));
    
    if(data == NULL) {
     printf("allocation failure\n");
     exit(1);
    }
    g_pointerList.push_back((void*)data);
    memset(data,0,current_size);
    if  (((k % 100)  == 0) || (k == (loop -1) ) )
    {
        int tmem = getRAMUsage();
        if(tmem > maxmem) 
        {
            maxmem = tmem;
            DEBUG_FUHU("update maxmem as %d kB ", maxmem);
        }
    }
    //free(data);
    remain_size -= current_size;
    //DEBUG_FUHU("k = %d,current_size:%d,  remain_size:%d, total_size:%d", k, current_size, remain_size, total_size);
  }
  for (int i = 0; i < 3; i++)
  {
     printf("Before release, ram usage: %d kB, max is %d kB \n", getRAMUsage(), maxmem);
     sleep(5);
  }

  myFree();

  do 
  {
    printf("After release,  ram usage: %d kB, max is %d kB \n", getRAMUsage(), maxmem);
    sleep(10);
  } while(1);

}
