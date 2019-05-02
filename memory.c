#include "memory.h"

/*Helper functions*/
long int fileSize(FILE * f)
/*This function calculate the file size*/
{
	long int res, temp;
	temp = ftell(f);
	fseek(f, 0, SEEK_END);
	res = ftell(f);
	fseek(f, temp, SEEK_SET);
	return res;
}
int rowlen(FILE *f)
/*This function get a size of a row from file*/
{
	char ch;
	int count = 0;
	ch = fgetc(f);
	while (ch != '\n')
	{
		count++;
		ch = fgetc(f);
	}
	fseek(f, -1 * (count + 2), SEEK_CUR);
	return count;
}

/*Saving*/
void save(char ** stockA, List * stockB, AptList *FullAprts, int * index)
/*This function save stocks and aprts to bin files*/
{
	saveAprts(FullAprts);
	saveStocks(stockA, stockB, index);

	return;
}
void saveAprts(AptList * FullAprts)
/*This function save the FullAprts to bin file*/
{
	FILE* f = fopen("apartments.bin", "wb");
	AptNode * temp = FullAprts->head;
	unsigned char mem[5];
	short int adrslen;

	while (temp != NULL)
	{
		adrslen = strlen(temp->aprt.adress);
		AprtToBits(mem, temp->aprt); //bits that need to be save
		fwrite(mem, sizeof(char), 5, f);
		fwrite(&temp->aprt.code, sizeof(short int), 1, f);
		fwrite(&adrslen, sizeof(short int), 1, f);
		fwrite(temp->aprt.adress, sizeof(char), adrslen, f);
		fwrite(&temp->aprt.price, sizeof(int), 1, f);
		temp = temp->next;
	}

	fclose(f);
}
void saveStocks(char ** stockA, List * stockB, int * index)
/*This function save the stockA and stockB to bin file*/
{
	int i, Aind = 0, cmdlen = 0;
	FILE* f = fopen("commands.txt", "w");
	*index = *index - 1;
	ListNode * temp = stockB->head;

	//check how many functions need to save from stockA
	if (*index > 8)
		Aind = 7;
	else
		Aind = *index;

	/* Saving stock A */
	for (i = Aind - 1; i >= 0; i--)
	{
		fprintf(f, "%s\n", stockA[i]);
	}

	/* Saving stock B */
	saveStocksRec(stockB->head, f);
	fclose(f);
}
void saveStocksRec(ListNode * node, FILE * f)
/*This function save stocks using a recursion*/
{
	if (node == NULL)
		return;
	saveStocksRec(node->next, f);
	fprintf(f, "%s\n", node->commands);
	return;
}

/*Loading*/
void load(char ** stockA, List * stockB, AptList *FullAprts, int * index)
/*This function load Stocks and Aprts from files*/
{
	loadAprts(FullAprts);
	loadStocks(stockA, stockB, index);
}
void loadAprts(AptList * FullAprts)
/*This function load Aprts from file to Fullaprts*/
{
	Aprtment aprt;
	char mem[5];
	short int adrslen;
	FILE* f = fopen("apartments.bin", "rb");
	if (f == NULL)
		return;
	long int fsize = fileSize(f);
	while (ftell(f) < fsize)
	{
		fread(mem, sizeof(char), 5, f);
		aprt = BitsToAprt(mem);
		fread(&aprt.code, sizeof(short int), 1, f);
		fread(&adrslen, sizeof(short int), 1, f);
		aprt.adress = (char*)malloc(sizeof(char)*adrslen + 1);
		fread(aprt.adress, sizeof(char), adrslen, f);
		aprt.adress[adrslen] = '\0';
		fread(&aprt.price, sizeof(int), 1, f);
		CreateAptNode(FullAprts, aprt);
	}
	FullAprts->TotalIndex = aprt.code; //update index of aprts

	fclose(f);
}
void loadStocks(char ** stockA, List * stockB, int * index)
/*This function load commands from file to stockA and stockB*/
{
	int size, logicSize = 0;
	char ch;
	char * temp;
	FILE* f = fopen("commands.txt", "r");
	if (f == NULL)
		return;

	long int filesize = fileSize(f);
	//load the file using a recorsuion	
	loadStocksRec(f,filesize,stockA, stockB,index);
	fclose(f);
}
void loadStocksRec(FILE * f,long int filesize, char ** stockA, List * stockB, int * index)
/*This function load commands from file to stockA and stockB using a recorsuion*/
{
	char * temp;
	int i=0;
	char ch;
	if (ftell(f) >= filesize)
		return;

	temp = (char*)calloc(1 + rowlen(f), sizeof(char));
	ch = fgetc(f);
	while (ch != '\n')
	{
		temp[i++] = ch;
		ch = fgetc(f);
	}
	loadStocksRec(f, filesize, stockA, stockB, index);
	addStockA(temp,stockA,stockB,index);
}

/*Bit conventors*/
void AprtToBits(char * mem, Aprtment Aprt)
/*This function turn Aprt into bits*/
{
	int i;
	for (i = 0; i < 5; i++)
		mem[i] = 0;

	//num of room 
	mem[0] = Aprt.rooms << 4;

	//enter date
	mem[0] = ((Aprt.enter_date.day >> 1) & 15) | mem[0];
	mem[1] = ((Aprt.enter_date.day << 7));
	mem[1] = ((Aprt.enter_date.month << 3) & 120) | mem[1];
	mem[1] = (Aprt.enter_date.year >> 4) & 7 | mem[1];
	mem[2] = (Aprt.enter_date.year << 4) & 240;

	//added date
	mem[3] = Aprt.added_date.day << 3;
	mem[3] = ((Aprt.added_date.month >> 1) & 7) | mem[3];
	mem[4] = Aprt.added_date.month << 7;
	mem[4] = (Aprt.added_date.year & 127) | mem[4];
	return;
}
Aprtment BitsToAprt(char * mem)
/*This function turn bits into an aparment*/
{
	Aprtment aprt;
	//rooms
	aprt.rooms = (mem[0] >> 4) & 15;

	//enter date
	aprt.enter_date.day = (mem[0] << 1) & 30 | (mem[1] >> 7) & 1;
	aprt.enter_date.month = (mem[1] >> 3) & 15;
	aprt.enter_date.year = (mem[1] << 4) & 112 | (mem[2] >> 4) & 15;

	//added date
	aprt.added_date.day = (mem[3] >> 3) & 15;
	aprt.added_date.month = (mem[3] << 1) & 14 | (mem[4] >> 7) & 1;
	aprt.added_date.year = mem[4] & 127;
	return aprt;
}