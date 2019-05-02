#include "commands.h"
#include <time.h>
#include <string.h>

/*Commands functions*/
void addApt(char * command, AptList * FullAprts)
/*This function add a new aprtment to FullAprts*/
{
	Aprtment aprt = { 0 };
	aprt.price = 0;
	int i = 1, j, addressLength = 1;

	/*Aprtments codes*/
	aprt.code = (FullAprts->TotalIndex) + 1;
	(FullAprts->TotalIndex)++;
	
	/*Checks the address length of the apartment*/
	while (command[i] != '"')
	{
		addressLength++;
		i++;
	}
	aprt.adress = (char*)malloc(sizeof(char)*addressLength);
	/*Enter the address */
	for (j = 0; j < addressLength - 1; j++)
		aprt.adress[j] = command[j + 1];
	aprt.adress[j] = '\0';

	 /*Price*/
	sscanf(command + addressLength + 2, "%d %hi %d %d %d", &aprt.price, &aprt.rooms, &aprt.enter_date.day, &aprt.enter_date.month, &aprt.enter_date.year);
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	aprt.added_date.day = tm.tm_mday;
	aprt.added_date.month = tm.tm_mon + 1;
	aprt.added_date.year = tm.tm_year-100;

	CreateAptNode(FullAprts, aprt);
}
void buyApt(char * command, AptList * FullAprts)
/*This functiton take care of buying and aparment*/
{
	int num = CharToNum(command);
	AptNode* aNode = FullAprts->head;
	AptNode* save = NULL;

	/* Search the AptNode with the appropriate code */
		while (aNode->aprt.code != num)
		{
			save = aNode;
			aNode = aNode->next;
		}

	/* Check if it is the last AptNode */
		if (FullAprts->tail == aNode)
		{
			save->next = NULL;
			FullAprts->tail = save;
			(FullAprts->TotalIndex)--;
		}
	/* Check if it is the first AptNode */
	else if (FullAprts->head == aNode)
		FullAprts->head = aNode->next;

	/* Check if AptNode is in the middle */
	else
		save->next = aNode->next;

	/* Release memory allocations */
		freeAptNode(aNode);
}
void deleteApt(char * command, AptList * FullAprts)
/*This function delete apartments from the day given*/
{
	int days=CharToNum(command);


	AptNode* aprtCurrent = FullAprts->head;
	AptNode* save = aprtCurrent;

	Date theday;
	time_t timeCurrent = time(NULL);
	time_t t = timeCurrent - (days * 24 * 60 * 60);
	struct tm tm = *localtime(&t);
	theday.day = tm.tm_mday;
	theday.month = tm.tm_mon + 1;
	theday.year = tm.tm_year - 100;

	while (aprtCurrent != NULL)
	{
		
		if (DateToNum(theday) <= DateToNum(aprtCurrent->aprt.added_date))
			{
			/*Check if it is the first AptNode*/
			if (FullAprts->head == aprtCurrent)
			{
				FullAprts->head = aprtCurrent->next;
				freeAptNode(aprtCurrent);
				aprtCurrent = FullAprts->head;
			}
			/*Check if it is the last AptNode*/
			else if (FullAprts->tail == aprtCurrent)
			{
				save->next = NULL;
				FullAprts->tail = save;
				freeAptNode(aprtCurrent);
				aprtCurrent = save;
			}

			/*Check if AptNode is in the middle*/
			else
			{
				save->next = aprtCurrent->next;
				freeAptNode(aprtCurrent);
				aprtCurrent = save;
			}
		}
		save = aprtCurrent;
		aprtCurrent = aprtCurrent->next;
	}
}
void getApt(char * command, AptList * FullAprts)
/*This function get a get-apt command and print the apartments that fit
to the given condions*/
{
	int MaxPrice = -1, MinRooms = -1, MaxRooms = -1, SortType = -1;
	Date Sdate = { 0,0,0 };

	//copy the command
	char * cpycmd = (char *)malloc(sizeof(char)*(1 + strlen(command)));
	cpycmd = strcpy(cpycmd, command);

	char * token = strtok(cpycmd, " ");
	if (strcmp("-Enter", token) == 0)
	{
		PrintCodesByDate(CharToNum(strtok(NULL, " ")), FullAprts);
		free(cpycmd);
		return;
	}

	while (token != NULL)
	//this while check what condition the user ask for
	{
		if (strcmp("-MaximumPrice", token) == 0)
			MaxPrice = CharToNum(strtok(NULL, " "));
		else if (strcmp("-MinimumNumRooms", token) == 0)
			MinRooms = CharToNum(strtok(NULL, " "));
		else if (strcmp("-MaximumNumRooms", token) == 0)
			MaxRooms = CharToNum(strtok(NULL, " "));
		else if (strcmp("-s", token) == 0)
			SortType = LOWTOHIGH;
		else if (strcmp("-sr", token) == 0)
			SortType = HIGHTOLOW;
		else if (strcmp("-Date", token) == 0)
			Sdate = CharToDate(strtok(NULL, " "));
		token=strtok(NULL, " ");
	}

	//check every aprtment with the condion given
	AptList newaprts = AprtrCondions(MinRooms, MaxRooms, MaxPrice, Sdate, FullAprts);

	//Check what kind of sort needed

	if (SortType == HIGHTOLOW)
		MergeSort(&newaprts.head, HIGHTOLOW);
	else if (SortType == LOWTOHIGH)
		MergeSort(&newaprts.head, LOWTOHIGH);

	
	printAprtments(&newaprts);

	//free allocation
	free(cpycmd);
	freeAprtList(&newaprts);

}
char * exclamation(char * command, char ** stockA, List*stockB)
/**/
{
	int cmdnum = 0,i = 6;
	char * cpycmd;
	if (command[0] == '!') //last one
	{ 
		while (stockA[i] == NULL)
			i--;
		cpycmd = (char*)malloc(sizeof(char)*(1 + strlen(stockA[i])));
		strcpy(cpycmd, stockA[i]);
		cpycmd[strlen(stockA[i])] = '\0';
		return cpycmd;
	}
	char * token = strtok(command, "^");
	char * oldcmd = getcmd(CharToNum(token), stockA, stockB);
	token = strtok(NULL, "^");
	if (token == NULL)
	{
		cpycmd = (char*)malloc(sizeof(char)*(1 + strlen(oldcmd)));
		strcpy(cpycmd, oldcmd);
		cpycmd[strlen(oldcmd)] = '\0';
		return cpycmd;
	}
		
	else
		return reStr(oldcmd, token, strtok(NULL, "^"));
}

