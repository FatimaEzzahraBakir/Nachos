// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}




//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------



void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;
		  }

      #ifdef CHANGED
      case SC_Exit:
  		  {
          while(currentThread->space->nombre_thread>0) {
		      currentThread->Yield(); }
  		    DEBUG ('s', "Exit.\n");
          //on affiche la valeur de retour stocke dans le registre 4
          printf("Exit %d\n",machine->ReadRegister(4) );

  		    interrupt->Halt ();
  		    break;
  		  }
 		case SC_PutChar:
		  {
        //on place le caractere dans le registre 4
		    char ch=machine->ReadRegister (4);
		    DEBUG ('s', "Putchar\n");
		    synchconsole->SynchPutChar(ch);
		    break;
		  }

		  case SC_PutString:
		  {
			DEBUG ('s', "PutString\n");
			char adresse[MAX_STRING_SIZE];
			int R;
			R = machine->ReadRegister(4);
      int taille_copie=MAX_STRING_SIZE-1;
      //pour gerer une chaine de taille grande
      while(taille_copie==MAX_STRING_SIZE-1)
      {
		  taille_copie=copyStringFromMachine(R,adresse ,MAX_STRING_SIZE);
		  synchconsole->SynchPutString(adresse);
      R+=taille_copie;
      }
		   break;
      }

      case SC_GetChar:
  		  {
          DEBUG ('s', "GetChar\n");
          //on stock le caractere lu dans le registre 2
		      machine->WriteRegister (2,synchconsole->SynchGetChar());
		      break;

  		  }

      case SC_GetString:
    		  {
           DEBUG ('s', "GetString\n");

  		      int adresse;
            int n=machine->ReadRegister(5);
            char *s= (char*) malloc (sizeof(char) *n);
            adresse=machine->ReadRegister(4);

            synchconsole->SynchGetString(s,MAX_STRING_SIZE);
            int taille=copyStringToMachine(adresse,s,MAX_STRING_SIZE);

            int i=0;
            while(taille==MAX_STRING_SIZE && i<n){
              n-=MAX_STRING_SIZE;
              i+=MAX_STRING_SIZE;
            synchconsole->SynchGetString(s,MAX_STRING_SIZE);
            taille=copyStringToMachine(adresse+i,s,MAX_STRING_SIZE);

            }
            free(s);
            break;
          }

          case SC_PutInt:
      		  {
              DEBUG ('s', "PutInt\n");

      		    int N=machine->ReadRegister (4);
      		    synchconsole->SynchPutInt(N);
              printf("\n" );
      		    break;
      		  }

        case SC_GetInt:
              {

                DEBUG ('s', "GetInt\n");
                int N;
                synchconsole->SynchGetInt(&N);
                int c =machine->ReadRegister(4);
                machine->WriteMem(c,sizeof(int),N);
                break;
              }
              case SC_ThreadCreate:
                                    {

                                      DEBUG ('s', "ThreadCreate\n");
                                      do_ThreadCreate(machine->ReadRegister(4),machine->ReadRegister(5));
                                      break;
                                    }
                case SC_ThreadExit:
                              {
                                 DEBUG ('s', "ThreadExit \n");

                                 do_ThreadExit();

                                 break;
                              }

  #endif //CHANGED

		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!type) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }

	default:
	  printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
	  ASSERT (FALSE);
      }
}
