#include "main.h"

/*Stocks functions*/

/*Add to stock*/
void addStockA(char * command, char ** stockA, List * stockB, int * index);
void addStockB(char* command, List * stockB, int * index);

/*Print stocks*/
void printStockA(char ** short_term_history, int * index);
void printstockB(List * stockB);

/*List functions*/
void createNode(char * command, int index, List * lst);
void emptyList(List * stockB);
void pullStockA(char ** stock);