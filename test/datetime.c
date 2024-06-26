#include <stdio.h>
#include <time.h>

int main()
{
	test2();
}
void test2()
{

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
void test1()
{
	struct tm tm;
	char buf[255];

	memset(&tm, 0, sizeof(tm));
	strptime("2001-11-12", "%Y-%m-%d", &tm);
	printf("%d",1900+tm.tm_year);
}
