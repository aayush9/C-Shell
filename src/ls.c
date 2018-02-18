#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
int flags[152];
static int cmp ( const void * a, const void * b){
	return strcmp(*(const char **)a,*(const char **)b);
}
int main(int argc, char *argv[]){
	int idx=1,cnt=1;
	while(idx<argc && argv[idx][0]=='-'){
		for(int i=1;i<strlen(argv[idx]);++i)
			flags[argv[idx][i]]=1;
		++idx;
	}
	for(int i =0;i<152;++i)
			if(flags[i]&& i!='U' && i!='a')
				printf("Invalid flag -%c, ignoring that...\n",i);
	// Idea from https://stackoverflow.com/questions/39717948/implementation-of-ls-command-in-c
	struct dirent *ptr;
	DIR *dir;
	if(idx >= argc) dir = opendir("./"); 
	else dir = opendir(argv[idx]); 
	char *arr[1005];
	int i;
	for (i=0; (ptr = readdir(dir))!=NULL; ++i){
		arr[i]=(char *) malloc(strlen(ptr->d_name));
		strcpy(arr[i],ptr->d_name);
		if(arr[i][0]=='.' && !flags['a']) --i;
	}
	if(!flags['U']) qsort(arr,i,sizeof(const char *),cmp);
	for(int j=0;j<i;++j) 
		printf("%s\n",arr[j]);
	closedir(dir);
}