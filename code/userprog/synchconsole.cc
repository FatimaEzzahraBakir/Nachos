#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
SynchConsole::SynchConsole(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
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
  console->PutChar (ch);
  writeDone->P ();
}

int SynchConsole::SynchGetChar()
{
  readAvail-> P ();
  return console->GetChar ();
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
