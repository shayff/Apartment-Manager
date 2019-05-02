#include "main.h"
#include <string.h>

/*Commands functions*/
void addApt(char * command, AptList * FullAprts);
void buyApt(char * command, AptList * FullAprts);
void deleteApt(char * command, AptList * FullAprts);
void getApt(char * command, AptList * FullAprts);
char * exclamation(char * command, char ** stockA, List*stockB);

/*exclamation sub functions*/
char* getcmd(int numcmd, char ** stockA, List*stockB);
char * reStr(char * oldcmd, char* str1, char * str2);

/*Get-Apt sub functions*/
AptList AprtrCondions(int minrooms, int maxrooms, int maxprice, Date enterdate,
	 AptList * FullAprts);
BOOL checkCondions(int minrooms, int maxrooms, int maxprice, Date sortdate,
	Aprtment Aprt);
void printAprtments(AptList * aprts);
void PrintCodesByDate(int days, AptList * FullAprts);

/*Merge functions*/
void MergeSort(AptNode** headOfSelectedList, int sortWay);
AptNode* SortedMerge(AptNode* first, AptNode* second, int sortWay);
void FrontBackSplit(AptNode* source, AptNode** first, AptNode** second);

/*List & nodes functions*/
void CreateAptNode(AptList * FullAprts, Aprtment aprt);
void swapnode(AptNode * a, AptNode * b, AptList * aprts);
void freeAptNode(AptNode* aNode);
void freeAprtList(AptList* Aprts);

/*Calculate helper functions*/
int DateToNum(Date date);
int CharToNum(char* str);
Date CharToDate(char * str);
Date TodayDate();