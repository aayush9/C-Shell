#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
int flags[152];


int is_folder(char *path){
	//Idea from https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
    struct stat status;
    stat(path, &status);
    return !S_ISREG(status.st_mode);
}
void delete_rec(char *path){
	if(chdir(path)) return;
	struct dirent *ptr;
	DIR *dir = opendir(".");
	for (;(ptr = readdir(dir))!=NULL;){
		if(!strcmp(ptr->d_name,"..") || !strcmp(ptr->d_name,"."))
			continue;
		if(is_folder(ptr->d_name))
			delete_rec(ptr->d_name);
		else unlink(ptr->d_name);
	}
	closedir(dir);
	chdir("..");
	rmdir(path);
}
int main(int argc, char *argv[]){
	int idx=1,cnt=1;
	while(idx<argc && argv[idx][0]=='-'){
		for(int i=1;i<strlen(argv[idx]);++i)
			flags[argv[idx][i]]=1;
		++idx;
	}
	for(int i =0;i<152;++i)
			if(flags[i]&& i!='r' && i!='d')
				printf("Invalid flag -%c, ignoring that...\n",i);
	for(;idx<argc;++idx){
		printf("Removing %s\n",argv[idx]);
		unlink(argv[idx]);
		if(flags['d']) rmdir(argv[idx]);
		if(flags['r']) delete_rec(argv[idx]);
	}
}