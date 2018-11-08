#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *mutexPut ;
static Semaphore *mutexGet ;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
SynchConsole::SynchConsole(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
mutexPut = new Semaphore ("mutexPut",1);
mutexGet = new Semaphore ("mutexGet",1);
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);
}
SynchConsole::~SynchConsole()
{
delete console;
delete writeDone;
delete readAvail;
}
void SynchConsole::SynchPutChar(int ch)
{
  mutexPut->P();
  console->PutChar (ch);
  writeDone->P ();
  mutexPut->V();
}

int SynchConsole::SynchGetChar()
{
  readAvail-> P ();
  mutexGet->P();
  int c= console->GetChar ();
  mutexGet->V();
  return c;
}
void SynchConsole::SynchPutString(const char s[])
{
for(int i=0;s[i]!='\0';i++){
  SynchPutChar(s[i]);
}
}
void SynchConsole::SynchGetString(char *s, int n)
{
  char c;
  int i;
  for(i=0;i<n;i++){
  c=SynchGetChar();
  if(c==EOF || c=='\n')
  {
    s[i]='\0';
    break;
  }
  s[i]=c;
  }
}

void SynchConsole::SynchPutInt(int N)
{
char *T=(char *)malloc(sizeof(char)*MAX_INT_SIZE);
snprintf(T,MAX_INT_SIZE,"%d",N);
SynchPutString(T);
free(T);
}

void SynchConsole::SynchGetInt(int *N)
{
char *c=(char*)malloc(sizeof(char)*MAX_INT_SIZE);
SynchGetString(c,MAX_INT_SIZE);
sscanf(c,"%d",N);
free(c);
}
#endif // CHANGED
