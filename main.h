#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define BOOL int
#define HIGHTOLOW 1
#define LOWTOHIGH 2
#define N 7

#pragma once

/*Structs*/

typedef struct date {
	short int day;
	short int month;
	short int year;
}Date;
/*Aprt Structs*/
typedef struct apartment {
	short int code;
	char * adress;
	int price;
	short int rooms;
	struct date enter_date;
	struct date added_date;
}Aprtment;
typedef struct aptnode {
	struct apartment aprt;
	struct aptnode*  next;
}AptNode;
typedef struct aptlist
{
	AptNode* head;
	AptNode* tail;
	int TotalIndex;
}AptList;

/*Stock Structs*/
typedef struct listnode {
	char*  commands;
	int index;
	struct listnode*  next;
}ListNode;
typedef struct list
{
	ListNode* head;
	ListNode* tail;
	int TotalIndex;
}List;

//main functions
BOOL userInput(char * command, AptList *FullAprts, char ** stockA, List*stockB, int * index);
char * getCommands();

/*Empty functions*/
void emptyStocks(char ** stockA, List * stockB);
void emptyAprts(AptList * FullAprts);

/*Free functions*/
void freeApp(char ** stockA, List * stockB, AptList * FullAprts);