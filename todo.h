#ifndef __todo_h_
#define __todo_h_
#endif

#define MAXURLLEN 100
#define APIENDPOINT "https://graph.microsoft.com"
#define APIVERSION "v1.0"
#define DEBUG
#include <stdbool.h>
#include <curl/curl.h>


struct todoList_s{
char *displayName;
char *id;
bool isShared;
};


int createTask(char *tasklistid,int nrTask,char *beginDate,char *pTokenHeader);
int createTaskList(char *pName,char *pTokenHeader,struct todoList_s *pTodoList);