/*exclamation sub functions*/
char* getcmd(int numcmd, char ** stockA, List*stockB)
/*This function get a cmd num and find it in stocks*/
{
	if (numcmd > stockB->TotalIndex)
		return stockA[numcmd- stockB->TotalIndex-1];
	else
	{
		ListNode * temp = stockB->head;
		while (temp->index != numcmd)
			temp = temp->next;
		return temp->commands;
	}
}
char * reStr(char * oldcmd, char* str1, char * str2)
/*This function get a cmd and 2 sub str, and replace the first sub and the second one in
every place that appear in cmd*/
{
	char * temp;
	int resize = strlen(str2) - strlen(str1);
	int oldlen = strlen(oldcmd);
	int str1len = strlen(str1);
	int index = 0, oldindex = 0;
	char * newcmd = (char*)malloc(sizeof(char)*oldlen);
	temp = strstr(oldcmd, str1);

	while (temp != NULL)
	{
		newcmd = (char*)realloc(newcmd, sizeof(char)*oldlen+resize+1);
		resize *= 2;
		memcpy(newcmd+index, oldcmd+oldindex,temp- (oldcmd + oldindex));
		index += temp - (oldcmd+oldindex);
		newcmd[index] = '\0';
		strcat(newcmd, str2);
		index += strlen(str2);
		oldindex = temp - oldcmd+str1len ;
		temp = strstr(temp + str1len, str1);
	}
	strcat(newcmd, oldcmd + oldindex);
	return newcmd;
}

