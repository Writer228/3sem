#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>

#define MAX_QUEUE_SIZE 7
#define TREE_PEAKS 12

struct Queue
{
    int head;
    int tail;
    int data[MAX_QUEUE_SIZE];
};

void Push(struct Queue* queue, int element); // добавляет новый элемент в хвост очереди
void Pop(struct Queue* queue); // удаляет элемент из головы очереди
int Front(struct Queue* queue); // возвращает элемент из головы очереди
int Back(struct Queue* queue); // -//- хвоста очереди
int isEmpty(struct Queue* queue);

FILE *tree_in;

int main()
{
	tree_in = fopen("tree.txt", "r");
	
	int tree[TREE_PEAKS][TREE_PEAKS];
	int i = 0, j = 0;
	struct Queue queue;
	queue.head = 1;
	queue.tail = 0;
	
	while (fscanf(tree_in, "%d", &tree[i][j]) != EOF)
	{
		//printf("%d   ", tree[i][j]);
		j++;
		if (j == TREE_PEAKS)
		{
			j = 0;
			i++;
		}
		
	}
	i=0;
	j=0;
	
  /*
   * Можно было бы написать более элегантно и лаконично написав так:
   * while (!isEmpty(&queue))
   * {
   *    ....
   * }
   }
   */
	Push(&queue, 0);
	while (1)
	{
		if (isEmpty(&queue) == 1)
			break;
		if (tree[i][j] == 0)
		j++;
		else 
		{
			Push(&queue, j);
			j++;
		}
		if (j >= TREE_PEAKS)
		{
			printf("%d ", Front(&queue)+1);
			Pop(&queue);
			if (isEmpty(&queue) == 1)
				break;
			i = Front(&queue);
			j = 0;
		}
	}
	return 0;
}

int Back(struct Queue* queue)
{
	if(isEmpty(queue) == 1) 
	{
		printf("queue is empry\n");
		return 0;
	}
	return queue->data[queue->tail];	
}

int Front(struct Queue* queue)
{
	if(isEmpty(queue) == 1) 
	{
		printf("queue is empry\n");
		return 0;
	}
	return queue->data[queue->head];
}

void Pop(struct Queue* queue)
{
	int h;
	if(isEmpty(queue) == 1) 
	{
		printf("queue is empty!\n");
	}
	for(h = queue->head; h < queue->tail; h++) 
	{
		queue->data[h] = queue->data[h+1];
	}
	queue->tail--;
}

void Push(struct Queue* queue, int element)
{
	if (queue->tail < MAX_QUEUE_SIZE -1)
	{
		queue->tail ++;
		queue->data[queue->tail] = element;
	}
	else
		printf("queue is full");
}

int isEmpty(struct Queue* queue)
{
	if (queue->tail < queue->head)
		return 1;
	else
		return 0;
}
