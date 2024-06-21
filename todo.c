#include "todo.h"
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#define CONTENTTYPE_JSON "Content-Type: application/json"
#define MAX_PARALLEL 10

struct MemoryStruct {
	char *memory;
	size_t size;
};

struct todoTask
{
	char title[128];
	char dueDateTime[128];

};

struct todoList_s todolist;

int parseJSON(char *pChar,struct todoList_s *pTodolist)
{
	json_object *root = json_tokener_parse(pChar);
	json_object *id = json_object_object_get(root, "id");
	json_object *displayName = json_object_object_get(root, "displayName");
	memcpy(todolist.id,json_object_get_string(id),sizeof(todolist.id)/sizeof(todolist.id[0])-1);
	memcpy(todolist.displayName,json_object_get_string(displayName),sizeof(todolist.displayName)/sizeof(todolist.displayName[0])-1);
	printf("id:%s\n",todolist.id);
	printf("displayName:%s\n",todolist.displayName);
}
	static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}
int createTaskList(char *name,char *pTokenHeader,struct todoList_s *pTodolist)
{
	struct MemoryStruct chunk;

	chunk.memory = malloc(1);  /* grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	char endpoint[MAXURLLEN];
	sprintf((char *)endpoint,"%s/%s/me/todo/lists",APIENDPOINT,APIVERSION);
	CURL *curl_tasklist;
	struct curl_slist* headers = NULL;
	headers =    curl_slist_append(headers,CONTENTTYPE_JSON );
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
		curl_easy_setopt(curl_tasklist, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_tasklist, CURLOPT_WRITEDATA, (void *)&chunk);	
		res = curl_easy_perform(curl_tasklist);
		curl_easy_cleanup(curl_tasklist);
	}


	if(res==CURLE_OK)
	{		printf("\nsend create task sucessfully!\n");

#ifdef DEBUG
		printf("%s\n",chunk.memory);
#endif
		parseJSON(chunk.memory,&todolist);
	}
	else
		printf("\ncurl_easy_perform is failed with error: %d\n",res);

}
int getTaskList(char *id)
{
	return 0;
}
static size_t write_cb(char *data, size_t n, size_t l, void *userp)
{
	/* take care of the data here, ignored in this example */
	(void)data;
	(void)userp;
	return n*l;
}