/*Get-Apt sub functions*/
AptList AprtrCondions(int minrooms, int maxrooms, int maxprice, Date enterdate,
	 AptList * FullAprts)
/*This function get an aprtlist and condions and return a new list with aprts that 
fit the condition*/
{
	AptList CondiAprts;
	CondiAprts.head = CondiAprts.tail = NULL;
	AptNode * temp = FullAprts->head;
	if (temp == NULL)
		return CondiAprts;

	while (temp != NULL)
	{
		if (checkCondions(minrooms, maxrooms, maxprice, enterdate, temp->aprt) == TRUE)
			CreateAptNode(&CondiAprts, temp->aprt);
		temp = temp->next;
	}
	return CondiAprts;
}
BOOL checkCondions(int minrooms, int maxrooms, int maxprice, Date sortdate,
	Aprtment Aprt)
	/*This function get an aprtment and check if in fit with the condions given*/
{
	int SortInNum = DateToNum(sortdate);
	int EnterInNum = DateToNum(Aprt.enter_date);
	
	if (Aprt.rooms < minrooms)
		return FALSE;
	if (maxrooms != -1 && Aprt.rooms > maxrooms)
		return FALSE;
	if (SortInNum != 0 && EnterInNum > SortInNum)
		return FALSE;
	if (maxprice != -1 && Aprt.price > maxprice)
		return FALSE;
	return TRUE;
}
void printAprtments(AptList * aprts)
/*This function get an List of aprtments and print if */
{
	AptNode* aprtCurrent = aprts->head;
	while (aprtCurrent != NULL)
	{
		printf("Apt details: \n");
		printf("Code: %d \n", aprtCurrent->aprt.code);
		printf("Address: %s \n", aprtCurrent->aprt.adress);
		printf("Number of rooms: %d	\n", aprtCurrent->aprt.rooms);
		printf("Price: %d \n", aprtCurrent->aprt.price);
		printf("Entry date: %u.%u.%u \n", aprtCurrent->aprt.enter_date.day, aprtCurrent->aprt.enter_date.month, aprtCurrent->aprt.enter_date.year+2000);
		printf("Database entry date: %u.%u.%u \n", aprtCurrent->aprt.added_date.day, aprtCurrent->aprt.added_date.month, aprtCurrent->aprt.added_date.year + 2000);
		aprtCurrent = aprtCurrent->next;
	}
}
void PrintCodesByDate(int days, AptList * FullAprts)
/*This function print aprtments by date*/
{
	Date today = TodayDate();
	int numtoday = DateToNum(today);
	AptNode* aprtCurrent = FullAprts->head;

	while (aprtCurrent != NULL)
	{
		if (numtoday - DateToNum(aprtCurrent->aprt.added_date) <= days)
			printf("Code: %d \n", aprtCurrent->aprt.code);
		aprtCurrent = aprtCurrent->next;
	}
}

