#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "todo.h"


#define LASTPAGE 1600
#define TOKEN ""
#define TOKENAUTH "Authorization: Bearer %s"

static char *pTokenHeader;

static void initTokenHeader(char *token);
static void freeTokenHeader(char *pTokenHeader);
static void parseArgs(int argc, char **argv);
static void showCmd();
static void printHelpMsg();
struct cmd_s{
	char *pTaskListName;
	char *pToken;
	int nums;
	char *startDate;
	char *timeZone;
	bool isCreate;
}cmd;

int main(int argc, char **argv)
{
	parseArgs(argc,argv);
	//showCmd();
	//return 0;
	initTokenHeader(TOKEN);
	struct todoList_s todolist;
	//createTaskList("abc",pTokenHeader,&todolist);
	createTask("AAMkADhiYTQ4ZTkyLTFhYTctNGJmYy1iYjJkLTY5ZmJmZjBiMGVkYQAuAAAAAADd-_KWVsEtR5gXDuPf7t1hAQA8PgoqDG7ARIJwzLLIi0ZeAAA-mY9GAAA=",5,"2024-06-22T00:00:00.0000000");
	freeTokenHeader(pTokenHeader);
	return 0;
}

static void parseArgs(int argc, char **argv)
{
	int nrTasks,opt;
	while ((opt = getopt(argc, argv, "cn:t:r:hs:q")) != -1) {
		switch (opt) {
			case 'c':
				cmd.isCreate=true;
				break;		
			case 'n':
				nrTasks=atoi(optarg);
				if(nrTasks==0)
				{
					printf("Wrong usage,please specify the num of tasks you want to create.\n");
					exit(-1);
				}
				else
				cmd.nums=nrTasks;
				break;
			case 't':
				cmd.pToken = strdup(optarg);
				break;
			case 'h':
				printHelpMsg();
				break;
			case 's':
				cmd.startDate=strdup(optarg);	
			default: 
				printHelpMsg();
				exit(EXIT_FAILURE);
		}
	}
/*
	if (optind >= argc) {
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}
*/
}

static void showCmd()
{
	printf("taskName:%s",cmd.pTaskListName);
	printf("token:%s",cmd.pToken);

}


static void freeTokenHeader(char *pTokenHeader)
{
	free(pTokenHeader);
}
static void initTokenHeader(char *token)
{
	pTokenHeader=malloc(1024*7);
	if(pTokenHeader==NULL)
	{
		fprintf(stderr,"malloc failed!");
		return ;
	}
	int size=sprintf(pTokenHeader,TOKENAUTH,token);
}

static void printHelpMsg()
{
	printf("todolist - This tools is used to create multiple tasks for Microsoft todo App\n");
	printf("Usage: todolist -c -n 500 -t {token} -r 50 -s 2024-6-21 {todolistName}");
	printf("	-c, Create todo List");
	printf("	-n, Num of todo task of todo list");
	printf("	-t, Token of used for query or create tasks");
	printf("	-h, Print help message");
	printf("	-r, Range of tasks, for example if r is 50, then the tasks 1-50,51-100... will be created. 25 is by default.");
	printf("	-s, Start date of the first task");
	printf("	-q, List the todo list owned");
}
