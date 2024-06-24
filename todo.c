#include "todo.h"
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#define CONTENTTYPE_JSON "Content-Type: application/json"
//APIENDPOINT/APIVERSION
#define TASKENDPOINT "%s/%s/me/todo/lists/%s/tasks" 
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


static int parseJSON(char *pChar,struct todoList_s *pTodolist)
{
	json_object *root = json_tokener_parse(pChar);
	json_object *id = json_object_object_get(root, "id");
	json_object *displayName = json_object_object_get(root, "displayName");
	pTodolist->id=strdup(json_object_get_string(id));
	pTodolist->displayName=strdup(json_object_get_string(displayName));
//	memcpy(pTodolist->id,json_object_get_string(id),sizeof(todolist.id)/sizeof(todolist.id[0])-1);
//	memcpy(pTodolist->displayName,json_object_get_string(displayName),sizeof(todolist.displayName)/sizeof(todolist.displayName[0])-1);
	printf("%s\n",__func__);
	printf("id:%s\n",pTodolist->id);
	printf("displayName:%s\n",pTodolist->displayName);
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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
		parseJSON(chunk.memory,pTodolist);
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

static void add_transfer(CURLM *cm, unsigned int i, int *left,char *listID,char *pTokenHeader)
{
	char endpoint[MAXURLLEN];
	sprintf((char *)endpoint,TASKENDPOINT ,APIENDPOINT,APIVERSION,listID);
	static int printmsg=0;
	if(printmsg==0)
		printf("create todo task:\n%s\n",endpoint);
	printmsg++;
	
	CURL *eh = curl_easy_init();
	curl_easy_setopt(eh, CURLOPT_URL, endpoint);
	curl_easy_setopt(eh, CURLOPT_POSTFIELDS, "{\"title\" : \"new task\",\"dueDateTime\":{\"dateTime\":\"2024-06-21T00:00:00.0000000\",\"timeZone\":\"Asia/Shanghai\"}}");;
	struct curl_slist* headers = NULL;
	headers =    curl_slist_append(headers,CONTENTTYPE_JSON );
	headers =  curl_slist_append(headers, pTokenHeader);
	curl_easy_setopt(eh, CURLOPT_HTTPHEADER,headers) ;
	curl_multi_add_handle(cm, eh);
	(*left)++;
}
int createTask(char *tasklistid,int nrTask,char *beginDate,char *pTokenHeader)
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
				add_transfer(cm, transfers++, &left,tasklistid,pTokenHeader);
		}
		if(left)
			curl_multi_wait(cm, NULL, 0, 1000, NULL);

	} while(left);

	curl_multi_cleanup(cm);
	curl_global_cleanup();

	return EXIT_SUCCESS;
}

