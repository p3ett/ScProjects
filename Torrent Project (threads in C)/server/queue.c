#include "MirrorServer.h"

Queue* queue_init()
{   
	Queue *Q;
    Q = (Queue *) malloc(sizeof(Queue*));
    
    queue_size = 0;
    Q->front = 0;
    Q->rear = -1;
    Q->elements = (char**)malloc(sizeof(char*)*QUEUE_SIZE);
    
    return Q;
}

void Enqueue(Queue *Q, char *element)
{
    /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
    if( queue_size == QUEUE_SIZE )
    {
    	printf("Queue is Full\n");
    }
    else
    {	
        queue_size++;
        Q->rear++;
 
        /* As we fill the queue in circular fashion */
        if( Q->rear == QUEUE_SIZE )
        	Q->rear = 0;
 
        /* Insert the element in its rear side */
        Q->elements[Q->rear] = (char*) malloc(sizeof(char)*strlen(element)+1);
       	strcpy(Q->elements[Q->rear], element);
    }
}

void Dequeue(Queue *Q)
{
	if( queue_size != 0 )
	{	
		free(Q->elements[Q->front]);
	
		queue_size--;
		
        Q->front++;
              
        /* As we fill elements in circular fashion */
        if( Q->front == QUEUE_SIZE )
        	Q->front = 0;
   	}
}

/* Return the element which is at the front*/
char* front(Queue *Q)
{
        if( queue_size != 0 )
        	return Q->elements[Q->front];
        else
        	return NULL;
}
