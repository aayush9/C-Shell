#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int flags[152];
int main(int argc, char *argv[]){
	int idx=1,cnt=1;
	int permissions = 0777;
	while(idx<argc && argv[idx][0]=='-'){
		for(int i=1;i<strlen(argv[idx]);++i){
			flags[argv[idx][i]]=1;
			if(argv[idx][i]=='m')
				permissions = strtol(argv[idx+1], NULL, 8);
		}
		++idx;
	}
	for(int i =0;i<152;++i)
			if(flags[i]&& i!='p' && i!='m')
				printf("Invalid flag -%c, ignoring that...\n",i);
	umask(0);
	if(flags['m']) ++idx;
	for(;idx<argc;++idx){
		if(!flags['p']) mkdir(argv[idx],permissions);
		else{
			char *word = strtok(argv[idx],"/");
			for(;word!=NULL;){
				mkdir(word,permissions);
				chdir(word);
				word = strtok(NULL,"/");
			}
		}
	}
}