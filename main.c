/*This program made by Shay pepper And Nely nehmia*/

#include "main.h"
#include "commands.h"
#include "memory.h"
#include "stocks.h"


void main()
	{
	/*Stocks*/
	int index = 1; //stocks index
	char * short_term_history[N];
	List stockB;
	emptyStocks(&short_term_history, &stockB);
	
	/*Aprt list*/
	AptList FullAprts;
	emptyAprts(&FullAprts);
	
	/*Load from files*/
	load(&short_term_history, &stockB, &FullAprts, &index);

	/*Print intro*/
	printf("Please enter one of the following commands:\n");
	printf("add-an-apt, get-an-apt, buy-an-apt or delete-an-apt\n");
	printf("For reconstruction commands, please enter:\n");
	printf("!!, !num, history, short_history or !num^str1^str2\n");
	printf("To exit, enter exit.\n");
	
	/*get input*/
	while (userInput(getCommands(), &FullAprts, &short_term_history,&stockB,&index) == TRUE)
     	{ 	}

	/*Save memory and free allocation*/
	save(&short_term_history, &stockB, &FullAprts, &index);
	freeApp(&short_term_history,&stockB, &FullAprts);
}

/*Main functions*/
BOOL userInput(char * command, AptList *FullAprts, char ** stockA,List*stockB,int * index )
/*This function get a user cmd and call the function that needed, its also save the cmd in the stocks*/
{
	BOOL isHis = TRUE;
	char cmd = command[0];
	switch (cmd) {
	case 'a': //add-an-apt
		addApt(command + 11, FullAprts);
		break;
	case 'b': //but-an-apt
		buyApt(command + 11, FullAprts);
		break;
	case 'd': //delete-an-apt
		deleteApt(command + 21, FullAprts);
		break;
	case'g': //get-an-apt
		getApt(command + 11, FullAprts);
		break;
	case'!':
		userInput(exclamation(command+1, stockA, stockB), FullAprts, stockA, stockB, index);
		isHis = FALSE;
		break;
	case's': //short_history
		printStockA(stockA,index);
		isHis = FALSE;
		break;
	case'h': //history
		printstockB(stockB);
		printStockA(stockA, index);
		isHis = FALSE;
		break;
	case'e': //exit
		printf("Good Bye!\n");
		free(command);
		return FALSE;
	}

	//save to history
	if (isHis == TRUE)
		addStockA(command, stockA, stockB, index);
	else
		free(command);
	return TRUE;
}
char * getCommands()
/*This function get a command from user and put it into a char ptr */
{
	int physSize = 2, logicSize = 0;
	char ch;
	char * cmd = (char *)malloc(sizeof(char) * physSize);
	printf(">> ");
	ch = getchar();
		while (ch != '\n')
		{
			if (physSize == logicSize)
			//realloc
			{
				physSize *= 2;
				cmd = (char *)realloc(cmd, physSize * sizeof(char));
			}
			cmd[logicSize++] = ch;
			ch = getchar();
		}
		cmd = (char *)realloc(cmd, (logicSize+1) * sizeof(char));
		cmd[logicSize] = '\0';
		return cmd;
}

/*Empty functions*/
void emptyStocks(char ** stockA, List * stockB)
/*This function empty the stocks*/
{
	int i;
	//empty stockA
	for (i = 0; i < 7; i++)
		stockA[i] = NULL;

	//empty stockB
	emptyList(stockB);
}
void emptyAprts(AptList * FullAprts)
/*This function empty the Aprts list*/
{
	FullAprts->head = FullAprts->tail = NULL;
	FullAprts->TotalIndex = 0;
}

/*Free memory funtions*/
void freeApp(char ** stockA,List * stockB, AptList * FullAprts)
/*This function free stockB and FullAprts*/
{	
	ListNode* tmpNode = stockB->head;
	ListNode* oldNode;
	int i=0;
	while (stockA[i] != NULL && i < 7)
		free(stockA[i++]);

	freeAprtList(FullAprts);

	//free stockB list
	while (tmpNode != NULL)
	{
		oldNode = tmpNode;
		free(tmpNode->commands);
		tmpNode = tmpNode->next;
		free(oldNode);
	}	
}