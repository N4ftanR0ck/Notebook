#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "item.h"
#include <string.h>
#include <malloc.h>
#include "splay.h";


item* dateIn(item* root, FILE* stream)
{
	item* itm = (item*)malloc(sizeof(item));
	itm = getDate(itm, stream);
	itm = getTime(itm, stream);
	itm = getevent(itm, stream);
	itm = getLocal(itm, stream);
	itm = getPriority(itm, stream);
	int date = dateKey(itm);
	return insert_date(root, date, itm, stream);
}

item* prIn(item* root, FILE* stream)
{
	item* itm = (item*)malloc(sizeof(item));
	itm = getDate(itm, stream);
	itm = getTime(itm, stream);
	itm = getevent(itm, stream);
	itm = getLocal(itm, stream);
	itm = getPriority(itm, stream);
	int pr = itm->priority;
	return insert_pr(root, pr, itm, stream);
}

item* deleteKey(item* root)
{
	printf("Enter date and name of event what you want to delete\n");
	item* itm = (item*)malloc(sizeof(item));
	itm = getDate(itm, stdin);
	itm = getevent(itm, stdin);
	root = delete_key_date(root, itm);
	return root;
}

item* fileInputP(item* root, FILE* stream)
{
	while (!feof(stream))
	{
		root = prIn(root, stream);
	}
	return root;
}

item* fileInputD(item* root, FILE* stream)
{
	while (!feof(stream))
	{
		root = dateIn(root, stream);
	}
	return root;
}

int findItem(item* root, char* name, int count)
{
	if (root != NULL)
	{
		count = findItem(root->left, name, count);
		++count;
		for (int i = 0; i < strlen(root->locate); i++)
		{
			if (!strncmp(root->locate + i, name, strlen(name)))
			{
				printItem(root, count);
				count = findItem(root->right, name, count);
				return count;
			}
		}
		count = findItem(root->right, name, count);
	}
	return count;
}


int main(int argc, char* argv[])
{

	FILE* input;
	FILE* output;

	/*input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");*/

	input = fopen("input.txt", "r");
	output = fopen("output.txt", "w");

	int year; //0 .. char_max
	char day; // 1 .. 31
	char mounth; // 1 .. 12
	char hour; // 0 .. 23
	char min; // 0 .. 59
	char event[50]; // getStroke();

	char c = 0;
	int p = 0; int d = 0;
	item* root = NULL;
	

	FILE* strm;

	int count = 0;
	char name[100];

	printf("+ - add item\n- - delete item\nl - list items\nf - add items from file\np - find event by location\nh - help\nq - quit\n");
	while (c != 'q')
	{
		c = getc(stdin);
		switch (c)
		{
			case '+':
				if (p == 0)
					root = dateIn(root, stdin);
				else if (d == 0)
					root = prIn(root, stdin);
				break;
			case '-':
				root = deleteKey(root);
				break;
			case 'f':
				printf("input name of the file: ");
				char name[100];
				fgetc(stdin);
				fscanf(stdin, "%s", name);
				strm = fopen(name, "r");
				if (d == 0)
					root = fileInputP(root, strm);
				else
					root = fileInputD(root, strm);
				fclose(strm);
				break;
			case 'l':
				printf("sort by date or priority?\nd - date\np - priority\n");
				c = getc(stdin);
				c = getc(stdin);
				if (c == 'p')
				{
					item* newroot = NULL;
					if (p == 0)
					{
						newroot = reTreePr(newroot, root);
						root = newroot;
					}
					d = 0;
					++p;
					printTree(root, 0);
					printf("-------------------------------------------------+\n");
				}
				if (c == 'd')
				{
					item* newroot = NULL;
					if (d == 0)
					{
						newroot = reTreeDate(newroot, root);
						root = newroot;
					}
					p = 0;
					++d;
					printTree(root, 0);
					printf("-------------------------------------------------+\n");
				}
				break;
			case 'p':
				printf("input part of the event's location: ");
				fgetc(stdin);
				fscanf(stdin, "%s", name);
				findItem(root, name, count);
				printf("-------------------------------------------------+\n");
				break;
			case 'q':
				return 0;
				break;
			case 'h':
				printf("+ - add item\n- - delete item\nl - list items\nf - add items from file\nh - help\nq - quit\n");
				break;
		}
	}
	
	return 0;
}