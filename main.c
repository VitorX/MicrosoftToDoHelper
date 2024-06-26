#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "todo.h"
#include <time.h>


#define TOKENAUTH "Authorization: Bearer "

static char *pTokenHeader;

static void initTokenHeader(char *token);
static void freeResouce();
static void parseArgs(int argc, char **argv);
static void printHelpMsg();
static void initCmdPara();

extern struct cmd_s cmd;
int main(int argc, char **argv)
{
	initCmdPara();
	parseArgs(argc,argv);
	initTokenHeader(cmd.pToken);
	struct todoList_s todolist={};
	createTaskList(cmd.pTaskListName,pTokenHeader,&todolist);
	createTask(todolist.id,&cmd,pTokenHeader);
	freeResouce();
	return 0;
}

static void initCmdPara()
{
	time_t t = time(NULL);
	struct tm *pTM=malloc(sizeof(struct tm));
	*pTM = *localtime(&t);
	cmd.startDate=pTM;	
	cmd.timeZone="Asia/Shanghai";
	cmd.range=25;
	cmd.printResponse=false;

}
static void parseArgs(int argc, char **argv)
{
	int nrTasks,opt;
	while ((opt = getopt(argc, argv, "c:n:t:r:hs:qv")) != -1) {
		switch (opt) {
			case 'c':
				cmd.pTaskListName = strdup(optarg);
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
				exit(0);
				break;
			case 's':
				strftime(optarg,10+1,"%Y-%m-%d",cmd.startDate);
				printf("%d",cmd.startDate->tm_year);
				exit(1);
				break;
			case 'r':
				cmd.range=atoi(optarg);
				break;
			case 'v':
				cmd.printResponse=true;
				break;
			default: 
				printHelpMsg();
				exit(EXIT_FAILURE);
		}
	}
	//caculate n again based on the range
	cmd.nums=cmd.nums/cmd.range+1;
/*	
	   if (optind >= argc) {
	   fprintf(stderr, "Expected argument after options\n");
	   exit(EXIT_FAILURE);
	   }
*/
}

static void freeResouce()
{
	if(pTokenHeader)
		free(pTokenHeader);
	if(cmd.startDate)
		free(cmd.startDate);
}
static void initTokenHeader(char *token)
{
	pTokenHeader=calloc(1,strlen(TOKENAUTH)+strlen(token)+1);
	if(pTokenHeader==NULL)
	{
		fprintf(stderr,"malloc failed!");
		return ;
	}
	pTokenHeader=strcat(pTokenHeader,TOKENAUTH);
	pTokenHeader=strcat(pTokenHeader,token);
}

static void printHelpMsg()
{
	printf("todolist - This tools is used to create multiple tasks for Microsoft todo App. Version 0.0.1.\n");
	printf("Usage: todolist -c {todolistName} -n 500 -t {token} -r 50 -s 2024-6-21 \n");
	printf("	-c, Create todo List\n");
	printf("	-n, Num of todo task of todo list\n");
	printf("	-t, Token of used for query or create tasks\n");
	printf("	-h, Print help message\n");
	printf("	-r, Range of tasks, for example if r is 50, then the tasks 1-50,51-100... will be created. 25 is by default.\n");
	printf("	-s, Start date of the first task\n");
	printf("	-v, Print REST request response.\n");
	printf("	-q, List the todo list owned(not support yet)\n");
}
