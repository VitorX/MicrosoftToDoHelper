#include "todo.h"

int createTaskList(char *name,char *pTokenHeader,struct todoList_s *pTodolist)
{
	char endpoint[MAXURLLEN];
	sprintf((char *)endpoint,"%s/%s/me/todo/lists",APIENDPOINT,APIVERSION);
	CURL *curl_tasklist;
	struct curl_slist* headers = NULL;
	headers =    curl_slist_append(headers, "Content-Type: application/json");
	headers =  curl_slist_append(headers, pTokenHeader);
	CURLcode res;
	char postTemp[] = "{'displayName':'%s'}";
	char postData[100] ;
	sprintf(postData,postTemp,name);
	curl_tasklist = curl_easy_init();

	if (curl_tasklist) {
		curl_easy_setopt(curl_tasklist, CURLOPT_URL,endpoint) ;
		curl_easy_setopt(curl_tasklist, CURLOPT_HTTPHEADER,headers) ;
		curl_easy_setopt(curl_tasklist, CURLOPT_POSTFIELDS, postData);
		res = curl_easy_perform(curl_tasklist);
		curl_easy_cleanup(curl_tasklist);
	}
	if(res==CURLE_OK)
		printf("\nsend create task sucessfully!");
	else
		printf("\ncurl_easy_perform is failed with error: %d",res);
}
int getTaskList(char *id)
{
	return 0;
}

int createTask(char *tasklistid)
{
	return 0;
}

