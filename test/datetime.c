#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
int main()
{
	test2();
}
void test2()
{

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *pTaskDate=malloc(10+17+1);
	int i=0;
	while(i++<30)
	{
		tm.tm_mday++;
		mktime(&tm);
		printf("\nyear:%d,mon:%d,day:%d\n",tm.tm_year,tm.tm_mon,tm.tm_mday);	
		strftime(pTaskDate,10+17+1,"%Y-%m-%dT00:00:00.0000000",&tm);
		printf("%s\n",pTaskDate);
	}
	//printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
void test1()
{
	struct tm tm;
	char buf[255];

	memset(&tm, 0, sizeof(tm));
	strptime("2001-11-12", "%Y-%m-%d", &tm);
	printf("%d",1900+tm.tm_year);
}
