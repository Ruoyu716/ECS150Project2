
# Project 2 Report
---
### Part 1 Queue fuctions(queue.c)
___
**nodes**

In order to get a queue that has a first in first out data structure,
we make a stuct "QNode"which contains its own data and address to 
next nodes,thus we can make queue as a linked nodes which has a head
and tail as it first and last nodes.

**create a queue**

In order to create a queue First, we need to allocate memory for it.
Then initial a queue contains only NULL head and tail.

**Destroy a queue**

Simply deallocate queue by free the memory.

**Enqueue and dequeue**

when enqueue we need to first allocate a new node that contains
new data, then link the node to queue we want to attach by 
eplacing the NULL tail fo the old queue, or otherwise, the next of 
old tail.

For dequeue, we need to take out the first nodes we put in the
queue, that is, the head. Then make the next of head to be the
new head.

___
### Part 2 thread fuctions(thread.c)
___
We make a thread control box as a struct contains tid, state,
context and it own stack.and queues for threads in each
states, i.e. zombies. 

**Initial mainThread**

If never a thread been create, we need to first initial a main
thread control box, and make this thread the running thread,
allocate the context and stack using uthread_ctx_init provided.

**create a thread**

Create a new thread control box, if already exist threads, make
the new thread state as READY.then attach this new created
thread into the ready list.

**thread yield**

created a new thread that is store the current running thread,
and enqueued the current thread to readylist of threads. 
Then, we dequeued the first thread in readylist of threads
and make it to be the current running thread.

**exit a thread**

Make the current thread a zombie until be colloected,and then 
move on to next thread.

**join thread**

if the current thread to be joined is "ZOMBIE",collect the retval.
wait until dead otherwise, that is , block the current running 
thread,make the joining thread new current running thread.

___

### Part 3 preempt

a timer which will using a SIGVTALRM signal to yields a hundred
times per second.
