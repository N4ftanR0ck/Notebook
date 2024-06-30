#define ITEM_H
#pragma once
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>



typedef struct item
{
	int year; //0 .. char_max
	char day; // 1 .. 31
	char mounth; // 1 .. 12
	char hour; // 0 .. 23
	char min; // 0 .. 59
	char* event;// getStroke();
	char* locate;
	char priority;
	item* left;
	item* right;
} item;


void printDayWeek(int year, char day, char mounth);
void printItem(item* itm, int count);
int printTree(item* root, int count);
int ifNum(char c);
int notNumCount(char* s);
int numCount(char* s);
item* getDate(item* itm, FILE* stream);
item* getTime(item* itm, FILE* stream);
item* getevent(item* itm, FILE* stream);
item* getLocal(item* itm, FILE* stream);
item* getPriority(item* itm, FILE* stream);




void printDayWeek(int year, char day, char mounth)
{
	int t = (6 + year % 100 + year % 100 / 4) % 7;
	char arr1[12] = { 1,4,4,0,2,5,0,3,6,1,4,6 };
	char arr2[12] = { 0,3,4,0,2,5,0,3,6,1,4,6 };

	int week = 0;
	if (year % 4 == 0)
		week = (arr2[mounth - 1] + day + t) % 7;
	else
		week = (arr1[mounth - 1] + day + t) % 7;
	switch (week)
	{
		case 2:	
			printf("day:\t|Monday\n");
			break;
		case 3:
			printf("day:\t|Tuesday\n");
			break;
		case 4:
			printf("day:\t|Wednesday\n");
			break;
		case 5:
			printf("day:\t|Thursday\n");
			break;
		case 6:
			printf("day:\t|Friday\n");
			break;
		case 0:
			printf("day:\t|Saturday\n");
			break;
		case 1:
			printf("day:\t|Sunday\n");
			break;

	}
}

void printItem(item* itm, int count)
{
	if (itm != NULL)
	{
		printf("---+---------------------------------------------+\n");
		printf(" %d | %s", count, itm->event);
		printf("---+---------------------------------------------+\n");
		printf("date:\t");
		if (itm->day < 10)
			printf("|0%d.", itm->day);
		else
			printf("|%d.", itm->day);
		if (itm->mounth < 10)
			printf("0%d.", itm->mounth);
		else
			printf("%d.", itm->mounth);
		printf("%d\n", itm->year);
		printf("time:\t|", itm->hour, itm->min);
		if (itm->hour < 10)
			printf("0%d:", itm->hour);
		else
			printf("%d:", itm->hour);
		if (itm->min < 10)
			printf("0%d\n", itm->min);
		else
			printf("%d\n", itm->min);
		printDayWeek(itm->year, itm->day, itm->mounth);
		/*if (itm->event != NULL)
			printf("name:\t|%s", itm->event);*/
		if (itm->locate != NULL)
			printf("locate:\t|%s", itm->locate);
		printf("prior:\t|%d\n", itm->priority);
		//printf("%d\n", dateKey1(itm));
	}
}

int printTree(item* root, int count)
{
	if (root != NULL)
	{
		count = printTree(root->left, count);
		++count;
		printItem(root, count);
		count = printTree(root->right, count);
	}
	return count;
}

int ifNum(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

int notNumCount(char* s)
{
	int count = 0;
	while ((*s) != '\0')
	{
		if (!ifNum(*s))
			++count;
		++s;
	}
	return count;
}

int numCount(char* s)
{
	int count = 0;
	while ((*s) != '\0')
	{
		if (ifNum(*s))
			++count;
		++s;
	}
	return count;
}

item* getDate(item* itm, FILE* stream)
{
	if (itm == NULL)
		return 0;
	if (stream == stdin)
		printf("Enter date in format DD.MM.YYYY (for example 01.02.2005 or 28.06.0988): ");
	char line[12];
	fscanf_s(stream,"%s",line, 12);
	if (stream == stdin) {
		while (line[2] != '.' || line[5] != '.' || notNumCount(line) != 2 || numCount(line) != 8 || ((line[0] - '0') * 10 + (line[1] - '0')) > 31 || (line[0] == '0' && line[1] == '0') || ((line[3] - '0') * 10 + line[4] - '0') > 12)
		{
			printf("error!");
			if (stream == stdin)
			{
				printf("\ntry again:");
				fscanf_s(stream, "%s", line, 12);
			}
		}
	}
	itm->day = (line[0] - '0') * 10 + (line[1] - '0');
	itm->mounth = (line[3] - '0') * 10 + line[4] - '0';
	itm->year = (line[6] - '0') * 1000 + (line[7] - '0') * 100 + (line[8] - '0') * 10 + (line[9] - '0');
	return itm;
}
item* getTime(item* itm, FILE* stream)
{
	if (itm == NULL)
		return 0;
	if (stream == stdin)
		printf("Enter event time in format HH:MM (for example 23:15 or 00:02): ");
	char line[7];
	fscanf_s(stream, "%s", line, 7);
	if (stream == stdin)
	{
		while (line[2] != ':' || notNumCount(line) != 1 || ((line[0] - '0') * 10 + (line[1] - '0')) > 23 || ((line[3] - '0') * 10 + line[4] - '0' > 59))
		{
			printf("error!");
			if (stream == stdin)
			{
				printf("\ntry again:");
				fscanf_s(stream, "%s", line, 7);
			}
		}
	}
	itm->hour = (line[0] - '0') * 10 + (line[1] - '0');
	itm->min = (line[3] - '0') * 10 + line[4] - '0';
	return itm;
}
item* getevent(item* itm, FILE* stream)
{
	if (itm == NULL)
		return 0;
	if (stream == stdin)
		printf("Enter event description (not over 100 symbols!): ");
	char line[100];
	fscanf_s(stream, "%c", line, 1);
	fgets(line, 100, stream);
	//fscanf_s(stream, "%s[\n]", line, 100);
	itm->event = (char*)malloc(100 * sizeof(char));
	strcpy(itm->event, line);
	return itm;
}

item* getLocal(item* itm, FILE* stream)
{
	if (itm == NULL)
		return 0;
	if (stream == stdin)
		printf("Enter event location (not over 50 symbols!): ");
	char line[50];
	//gets_s(line, 50);
	fgets(line, 50, stream);
	//fscanf_s(stream, "%s[\n]", line, 50);
	itm->locate = (char*)malloc(50 * sizeof(char));
	
	strcpy(itm->locate, line);
	return itm;
}

item* getPriority(item* itm, FILE* stream)
{
	if (itm == NULL)
		return 0;
	if (stream == stdin)
		printf("Enter event priority: ");
	char line[3];
	fscanf_s(stream, "%s", line, 3);

	if (stream == stdin) 
	{
		while ((line[0] - '0') * 10 + line[1] - '0' > 10 || numCount(line) > 2)
		{
			printf("error!");
			if (stream == stdin)
			{
				printf("\ntry again:");
				fscanf_s(stream, "%s", line, 3);
			}
		}
	}
	if (line[0] == '1' && line[1] == '0')
		itm->priority = 10;
	else
		itm->priority = line[0] - '0';
	return itm;
}



