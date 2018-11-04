#ifdef CHANGED
#include "system.h"
#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "copyright.h"
#include "synch.h"

 static Semaphore *mutexThread = new Semaphore ("nombre de threads",1);
typedef  struct schmurtz{
  int f;
  int arg;
  int val_return;
}schmurtz;



static void StartUserThread(void *Schmurtz){

    DEBUG('x',"mdebug %d\n");
    int i;
    schmurtz *s=(schmurtz*)Schmurtz;
    for (i = 0; i < NumTotalRegs; i++)


      machine->WriteRegister (i, 0);
      machine->WriteRegister (4, s->arg);
      // Initial program counter -- must be location of "Start"
      machine->WriteRegister (PCReg,s->f );
      machine->WriteRegister(RetAddrReg,s->val_return);

      // Need to also tell MIPS where next instruction is, because
      // of branch delay possibility
      machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

      // Set the stack register to the end of the address space, where we
      // allocated the stack; but subtract off a bit, to make sure we don't
      // accidentally reference off the end!
      machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack() - 16);
      DEBUG ('a', "Initializing stack register to 0x%x\n",currentThread->space->AllocateUserStack()- 16);

      machine->Run();
}

int do_ThreadCreate(int f, int arg)
{

  schmurtz *s=(schmurtz*)malloc(sizeof(struct schmurtz));
  Thread *newthread = new Thread ("first thread");
  s->f=f;
  s->arg=arg;
  s->val_return=machine->ReadRegister(6);
  newthread->Start(StartUserThread,(void*)s);
  mutexThread->P();
  currentThread->space->nombre_thread++;
  mutexThread->V();

}

void do_ThreadExit(){

  if(currentThread->space->nombre_thread>0){
  mutexThread->P();
  currentThread->space->nombre_thread--;
  mutexThread->V();
  currentThread->Finish();
  }
  else
  {interrupt->Halt();
  currentThread->Finish();
  }

}
#endif
