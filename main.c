#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "todo.h"


#define LASTPAGE 1600
#define TOKEN "eyJ0eXAiOiJKV1QiLCJub25jZSI6IlFGUXFhRGNManp2MXlXdExXOGN0RUJOOWQ2OGctNG1aMGkyNUFfeVNMWjQiLCJhbGciOiJSUzI1NiIsIng1dCI6Ik1HTHFqOThWTkxvWGFGZnBKQ0JwZ0I0SmFLcyIsImtpZCI6Ik1HTHFqOThWTkxvWGFGZnBKQ0JwZ0I0SmFLcyJ9.eyJhdWQiOiIwMDAwMDAwMy0wMDAwLTAwMDAtYzAwMC0wMDAwMDAwMDAwMDAiLCJpc3MiOiJodHRwczovL3N0cy53aW5kb3dzLm5ldC83MmY5ODhiZi04NmYxLTQxYWYtOTFhYi0yZDdjZDAxMWRiNDcvIiwiaWF0IjoxNzE5MjEyMDIzLCJuYmYiOjE3MTkyMTIwMjMsImV4cCI6MTcxOTIxNjM3OSwiYWNjdCI6MCwiYWNyIjoiMSIsImFjcnMiOlsidXJuOnVzZXI6cmVnaXN0ZXJzZWN1cml0eWluZm8iXSwiYWlvIjoiQVlRQWUvOFhBQUFBK1hYRUFKdm01bkx2ZjlVVlFNZXIwRlk1ZzNjVEw0ZWUxbFJNMVVwVW52MU5heEZUWUdxaEE1bm1BTU91T1U4WE1FMkVqR29MUTdZVWJ6Z0dsYXJleGZYM2pVanZheCt3SlhlMmhFM3owa0Vhb0ttUHNpYVBzVzNDZ2gwclRNZ0ptZkpZaDRxUkFRdUNyYko2dkxSMDdUaG1zV0NZTnFObERZK1BMME9JblBvPSIsImFtciI6WyJyc2EiLCJtZmEiXSwiYXBwX2Rpc3BsYXluYW1lIjoiR3JhcGggRXhwbG9yZXIiLCJhcHBpZCI6ImRlOGJjOGI1LWQ5ZjktNDhiMS1hOGFkLWI3NDhkYTcyNTA2NCIsImFwcGlkYWNyIjoiMCIsImNhcG9saWRzX2xhdGViaW5kIjpbIjU5NTZmZjVhLTZmZGItNDc3ZS05ZDRkLTlmN2QyNjJlNjk0YSJdLCJjb250cm9scyI6WyJhcHBfcmVzIl0sImNvbnRyb2xzX2F1ZHMiOlsiZGU4YmM4YjUtZDlmOS00OGIxLWE4YWQtYjc0OGRhNzI1MDY0IiwiMDAwMDAwMDMtMDAwMC0wMDAwLWMwMDAtMDAwMDAwMDAwMDAwIiwiMDAwMDAwMDMtMDAwMC0wZmYxLWNlMDAtMDAwMDAwMDAwMDAwIl0sImRldmljZWlkIjoiZTk3ZGI4YTQtYWY3ZS00YWRiLTljYzUtODdiZjg1MjJkMDc5IiwiZmFtaWx5X25hbWUiOiJYdWUiLCJnaXZlbl9uYW1lIjoiRmVpIiwiaWR0eXAiOiJ1c2VyIiwiaXBhZGRyIjoiMjQwNDpmODAxOjkwMDA6MTg6YjE0OTplNDA2OjU1NTA6NDc5YyIsIm5hbWUiOiJGZWkgWHVlIChTaGFuZ2hhaSBXaWNyZXNvZnQgQ28gTHRkKSIsIm9pZCI6ImQ3MDE0ZjI2LWE3N2ItNGM2Yy1hMWIxLWFlMmViODZiNTMyZiIsIm9ucHJlbV9zaWQiOiJTLTEtNS0yMS0yMTI3NTIxMTg0LTE2MDQwMTI5MjAtMTg4NzkyNzUyNy03NTY5OTgwNCIsInBsYXRmIjoiMyIsInB1aWQiOiIxMDAzMjAwMzYzMEVEN0FEIiwicmgiOiIwLkFSb0F2NGo1Y3ZHR3IwR1JxeTE4MEJIYlJ3TUFBQUFBQUFBQXdBQUFBQUFBQUFBYUFHdy4iLCJzY3AiOiJDYWxlbmRhcnMuUmVhZFdyaXRlIENvbnRhY3RzLlJlYWRXcml0ZSBEZXZpY2VNYW5hZ2VtZW50QXBwcy5SZWFkV3JpdGUuQWxsIERldmljZU1hbmFnZW1lbnRDb25maWd1cmF0aW9uLlJlYWQuQWxsIERldmljZU1hbmFnZW1lbnRDb25maWd1cmF0aW9uLlJlYWRXcml0ZS5BbGwgRGV2aWNlTWFuYWdlbWVudE1hbmFnZWREZXZpY2VzLlByaXZpbGVnZWRPcGVyYXRpb25zLkFsbCBEZXZpY2VNYW5hZ2VtZW50TWFuYWdlZERldmljZXMuUmVhZC5BbGwgRGV2aWNlTWFuYWdlbWVudE1hbmFnZWREZXZpY2VzLlJlYWRXcml0ZS5BbGwgRGV2aWNlTWFuYWdlbWVudFJCQUMuUmVhZC5BbGwgRGV2aWNlTWFuYWdlbWVudFJCQUMuUmVhZFdyaXRlLkFsbCBEZXZpY2VNYW5hZ2VtZW50U2VydmljZUNvbmZpZy5SZWFkLkFsbCBEZXZpY2VNYW5hZ2VtZW50U2VydmljZUNvbmZpZy5SZWFkV3JpdGUuQWxsIERpcmVjdG9yeS5BY2Nlc3NBc1VzZXIuQWxsIERpcmVjdG9yeS5SZWFkV3JpdGUuQWxsIEZpbGVzLlJlYWRXcml0ZS5BbGwgR3JvdXAuUmVhZFdyaXRlLkFsbCBJZGVudGl0eVJpc2tFdmVudC5SZWFkLkFsbCBNYWlsLlJlYWRXcml0ZSBNYWlsYm94U2V0dGluZ3MuUmVhZFdyaXRlIE5vdGVzLlJlYWRXcml0ZS5BbGwgb3BlbmlkIFBlb3BsZS5SZWFkIFBvbGljeS5SZWFkLkFsbCBQcmVzZW5jZS5SZWFkIFByZXNlbmNlLlJlYWQuQWxsIHByb2ZpbGUgUmVwb3J0cy5SZWFkLkFsbCBTaXRlcy5SZWFkV3JpdGUuQWxsIFRhc2tzLlJlYWRXcml0ZSBVc2VyLlJlYWQgVXNlci5SZWFkQmFzaWMuQWxsIFVzZXIuUmVhZFdyaXRlIFVzZXIuUmVhZFdyaXRlLkFsbCBlbWFpbCIsInNpZ25pbl9zdGF0ZSI6WyJkdmNfbW5nZCIsImR2Y19jbXAiLCJpbmtub3dubnR3ayIsImttc2kiXSwic3ViIjoibWQ4TWNXbWVmbXBsOEF3YTl2ZmhwcllWOFI0aWhzckc5OGkyN1FGVWtjbyIsInRlbmFudF9yZWdpb25fc2NvcGUiOiJXVyIsInRpZCI6IjcyZjk4OGJmLTg2ZjEtNDFhZi05MWFiLTJkN2NkMDExZGI0NyIsInVuaXF1ZV9uYW1lIjoidi1mZWl4dWUxQG1pY3Jvc29mdC5jb20iLCJ1cG4iOiJ2LWZlaXh1ZTFAbWljcm9zb2Z0LmNvbSIsInV0aSI6IkJmZzhjWnZ2RDBLVER6V2MyN3BSQUEiLCJ2ZXIiOiIxLjAiLCJ3aWRzIjpbImI3OWZiZjRkLTNlZjktNDY4OS04MTQzLTc2YjE5NGU4NTUwOSJdLCJ4bXNfY2MiOlsiQ1AxIl0sInhtc19pZHJlbCI6IjEgMiIsInhtc19zc20iOiIxIiwieG1zX3N0Ijp7InN1YiI6ImJadUNVREgwMzAyb3c5cjdTTnRFUXFWeElLaGlWVkw3cEhrajdXZVNuek0ifSwieG1zX3RjZHQiOjEyODkyNDE1NDd9.j7l6ABD3_pf-NPJA5GVdkb2qxwcd-2cmnFEouPrvmfml1oEL8_Qz8IRDSieVdic_X4ypuJoMdPd9ZYtCNjOze-hLlvILcPk8jeOYk_8mp866eqBqpjjHoqnbUIHUJhXNrtvHyv2GryLrsvxYxCjlMbpgd96VYzdYSm2-zNOlwpAzGA6eAZvyshPNYeK-QpfrVq-WZieNkszscbKrpCikdG1o4IICzxnDA06HegEsvqL3mBGt5NoHM4mQjzh9kkCzKwIW3quta4b7dPi_B2fFbg6x5DcugmH-IMLVJ0O-liCGsm9Ops9cZlvQdlzE_qtlnqkjLCzmTpvNx7cCg1EYIg"
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
	struct todoList_s todolist={};
	createTaskList("abc",pTokenHeader,&todolist);
	//printf("%s:%s\n",__func__,todolist.id);
	//printf("%s:%s\n",__func__,todolist.displayName);
	//return 1;
	createTask(todolist.id,20,"2024-06-22T00:00:00.0000000",pTokenHeader);
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
