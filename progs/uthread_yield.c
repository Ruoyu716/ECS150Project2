/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void* arg)
{
	printf("3-1\n");
	uthread_yield();
	printf("3-2\n");
	printf("thread%d\n", uthread_self());
	printf("3-3\n");
	return 0;
}

int thread2(void* arg)
{
	printf("2-1\n");
	uthread_create(thread3, NULL);
	printf("2-2\n");
	uthread_yield();
	printf("2-3\n");
	printf("thread%d\n", uthread_self());
	printf("2-4\n");
	return 0;
}

int thread1(void* arg)
{
	printf("1-1\n");
	uthread_create(thread2, NULL);
	printf("1-2\n");
	uthread_yield();
	printf("1-3\n");
	printf("thread%d\n", uthread_self());
	printf("1-4\n");
	uthread_yield();
	printf("1-5\n");
	return 0;
}

int main(void)
{
	uthread_join(uthread_create(thread1, NULL), NULL);
	return 0;
}
