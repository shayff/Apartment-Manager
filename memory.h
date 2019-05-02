#include "main.h"

long int fileSize(FILE * f);
int rowlen(FILE *f);

/*loading functions*/
void load(char ** stockA, List * stockB, AptList *FullAprts, int * index);
void loadAprts(AptList * FullAprts);
void loadStocks(char ** stockA, List * stockB, int * index);
void loadStocksRec(FILE * f, long int filesize, char ** stockA, List * stockB, int * index);

/*saving functions*/
void save(char ** stockA, List * stockB, AptList *FullAprts, int * index);
void saveAprts(AptList * FullAprts);
void saveStocks(char ** stockA, List * stockB, int * index);
void saveStocksRec(ListNode * node, FILE * f);

/*Bit conventors*/
void AprtToBits(char * mem, Aprtment Aprt);
Aprtment BitsToAprt(char * mem);