/*Sort functions*/
void MergeSort(AptNode** headOfSelectedList, int sortWay)
/*This function sort a list using merge sort method*/
{
	AptNode* head = *headOfSelectedList;
	AptNode* first;
	AptNode* second;

	if ((head == NULL) || (head->next == NULL))
	{
		return;
	}

	// split head into 'first' and 'second' sublists
	FrontBackSplit(head, &first, &second);

	//recursively sort the sublists
	MergeSort(&first, sortWay);
	MergeSort(&second, sortWay);

	//*headOfList = merge the two sorted lists together */
	*headOfSelectedList = SortedMerge(first, second, sortWay);
}
AptNode* SortedMerge(AptNode* first, AptNode* second, int sortWay)
/*This function merge 2 sorted list*/
{
	AptNode* result = NULL;

	/* Base cases */
	if (first == NULL)
		return(second);
	else if (second == NULL)
		return(first);

	if(sortWay==LOWTOHIGH)
	{
		if (first->aprt.price < second->aprt.price)
		{
			result = first;
			result->next = SortedMerge(first->next, second, sortWay);
		}
		else
		{
			result = second;
			result->next = SortedMerge(first, second->next, sortWay);
		}
	}
	else {
		if (first->aprt.price < second->aprt.price) {
			result = second;
			result->next = SortedMerge(first, second->next, sortWay);
		}
		else
		{
			result = first;
			result->next = SortedMerge(first->next, second, sortWay);
		}
	}
	return(result);
}
void FrontBackSplit(AptNode* source, AptNode** first, AptNode** second)
/*This function split 2 linked list, Uses the fast/slow pointer strategy.*/
{
	AptNode* fast;
	AptNode* slow;
	slow = source;
	fast = source->next;

	// advance 'fast' two nodes, and advance 'slow' one node
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*first = source;
	*second = slow->next;
	slow->next = NULL;
}

/*List functions*/
void swapnode(AptNode * a, AptNode * b, AptList * aprts)
/*This function swap 2 node and update the list*/
{
	Aprtment  temp = a->aprt;
	a->aprt = b->aprt;
	b->aprt = temp;
	return;
}
void CreateAptNode(AptList * FullAprts, Aprtment aprt)
/*This function create a new AptNode and update the List*/
{
	AptNode * temp = (AptNode *)malloc(sizeof(AptNode));
	temp->aprt = aprt;
	temp->aprt.adress = (char*)calloc(strlen(aprt.adress) + 1, sizeof(char));
	strcpy(temp->aprt.adress, aprt.adress);
	temp->next = NULL;

	if (FullAprts->head == NULL)
	{
		FullAprts->head = FullAprts->tail = temp;
	}
	else
	{
		FullAprts->tail->next = temp;
		FullAprts->tail = temp;
	}
}
void freeAptNode(AptNode* aNode)
/*This function free an AptNode*/
{
	free(aNode->aprt.adress);
	free(aNode);
}
void freeAprtList(AptList* Aprts)
/*This function free an apt list*/
{
	AptNode* tmpApt = Aprts->head;
	AptNode * oldApt;
	while (tmpApt != NULL)
	{
		free(tmpApt->aprt.adress);
		oldApt = tmpApt;
		tmpApt = tmpApt->next;
		free(oldApt);
	}
}

/*Calculate helper functions*/
int DateToNum(Date date)
/*This function get a date and turn it into a number*/
{
	return (date.year * 10000 + date.month * 100 + date.day);
}
int CharToNum(char* str)
/*A function that receive a number in the form of a string and returns integer */
{
	int i = 0, num = 0;
	while(str[i]!='\0')
		num = num * 10 + str[i++] - '0';
	return num;
}
Date CharToDate(char * str)
/*This function get a str and save it in date format*/
{
	Date temp;
	temp.day = 10 * (str[0] - '0') + str[1]-'0';
	temp.month = 10 * (str[2] - '0') + str[3]-'0' ;
	//temp.year = 1000 * (str[4] - '0') + 100 * (str[5] - '0') + 10 * (str[6] - '0') +  (str[7] - '0')  ;
	temp.year = 10*(str[6] - '0') + (str[7] - '0');
	return temp;
}
Date TodayDate()
/*This function return a date with today date*/
{
	Date today;
	time_t timeCurrent = time(NULL);
	struct tm tm = *localtime(&timeCurrent);
	today.day = tm.tm_mday;
	today.month = tm.tm_mon + 1;
	today.year = tm.tm_year - 100;
	return today;
}