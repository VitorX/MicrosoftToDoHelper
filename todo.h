#ifndef __todo_h_
#define __todo_h_
#endif

#define MAXURLLEN 100
#define APIENDPOINT "https://graph.microsoft.com"
#define APIVERSION "v1.0"

#include <stdbool.h>
#include <curl/curl.h>


struct todoList_s{
char displayName[256];
char id[128];
bool isShared;
};


int createTaskList(char *pName,char *pTokenHeader,struct todoList_s *pTodoList);
