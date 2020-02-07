#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

enum states{running,ready,block,zombie};

struct TCB{
  uthread_t tid;
  uthread_t preTid;//for thread who is waiting for another.
  uthread_ctx_t* ctxt;
  enum states state;
  void* stack;
  int retval;

  struct TCB* joinThread;
};

static int numOfThread;
static queue_t readyQ;
//static queue_t runningQ;
static queue_t blockQ;
static queue_t zombieQ;
static struct TCB *curThread;
/* TODO Phase 2 */

void initialize_mainThread(uthread_func_t func, void *arg){
  struct TCB *mainThread = (struct TCB*)malloc(sizeof(struct TCB));

  mainThread->tid = numOfThread;
  mainThread->ctxt=malloc(sizeof(uthread_ctx_t));

  mainThread->state = running;
  mainThread->stack = uthread_ctx_alloc_stack();//may have problem

  uthread_ctx_init(mainThread->ctxt,mainThread->stack,func,arg);

  mainThread->joinThread = NULL;//issue
  curThread = mainThread;
  numOfThread ++;
//correct place?

  preempt_disable();
  readyQ = queue_create();
  preempt_enable();

  preempt_disable();
  blockQ = queue_create();
  preempt_enable();

  preempt_disable();
  zombieQ = queue_create();
  preempt_enable();
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
  struct TCB *oldThread = (struct TCB*)malloc(sizeof(struct TCB));
  struct TCB *nextThread = (struct TCB*)malloc(sizeof(struct TCB));//no error message to handle allocate failure?

  oldThread = curThread;

  if(curThread->state!=block && curThread->state!=zombie){
    preempt_disable();
    queue_enqueue(readyQ,curThread);
    preempt_enable();
  }
  printf("de1\n");
  preempt_disable();
  printf("de2\n");
  queue_dequeue(readyQ, (void**)&nextThread);
  printf("de3\n");
  preempt_enable();
  printf("de4\n");
  // if (next != -1){
  curThread->state = ready;
  printf("de5\n");//stop from running.but do we really need this line?
  nextThread->state = running;
  printf("de6\n");
  curThread = nextThread;
  printf("de7\n");
  uthread_ctx_switch(oldThread->ctxt,nextThread->ctxt);
  printf("de8\n");
  //}
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
  return curThread->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  preempt_start();
  if(numOfThread == 0){//initialize main thread
    initialize_mainThread(func,arg);
  }

  struct TCB *newThread = (struct TCB*)malloc(sizeof(struct TCB));
  if(!newThread){
    return -1;
  }

  newThread->tid = numOfThread;
  newThread->ctxt = malloc(sizeof(uthread_ctx_t));
  if(!newThread->ctxt){
    return -1;
  }
  newThread->state = ready;
  newThread->stack = uthread_ctx_alloc_stack();
  if(!newThread->stack){
    return -1;
  }
  //?????
  uthread_ctx_init(newThread->ctxt,newThread->stack,func,arg);

  newThread->joinThread = NULL;
  numOfThread ++;

  preempt_disable();
  queue_enqueue(readyQ,newThread);
  preempt_enable();

  return newThread->tid;
}

static int findBlock(void* tcb, uthread_t* preTid){
	struct TCB* thread = (struct TCB*) tcb;

  if (thread->tid == *preTid){
    return 1;
	}
  return 0;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */

  curThread->state = zombie;
  curThread->retval = retval;
  preempt_disable();
  queue_enqueue(zombieQ,curThread);
  preempt_enable();
  struct TCB* tcb = malloc(sizeof(struct TCB));
  if(curThread->joinThread != NULL){
    preempt_disable();
    int isFound = queue_iterate(blockQ, (queue_func_t)findBlock, (void *)&curThread->preTid, (void**)&tcb);
    preempt_enable();

    if(isFound == 1){//found the blocked parent thread.
      curThread->joinThread->state = ready;
      preempt_disable();
      queue_delete(blockQ,curThread->joinThread);
      preempt_enable();

      preempt_disable();
      queue_enqueue(readyQ,curThread->joinThread);
      preempt_enable();
    }
  }else{
    struct TCB* tmp = malloc(sizeof(struct TCB));
    struct TCB* old = curThread;

    preempt_disable();
    int dq = queue_dequeue(readyQ,(void**)&tmp);
    preempt_enable();

    if( dq== 0){//pull out the next avaliable thread.
      curThread = tmp;
      curThread->state = running;
      uthread_ctx_switch(old->ctxt,curThread->ctxt);//switch to the next avaliable thread
    }

  }


}

static int findJoin(void* tcb, void* tid){
	struct TCB* thread = (struct TCB*) tcb;
  int x = (int)(unsigned long)tid;
  if (thread->tid == x){
    return 1;
	}
  return 0;
}

int uthread_join(uthread_t tid, int *retval)
{
  if(tid == 0 || tid == curThread->tid){
    return -1;
  }

  struct TCB* foundR = malloc (sizeof(struct TCB));
  struct TCB* foundZ = malloc (sizeof(struct TCB));

  preempt_disable();
  queue_iterate(readyQ, (queue_func_t)findJoin, (void*)(unsigned long)tid,(void**)foundR);
  preempt_enable();

  if(foundR == NULL){

    return -1;
  }else{

    if(foundR->joinThread != NULL){//has been joined.

      return -1;
    }

    curThread->state = block;
    foundR->state = running;
    foundR->joinThread = curThread;
    foundR->preTid = curThread->tid;


    preempt_disable();
    queue_enqueue(blockQ,curThread);
    preempt_enable();

    preempt_disable();
    queue_delete(readyQ,foundR);
    preempt_enable();

    printf("7\n");

    uthread_yield();
    printf("8\n");
  }
  printf("9\n");
  preempt_disable();
  int isFoundZ = queue_iterate(zombieQ, (queue_func_t)findJoin, (void*)&tid,(void**)foundZ);
  preempt_enable();

  if(isFoundZ != 0){
    if(retval != NULL){
			*retval=foundR->retval;
			queue_delete(zombieQ,foundZ);
			free(foundR);
      numOfThread --;
		}
	}else{// tid not found
		return -1;
	}
  return 0;
}
