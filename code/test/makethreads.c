#include "syscall.h"


void print(char c){
  int i=0;
  for(i=0;i<30;i++)
  { PutChar(c); }
    PutChar('\n');
    ThreadExit();
}
int main()
{
  //ThreadCreate((void *)print,'a');
//ThreadCreate((void *)print,'k');
int threads=10;
int i=0;
for(i=0; i<threads; i++)
{
  ThreadCreate((void *)print,'a');

}

  //PutChar('m');
  ThreadExit();
}
