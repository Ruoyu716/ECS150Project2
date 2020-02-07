
# Project 2 Report
---
### Part 1 Queue API(queue.c)
___
Overall, we implement a simple FIFO queue based on linked list idea 
that allows user to add new items to the end of queue and remove
items from the head of queue(the least recent items in other word)

**Data Structure :**

**structure nodes**

Each node has void pointer that holds the data and a struct pointer
that holds the node following it.

**structure queue**

Each queue has a head node and a tail node which means the first and 
last items in a queue, and it also has a integer variable 'size' to
store the size of queue.

**Create a queue**

A empty queue is created here with two null pointers, head and tail
and its initial size of 0

**Destroy a queue**

We deallocate the queue and return 0, and if queue is NULL,return 
-1.

**Enqueue**

When doing enqueue, we add the new item to the end of the queue by
calling function queue_enqueue(). And it will return -1 for 
failure on memory allocating,NULL data and NULL queue. In this 
process, a new node is created to link to previous tail node and 
become the new tail node. Lastly, update the queue size.

**Dequeue**

When doing dequeue, we pull out the least recent node in the queue
by calling function queue_dequeue(). And it will return -1 for 
NULL data and empty queue. In this process, data of removed node
will be stored in another pointer, then free the previous head 
node and update queue size.

**Delete**

When doing delete, we search in the specified queue from its 
head node, when target is found, let its parent node point 
to its child node, and update queue size.

**Iteration**

When doing iteration, we use a while loop to go through the 
specified queue to invoke call back function on each node.
It wil return -1 for NULL queue and NULL fuction.

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
times per second. THe preemption should be invoked whenever a user
thread is running. We call preempt_start() to active the timer 
once we initialized our main thread. We also set the timer to 
be expired after 100 seconds
