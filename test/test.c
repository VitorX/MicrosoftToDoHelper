#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <curl/curl.h>

int main(int argc, char *argv[]) {
//	getValueByName();   
	postTest();
	return 0;
}

void showJSONVersion()
{
	printf("Version: %s\n", json_c_version());
	printf("Version Number: %d\n", json_c_version_num());

}

void getValueByName()
{
	json_object *root = json_object_from_file("contact.json");
	if (!root)
		return 1;

	json_object *first_name = json_object_object_get(root, "firstName");
	json_object *last_name = json_object_object_get(root, "lastName");
	printf("%s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name));

	json_object_put(root);	
}

void postTest()
{
 
  CURL *curl;
  CURLcode res;
 
  /* In windows, this inits the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be an https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
 
    /* Perform the request, res gets the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}
