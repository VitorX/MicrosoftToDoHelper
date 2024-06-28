#ifndef __todo_h_
#define __todo_h_
#endif

#define APIENDPOINT "https://graph.microsoft.com"
#define APIVERSION "v1.0"
#define DEBUG
#include <stdbool.h>
#include <curl/curl.h>



struct cmd_s{
	char *pTaskListName;
	char *pToken;
	int nums;
	int range;
	struct tm *startDate;
	char *timeZone;
	bool isCreate;
	bool printResponse;
};
struct todoList_s{
char *displayName;
char *id;
bool isShared;
};


int createTask(char *tasklistid,struct cmd_s *pCMD,char *pTokenHeader);
void createTaskList(char *pName,char *pTokenHeader,struct todoList_s *pTodoList);
