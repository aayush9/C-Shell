// Help from https://stackoverflow.com/questions/1442116/how-to-get-date-and-time-value-in-c-program

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

char *months[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char *day[]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
int len[]={31,28,31,30,31,30,31,31,30,31,30,31};
int flags[152];
int main(int argc, char * argv[]){
	int idx=1,cnt=1;
	while(idx<argc && argv[idx][0]=='-'){
		for(int i=1;i<strlen(argv[idx]);++i)
			flags[argv[idx][i]]=1;
		++idx;
	}
	for(int i =0;i<152;++i)
			if(flags[i]&& i!='u' && i!='r')
				printf("Invalid flag -%c, ignoring that...\n",i);
	time_t cur = time(NULL);
	struct tm date;
	if(flags['u']) date = *gmtime(&cur);
	else date  = *localtime(&cur);
	if(flags['r']){
		struct stat b;
		char t[100]="";
		if (!stat(argv[idx], &b))
			date = *localtime( &b.st_mtime);
		else {
			printf("Error displaying last modififed time.\n");
			return 0;
		}
	}

	int days = 365*date.tm_year;
	for(int i=0;i<date.tm_mon;days+=len[i++]);
	days+=date.tm_mday;
	days+=(date.tm_year/4 - ((date.tm_year+1900)/400-4));
	if(date.tm_year%4==0 && date.tm_year%400 && date.tm_mon>1) --days;
	if(!flags['u'])
		printf("%s %s\t%d %d:%d:%d %d\n",day[days%7], months[date.tm_mon], date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec, date.tm_year + 1900);
	else
		printf("%s %s\t%d %d:%d:%d UTC %d\n",day[days%7], months[date.tm_mon], date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec, date.tm_year + 1900);
}