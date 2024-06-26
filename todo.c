#define _GNU_SOURCE
#include "todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>


#define MAXURLLEN 512 
#define CONTENTTYPE_JSON "Content-Type: application/json"
#define TASKLISTENDPOINT "/me/todo/lists"
//GRAPHAPIENDPOINT/APIVERSION
#define TASKENDPOINT "%s%s/me/todo/lists/%s/tasks"
//Microsoft limit the API requst rate, based on the test 3 is ableto submit the request sucessfull 6/28/2024
#define MAX_PARALLEL 3

struct cmd_s cmd;
struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t write_cb(char *data, size_t n, size_t l, void *userp);
static void parseJSON(char *pChar,struct todoList_s *pTodolist)
{
	json_object *root = json_tokener_parse(pChar);
	json_object *id = json_object_object_get(root, "id");
	json_object *displayName = json_object_object_get(root, "displayName");
	pTodolist->id=strdup(json_object_get_string(id));
	pTodolist->displayName=strdup(json_object_get_string(displayName));
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}
void createTaskList(char *name,char *pTokenHeader,struct todoList_s *pTodolist)
{
	struct MemoryStruct chunk;

	chunk.memory = malloc(1);  /* grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	char *endpoint=calloc(1,strlen(GRAPHAPIENDPOINT)+strlen(APIVERSION)+strlen(TASKLISTENDPOINT)+1);

	if(!endpoint)
		{
			perror("calloc()");
			exit(1);
		}

	endpoint=strcat(endpoint,GRAPHAPIENDPOINT);
	endpoint=strcat(endpoint,APIVERSION);
	endpoint=strcat(endpoint,TASKLISTENDPOINT);
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
	{		printf("\nsend create task request sucessfully!\n");

		if(cmd.printResponse) 
			printf("%s\n",chunk.memory);
		parseJSON(chunk.memory,pTodolist);
	}
	else
		printf("\ncurl_easy_perform is failed with error: %d\n",res);

}
static size_t write_cb(char *data, size_t n, size_t l, void *userp)
{
	/* take care of the data here, ignored in this example */
	(void)data;
	(void)userp;
	return n*l;
}

static void add_transfer(CURLM *cm, unsigned int i, int *left,char *listID,char *pTokenHeader)
{
	char endpoint[MAXURLLEN];
	char *pTaskJSONData=NULL;
	struct tm temp_tm={0};
	char *pTaskDate=malloc(10+17+1);
	temp_tm.tm_year=cmd.startDate->tm_year;
	temp_tm.tm_mon=cmd.startDate->tm_mon;
	temp_tm.tm_mday=cmd.startDate->tm_mday+i;
	mktime(&temp_tm);
	strftime(pTaskDate,10+17+1,"%Y-%m-%dT00:00:00.0000000",&temp_tm);
	asprintf(&pTaskJSONData,"{\"title\" : \"P%d~%d\",\"dueDateTime\":{\"dateTime\":\"%s\",\"timeZone\":\"Asia/Shanghai\"}}",i*cmd.range+1,(i+1)*cmd.range,pTaskDate);
	sprintf((char *)endpoint,TASKENDPOINT ,GRAPHAPIENDPOINT,APIVERSION,listID);

	CURL *eh = curl_easy_init();
	curl_easy_setopt(eh, CURLOPT_URL, endpoint);
	curl_easy_setopt(eh, CURLOPT_POSTFIELDS,pTaskJSONData); 
	if(!cmd.printResponse)
	{
		curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION,write_cb );
	}else
		printf("\n...%s...\n",pTaskDate);
	struct curl_slist* headers = NULL;
	headers =    curl_slist_append(headers,CONTENTTYPE_JSON );
	headers =  curl_slist_append(headers, pTokenHeader);
	curl_easy_setopt(eh, CURLOPT_HTTPHEADER,headers) ;
	curl_multi_add_handle(cm, eh);
	(*left)++;
}
int createTask(char *tasklistid,struct cmd_s *pCMD,char *pTokenHeader)
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

	for(transfers = 0; transfers < MAX_PARALLEL && transfers < pCMD->nums;
			transfers++)
		add_transfer(cm, transfers, &left,tasklistid,pTokenHeader);

	do {
		int still_alive = 1;
		curl_multi_perform(cm, &still_alive);

		/* !checksrc! disable EQUALSNULL 1 */
		while((msg = curl_multi_info_read(cm, &msgs_left)) != NULL) {
			if(msg->msg == CURLMSG_DONE) {
				char *url;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
				if(cmd.printResponse)
					fprintf(stderr, "R: %d - %s <%s>\n",msg->data.result, curl_easy_strerror(msg->data.result), url);
				curl_multi_remove_handle(cm, e);
				curl_easy_cleanup(e);
				left--;
			}
			else {
				fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
			}
			if(transfers < pCMD->nums)
				add_transfer(cm, transfers++, &left,tasklistid,pTokenHeader);
		}
		if(left)
			curl_multi_wait(cm, NULL, 0, 1000, NULL);

	} while(left);

	curl_multi_cleanup(cm);
	curl_global_cleanup();

	return EXIT_SUCCESS;
}

