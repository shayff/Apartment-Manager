#include "stocks.h"

/*Add to stock*/
void addStockA(char * command, char ** stockA, List * stockB, int * index)
/*This function add the cmd to stockA, if stockA is full we push cmd to stockB*/
{
	int i = 0;
	if (stockA[N - 1] == NULL)
		//stock A not full
	{
		while (stockA[i] != NULL)
			i++;
		stockA[i] = command;
	}
	else
		//Stock A full
	{
		addStockB(stockA[0], stockB, index);
		pullStockA(stockA);
		stockA[N - 1] = command;
	}
	(*index)++;
}
void addStockB(char* command, List * stockB, int * index)

/*This function add a command to END of stock B*/
{
	stockB->TotalIndex++;
	ListNode* lNode = (ListNode *)malloc(sizeof(ListNode));
	lNode->commands = command;
	lNode->index = *index - N;
	lNode->next = NULL;

	if (stockB->head == NULL)
		stockB->head = stockB->tail = lNode;
	else
	{
		stockB->tail->next = lNode;
		stockB->tail = lNode;
	}

}

/*Print stocks*/
void printStockA(char ** short_term_history, int * index)
/*This function print stockA*/
{
	int i;
	//what index we want to print
	int num =*index;
	if (num < N)
		num = 0;
	else
		num = *index - 7;

	for (i = 0; i < N; i++)
		printf("%d: %s\n", num+i,short_term_history[i]);
}
void printstockB(List * stockB)
/*This function print stockB*/
{
	ListNode* lnode = stockB->head;
	while (lnode)
	{
		printf("%d: %s\n", lnode->index, lnode->commands);
		lnode = lnode->next;
	}
}

/*List functions*/
void createNode(char * command, int index, List * lst)
/*This function create list node from stock b*/
{
	ListNode * temp = (ListNode *)malloc(sizeof(ListNode));
	temp->commands = command;
	temp->next = NULL;
		if (lst->head == NULL)
		{
			lst->head = lst->tail = temp;
		}
		else
		{
			lst->tail->next = temp;
			lst->tail = temp;
		}
}
void emptyList(List * stockB)
/*This function empty a list*/
{
	stockB->head = stockB->tail = NULL;
	stockB->TotalIndex = 0;
}
void pullStockA(char ** stock)
/*This function pull stockA one place*/
{
	int i;
	for (i = 0; i < N - 1; i++)
		stock[i] = stock[i + 1];
	return;
}