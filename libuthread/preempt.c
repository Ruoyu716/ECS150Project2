#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct itimerval timer;
struct sigaction action;
sigset_t alarm;

void preempt_disable(void)
{
	/* TODO Phase 4 */
  sigprocmask(SIG_BLOCK, &alarm, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
  sigprocmask(SIG_UNBLOCK, &alarm, NULL);
}

static void handler(){
	uthread_yield();
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	//install handler
  sigemptyset(&action.sa_mask);
  sigaddset(&action.sa_mask, SIGVTALRM);
	action.sa_handler = handler;
	action.sa_flags=0;
	alarm=action.sa_mask;

	if(sigaction(SIGVTALRM,&action,NULL)<0){
		printf("sigaction error\n");
		exit(EXIT_FAILURE);
	}

  sigaction(SIGVTALRM,&action,NULL);

	timer.it_value.tv_sec=0;
	timer.it_value.tv_usec=10000;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 10000;

	if(setitimer (ITIMER_VIRTUAL, &timer, NULL)<0){
		printf("timer error\n");
		exit(EXIT_FAILURE);
	}

  setitimer (ITIMER_VIRTUAL, &timer, NULL);
}