static void add_transfer(CURLM *cm, unsigned int i, int *left)
{
	char *pTaskCreateEndPoint="https://graph.microsoft.com/v1.0/me/todo/lists/AAMkADhiYTQ4ZTkyLTFhYTctNGJmYy1iYjJkLTY5ZmJmZjBiMGVkYQAuAAAAAADd-_KWVsEtR5gXDuPf7t1hAQA8PgoqDG7ARIJwzLLIi0ZeAAA-mY9GAAA=/tasks";
	CURL *eh = curl_easy_init();
	curl_easy_setopt(eh, CURLOPT_URL, pTaskCreateEndPoint);
	curl_easy_setopt(eh, CURLOPT_POSTFIELDS, "{\"title\" : \"new task\",\"dueDateTime\":{\"dateTime\":\"2024-06-21T00:00:00.0000000\",\"timeZone\":\"Asia/Shanghai\"}}");;
	struct curl_slist* headers = NULL;
	headers =    curl_slist_append(headers,CONTENTTYPE_JSON );
	headers =  curl_slist_append(headers, "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJub25jZSI6InpoRktnbTA1ZUYwTVl4My1UVnFDUzA2ZnpyZHJpQjAzZWZpbGY1TDJfQ0UiLCJhbGciOiJSUzI1NiIsIng1dCI6InE3UDFOdnh1R1F3RE4yVGFpTW92alo4YVp3cyIsImtpZCI6InE3UDFOdnh1R1F3RE4yVGFpTW92alo4YVp3cyJ9.eyJhdWQiOiIwMDAwMDAwMy0wMDAwLTAwMDAtYzAwMC0wMDAwMDAwMDAwMDAiLCJpc3MiOiJodHRwczovL3N0cy53aW5kb3dzLm5ldC83MmY5ODhiZi04NmYxLTQxYWYtOTFhYi0yZDdjZDAxMWRiNDcvIiwiaWF0IjoxNzE4OTM4NjUwLCJuYmYiOjE3MTg5Mzg2NTAsImV4cCI6MTcxODk0MzQyNiwiYWNjdCI6MCwiYWNyIjoiMSIsImFjcnMiOlsidXJuOnVzZXI6cmVnaXN0ZXJzZWN1cml0eWluZm8iXSwiYWlvIjoiQVlRQWUvOFhBQUFBdXhrOUU5QzNhdlBxZWdFeGJmVHlrLzYyVzhtN1VFczl4RTY0OFdpWWlkb0VobTI3U1U3T2dIeGtTSTY4QnpDRTZ5ZDh0ZzBVdnY4QWZGWVAxVnR5cU1UUmR4KytYSVM1dWlwQWNCWjRNLzQzREtRYUppZlRXUGxweWZPN2J6cUtRYVZYTGtFeGRQNEc2SzNXeWd1R2NUbStOeGVFL20rdlV1My9lWGxiRGdBPSIsImFtciI6WyJyc2EiLCJtZmEiXSwiYXBwX2Rpc3BsYXluYW1lIjoiR3JhcGggRXhwbG9yZXIiLCJhcHBpZCI6ImRlOGJjOGI1LWQ5ZjktNDhiMS1hOGFkLWI3NDhkYTcyNTA2NCIsImFwcGlkYWNyIjoiMCIsImNhcG9saWRzX2xhdGViaW5kIjpbIjU5NTZmZjVhLTZmZGItNDc3ZS05ZDRkLTlmN2QyNjJlNjk0YSJdLCJjb250cm9scyI6WyJhcHBfcmVzIl0sImNvbnRyb2xzX2F1ZHMiOlsiZGU4YmM4YjUtZDlmOS00OGIxLWE4YWQtYjc0OGRhNzI1MDY0IiwiMDAwMDAwMDMtMDAwMC0wMDAwLWMwMDAtMDAwMDAwMDAwMDAwIiwiMDAwMDAwMDMtMDAwMC0wZmYxLWNlMDAtMDAwMDAwMDAwMDAwIl0sImRldmljZWlkIjoiZTk3ZGI4YTQtYWY3ZS00YWRiLTljYzUtODdiZjg1MjJkMDc5IiwiZmFtaWx5X25hbWUiOiJYdWUiLCJnaXZlbl9uYW1lIjoiRmVpIiwiaWR0eXAiOiJ1c2VyIiwiaXBhZGRyIjoiMjQwNDpmODAxOjkwMDA6MWE6YmVmNDo5OTRkOmVmNGE6Nzk0OSIsIm5hbWUiOiJGZWkgWHVlIChTaGFuZ2hhaSBXaWNyZXNvZnQgQ28gTHRkKSIsIm9pZCI6ImQ3MDE0ZjI2LWE3N2ItNGM2Yy1hMWIxLWFlMmViODZiNTMyZiIsIm9ucHJlbV9zaWQiOiJTLTEtNS0yMS0yMTI3NTIxMTg0LTE2MDQwMTI5MjAtMTg4NzkyNzUyNy03NTY5OTgwNCIsInBsYXRmIjoiMyIsInB1aWQiOiIxMDAzMjAwMzYzMEVEN0FEIiwicmgiOiIwLkFRRUF2NGo1Y3ZHR3IwR1JxeTE4MEJIYlJ3TUFBQUFBQUFBQXdBQUFBQUFBQUFBYUFHdy4iLCJzY3AiOiJDYWxlbmRhcnMuUmVhZFdyaXRlIENvbnRhY3RzLlJlYWRXcml0ZSBEZXZpY2VNYW5hZ2VtZW50QXBwcy5SZWFkV3JpdGUuQWxsIERldmljZU1hbmFnZW1lbnRDb25maWd1cmF0aW9uLlJlYWQuQWxsIERldmljZU1hbmFnZW1lbnRDb25maWd1cmF0aW9uLlJlYWRXcml0ZS5BbGwgRGV2aWNlTWFuYWdlbWVudE1hbmFnZWREZXZpY2VzLlByaXZpbGVnZWRPcGVyYXRpb25zLkFsbCBEZXZpY2VNYW5hZ2VtZW50TWFuYWdlZERldmljZXMuUmVhZC5BbGwgRGV2aWNlTWFuYWdlbWVudE1hbmFnZWREZXZpY2VzLlJlYWRXcml0ZS5BbGwgRGV2aWNlTWFuYWdlbWVudFJCQUMuUmVhZC5BbGwgRGV2aWNlTWFuYWdlbWVudFJCQUMuUmVhZFdyaXRlLkFsbCBEZXZpY2VNYW5hZ2VtZW50U2VydmljZUNvbmZpZy5SZWFkLkFsbCBEZXZpY2VNYW5hZ2VtZW50U2VydmljZUNvbmZpZy5SZWFkV3JpdGUuQWxsIERpcmVjdG9yeS5BY2Nlc3NBc1VzZXIuQWxsIERpcmVjdG9yeS5SZWFkV3JpdGUuQWxsIEZpbGVzLlJlYWRXcml0ZS5BbGwgR3JvdXAuUmVhZFdyaXRlLkFsbCBJZGVudGl0eVJpc2tFdmVudC5SZWFkLkFsbCBNYWlsLlJlYWRXcml0ZSBNYWlsYm94U2V0dGluZ3MuUmVhZFdyaXRlIE5vdGVzLlJlYWRXcml0ZS5BbGwgb3BlbmlkIFBlb3BsZS5SZWFkIFBvbGljeS5SZWFkLkFsbCBQcmVzZW5jZS5SZWFkIFByZXNlbmNlLlJlYWQuQWxsIHByb2ZpbGUgUmVwb3J0cy5SZWFkLkFsbCBTaXRlcy5SZWFkV3JpdGUuQWxsIFRhc2tzLlJlYWRXcml0ZSBVc2VyLlJlYWQgVXNlci5SZWFkQmFzaWMuQWxsIFVzZXIuUmVhZFdyaXRlIFVzZXIuUmVhZFdyaXRlLkFsbCBlbWFpbCIsInNpZ25pbl9zdGF0ZSI6WyJkdmNfbW5nZCIsImR2Y19jbXAiLCJpbmtub3dubnR3ayIsImttc2kiXSwic3ViIjoibWQ4TWNXbWVmbXBsOEF3YTl2ZmhwcllWOFI0aWhzckc5OGkyN1FGVWtjbyIsInRlbmFudF9yZWdpb25fc2NvcGUiOiJXVyIsInRpZCI6IjcyZjk4OGJmLTg2ZjEtNDFhZi05MWFiLTJkN2NkMDExZGI0NyIsInVuaXF1ZV9uYW1lIjoidi1mZWl4dWUxQG1pY3Jvc29mdC5jb20iLCJ1cG4iOiJ2LWZlaXh1ZTFAbWljcm9zb2Z0LmNvbSIsInV0aSI6IkQ0V0pJOTktVEVLa3ExWF9oelFWQUEiLCJ2ZXIiOiIxLjAiLCJ3aWRzIjpbImI3OWZiZjRkLTNlZjktNDY4OS04MTQzLTc2YjE5NGU4NTUwOSJdLCJ4bXNfY2MiOlsiQ1AxIl0sInhtc19pZHJlbCI6IjEgMiIsInhtc19zc20iOiIxIiwieG1zX3N0Ijp7InN1YiI6ImJadUNVREgwMzAyb3c5cjdTTnRFUXFWeElLaGlWVkw3cEhrajdXZVNuek0ifSwieG1zX3RjZHQiOjEyODkyNDE1NDd9.NhLyncS3KxwBsBYYONaeJnUa1sMzM8pJci3lmXh0h8HqU4Qi94hrb9Dh7v7cc15ydUW1_5KR7nWxVZ72e5BlLbiYw-BlgfCnSkR0DiSt950Z94M8OFJNzgiRLK1pUNVYukS1IY3vdmU4zPeJc_ZPWEGoAyNJEEaRwO7_XsGt5xw_iP0dIT-e9o5I4B22z37cbPju2cus5BMbdipntXs3KZ8JdEipGYxjAaBVZsw50mJzGxDAP7BnhUWbjwNXV2-veSVNn60OOej114BzSGvSlywkiKWS63rX3jHX_mhPFoZyLAFTnHrqcZSLyJvyAws6OLqSO-Ukfv6qtenvUMgJiQ");
	curl_easy_setopt(eh, CURLOPT_HTTPHEADER,headers) ;
	curl_multi_add_handle(cm, eh);
	(*left)++;
}
int createTask(char *tasklistid,int nrTask,char *beginDate)
{
	CURLM *cm;
	CURLMsg *msg;
	unsigned int transfers = 0;
	int msgs_left = -1;
	int left = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	cm = curl_multi_init();

	/* Limit the amount of simultaneous connections curl should allow: */
	curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX_PARALLEL);

	for(transfers = 0; transfers < MAX_PARALLEL && transfers < nrTask;
			transfers++)
		add_transfer(cm, transfers, &left);

	do {
		int still_alive = 1;
		curl_multi_perform(cm, &still_alive);

		/* !checksrc! disable EQUALSNULL 1 */
		while((msg = curl_multi_info_read(cm, &msgs_left)) != NULL) {
			if(msg->msg == CURLMSG_DONE) {
				char *url;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
				fprintf(stderr, "R: %d - %s <%s>\n",
						msg->data.result, curl_easy_strerror(msg->data.result), url);
				curl_multi_remove_handle(cm, e);
				curl_easy_cleanup(e);
				left--;
			}
			else {
				fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
			}
			if(transfers < nrTask)
				add_transfer(cm, transfers++, &left);
		}
		if(left)
			curl_multi_wait(cm, NULL, 0, 1000, NULL);

	} while(left);

	curl_multi_cleanup(cm);
	curl_global_cleanup();

	return EXIT_SUCCESS;
}

