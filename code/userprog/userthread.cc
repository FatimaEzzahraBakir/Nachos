#ifdef CHANGED
#include "system.h"
#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "copyright.h"
#include "synch.h"

 static Semaphore *mutexThread = new Semaphore ("nombre de threads",1);
 static Semaphore *m = new Semaphore ("nombre de threads",1);



typedef  struct schmurtz{
  int f;
  int arg;
  int val_return;
}schmurtz;


//int cmpt=0;

int do_ThreadCreate(int f, int arg,int val)
{

 int x= currentThread->space->bitmap->Find();


 if(x==-1)
 {
  m->P();
  return -1;
 }

else
{
  m->P();
  schmurtz *s=(schmurtz*)malloc(sizeof(struct schmurtz));
  Thread *newthread = new Thread ("new thread");
  newthread->pos=x;
  s->f=f;
  s->arg=arg;
  s->val_return=val;
  mutexThread->P();
  currentThread->space->nombre_thread++;
  mutexThread->V();

  newthread->Start(StartUserThread,(void*)s);

  return 0;
}
}




static void StartUserThread(void *Schmurtz){

    DEBUG('x',"%s\n", currentThread->getName());
    int i;
    schmurtz *s=(schmurtz*)Schmurtz;

    //initialisation des registres
    for (i = 0; i < NumTotalRegs; i++)


      machine->WriteRegister (i, 0);

      //Placer les paramètres dans le registre 4
      machine->WriteRegister (4, s->arg);

      //écrire l'adresse de la fonction f
      machine->WriteRegister (PCReg,s->f );



      machine->WriteRegister (NextPCReg, (s->f) + 4);



      //cmpt++;
      //machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(cmpt);
      machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(currentThread->pos)-16);
    //printf("===========> %d \n",currentThread->space->AllocateUserStack(currentThread->pos)-16);
    //printf(" pos thread : %d\n",currentThread->pos);

      machine->WriteRegister(RetAddrReg,s->val_return);
      free(s);

      machine->Run();


}


void do_ThreadExit(){


  mutexThread->P();
  currentThread->space->nombre_thread--;
  mutexThread->V();
  currentThread->space->bitmap->Clear(currentThread->pos);

  m->V();
  currentThread->Finish();


}
#endif
