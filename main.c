#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "todo.h"
#include <time.h>


#define TOKENAUTH "Authorization: Bearer %s"

static char *pTokenHeader;

static void initTokenHeader(char *token);
static void freeResouce();
static void parseArgs(int argc, char **argv);
static void showCmd();
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
	freeResouce(pTokenHeader);
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

}
static void parseArgs(int argc, char **argv)
{
	int nrTasks,opt;
	while ((opt = getopt(argc, argv, "c:n:t:r:hs:q")) != -1) {
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
	printf("\ntaskName:%s\n",cmd.pTaskListName);
	printf("token:%s\n",cmd.pToken);
	printf("%d-%02d-%02d\n",cmd.startDate->tm_year+1900,cmd.startDate->tm_mon+1,cmd.startDate->tm_mday);
}


static void freeResouce()
{
	free(pTokenHeader);
	if(cmd.pTaskListName)
		free(cmd.pTaskListName);
	if(cmd.pToken)
		free(cmd.pToken);
	if(cmd.startDate)
		free(cmd.startDate);
	if(cmd.timeZone)
		free(cmd.timeZone);
}
static void initTokenHeader(char *token)
{
	pTokenHeader=malloc(1024*7);
	if(pTokenHeader==NULL)
	{
		fprintf(stderr,"malloc failed!");
		return ;
	}
	sprintf(pTokenHeader,TOKENAUTH,token);
}

static void printHelpMsg()
{
	printf("todolist - This tools is used to create multiple tasks for Microsoft todo App\n");
	printf("Usage: todolist -c {todolistName} -n 500 -t {token} -r 50 -s 2024-6-21 \n");
	printf("	-c, Create todo List\n");
	printf("	-n, Num of todo task of todo list\n");
	printf("	-t, Token of used for query or create tasks\n");
	printf("	-h, Print help message\n");
	printf("	-r, Range of tasks, for example if r is 50, then the tasks 1-50,51-100... will be created. 25 is by default.\n");
	printf("	-s, Start date of the first task\n");
	printf("	-q, List the todo list owned\n");
}
