#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

//use linked list to implement queue
struct QNode{
  void* data;
  struct QNode* next;
};

struct queue {
	/* TODO Phase 1 */
  struct QNode* head;
  struct QNode* tail;
  int size;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
  queue_t q;
  q = (queue_t)malloc(sizeof(struct queue));

  //do really need if/else statement to handle malloc failure?????
  if(!q){//malloc failed
    return NULL;
  }else{//empty queue
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
  }
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
  if(queue == NULL){
    return -1;
  }else{
    free(queue);//deallocate queue

    if(queue->size != 0){
      return -1;
    }else{
      return 0;
    }
  }

}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
  if(queue == NULL || data == NULL){
    return -1;
  }
  queue->tail = (struct QNode*)malloc(sizeof(struct QNode));

  if(!queue->tail){
    return -1;
  }
  if(queue->size == 0){
    queue->head = (struct QNode*)malloc(sizeof(struct QNode));
    if(!queue->head){
      return -1;
    }
    //currently data of tail is NULL
    queue->head->data = data;
    queue->head->next = NULL;
    //queue->tail=queue->head;
    //queue->tail->next = NULL;
    //queue->tail = NULL;//can set a node to null?

  }else{
    queue->tail->next = (struct QNode*)malloc(sizeof(struct QNode));
    if(queue->tail == NULL){
      queue->tail->data = data;
      queue->tail->next = NULL;
      queue->head->next = queue->tail;
    }else{
      queue->tail->next->data = data;
      queue->tail->next->next= NULL;
      queue->tail = queue->tail->next;
    }
  }
  queue->size += 1;
  return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
  if(queue == NULL || data == NULL || queue->size == 0){
    return -1;
  }
  printf("DQ1\n");
  struct QNode* tmpNode = malloc(sizeof(struct QNode));
  printf("DQ2\n");
  if(!tmpNode){
    printf("DQ3\n");
    return -1;
  }
  printf("DQ4\n");

  if(queue->size==1){
    printf("DQ5\n");
    *data = queue->head->data;
    printf("DQ6\n");
    queue->head = NULL;
    printf("DQ7\n");
    queue->size -= 1;
    printf("DQ8\n");

  }else{
    printf("DQ9\n");
    *data = queue->head->data;
    printf("DQ10\n");
    tmpNode = queue->head->next;
    free(queue->head);
    printf("DQ11\n");
    queue->head = tmpNode;
    printf("DQ12\n");
    queue->size -= 1;
    printf("DQ13\n");
  }
  printf("DQ14\n");
  return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
  if(queue == NULL || data == NULL || queue->size == 0){
    return -1;
  }

  if(queue->head->data == data){
    struct QNode* tmpNode = malloc(sizeof(struct QNode));
    if(!tmpNode){
      return -1;
    }

    if(queue->size==1){
      queue->head = NULL;
      queue->size -= 1;
    }else{
      tmpNode = queue->head->next;
      queue->head = tmpNode;
      queue->size -= 1;
    }
    return 0;//found at head
  }else{//if head is not the one to delete,search in the queue
    struct QNode* current = malloc(sizeof(struct QNode));
    struct QNode* next = malloc(sizeof(struct QNode));

    current = queue->head;
    next = queue->head->next;

    while(next!=NULL){
      if(next->data == data){
        current->next = next->next;//do we need to free?????
        queue->size -= 1;
        return 0;
      }
      current = next;
      next = next->next;//update current node and next node for new iteration
    }
  }
  return -1;//not found in queue
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
  if(queue == NULL || func == NULL){
    return -1;
  }

  struct QNode* tmpNode = malloc(sizeof(struct QNode));
  tmpNode = queue->head;

  while(tmpNode!=NULL){//what if there is no Node let func return 1?
    if(func(tmpNode->data,arg) == 1){
      *data = tmpNode->data;
      return 0;
    }else{
      tmpNode = tmpNode->next;
    }
  }
  return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
  if(queue == NULL){
    return -1;
  }

  return queue->size;
}
