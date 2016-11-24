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

#define MAX_STACK_SIZE 7
#define TREE_PEAKS 12

struct Stack
{
   int size;
   int data[MAX_STACK_SIZE];
};

int Top(struct Stack* stack);// возвращает элемент с верщины стека
void Pop(struct Stack* stack);// удаляет элемент с верщины стека
void Push(struct Stack* stack, int element);// добавляет новый элемент в стек
int isEmpty(struct Stack* stack);// возвращает 1, если стек пуст и 0 - в противном случае
void DFS(struct Stack* stack, int* tree);

FILE *tree_in;

int main()
{
	tree_in = fopen("tree.txt", "r");
	
	int tree[TREE_PEAKS][TREE_PEAKS];
	int i = 0, j = 0;
	struct Stack stack;
	stack.size = 0;
	
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
	
	Push(&stack, 0);
	while (1)
	{
		if (tree[i][j] == 0)
		j++;
		else 
		{
			Push(&stack, j);
			//printf("%d ", Top(&stack));
			i = Top(&stack);
			j = 0;
		}
		if (j >= TREE_PEAKS)
		{
			j = Top(&stack)+1;
			printf("%d ", i+1);
			Pop(&stack);
			i = Top(&stack);
		}
		if (isEmpty(&stack) == 1)
			break;
	}
	return 0;
}

int Top(struct Stack* stack)
{
	return stack->data[stack->size - 1];
}

void Pop(struct Stack* stack)
{
	if (stack->size > 0)
		stack->size--;
	else
		printf("stack is empty");
}

void Push(struct Stack* stack, int element)
{
	if (stack->size < MAX_STACK_SIZE -1)
		stack->data[stack->size] = element;	
	else 
		printf("stack is full");
	stack->size++;
}

int isEmpty(struct Stack* stack)
{
	if (stack->size == 0)
		return 1;
	else
		return 0;
}

void DFS(struct Stack* stack, int* tree)
{
	int i = 0, j = 0;
	if (tree[i][j] == 0)
		j++;
		else 
		{
			Push(stack, j);
			//printf("%d ", Top(&stack));
			i = Top(stack);
			j = 0;
		}
		if (j >= TREE_PEAKS)
		{
			j = Top(stack)+1;
			printf("%d ", i+1);
			Pop(stack);
			i = Top(stack);
		}
}
