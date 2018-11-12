#include "syscall.h"


void print(char c){
  int i=0;
  for(i=0;i<3;i++)
  { PutChar(c+i); }
    PutChar('\n');
    ThreadExit();
}
int main()
{


int threads=10;
int i=0;

for(i=0; i<threads; i++)
{
  ThreadCreate((void *)print,'a');
}

  //PutChar('m');
  //ThreadExit();
}
