/*input
cat -ns shell.c date.c
exit
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

char *history[100005], executables[1005];
int idx;
void process(char *s){
	char *args[1005];
	int len = 0, flags[152];
	memset(flags,0,sizeof(flags)); 
	char *word = strtok(s," ");
	for(;word!=NULL;len++){
		args[len] = (char *)malloc(strlen(word));
		strcpy(args[len],word);
		word = strtok(NULL," ");
	}
	args[len] = NULL;
	int id;
	for(id = 1;id<len && args[id][0]=='-';++id)
		for(int i=1;i<strlen(args[id]);++i)
			flags[args[id][i]]=1;
	if(!strcmp(args[0],"echo")){
		for(int i =0;i<152;++i)
			if(flags[i]&& i!='e' && i!='n')
				printf("Invalid flag -%c, ignoring that...\n",i);
		int balanced = 1;
		char *buffer;
		buffer = (char *) malloc(10005);
		int last_was_backslash = 0;
		for(int i=1+(args[1][0]=='-');i<len;++i){
			for(int j=0;j<strlen(args[i]);++j){
				if(flags['e']){
					if(!last_was_backslash && args[i][j]=='\\');
					else if(last_was_backslash){
						int slen = strlen(buffer);
						if(args[i][j] == '"') buffer[slen] = '"';
						else if(args[i][j] == 'n') buffer[slen] = '\n';
						else if(args[i][j] == 't') buffer[slen] = '\t';
						else buffer[slen] = args[i][j];
						buffer[slen+1] = '\0';
					}
					else{
						balanced^=(args[i][j]=='"');
						if(args[i][j]!='"'){
						int slen = strlen(buffer);
						buffer[slen] = args[i][j];
						buffer[slen+1] = '\0';}
					}
				}
				else{
					balanced^=(args[i][j]=='"');
					if(args[i][j]!='"'){
						int slen = strlen(buffer);
						buffer[slen] = args[i][j];
						buffer[slen+1] = '\0';
					}
				}
				last_was_backslash = (args[i][j]=='\\');
			}
		}
		while(!balanced){
			char inp[1005];
			printf("> ");
			fgets(inp,1005,stdin);
			inp[strlen(inp)-1]='\0';
			int slen = strlen(buffer);
			buffer[slen] = '\n';
			buffer[slen+1] = '\0';
			for(int j=0;j<strlen(inp);++j){
				if(flags['e']){
					if(!last_was_backslash && inp[j]=='\\');
					if(last_was_backslash){
						int slen = strlen(buffer);
						if(inp[j] == '"') buffer[slen] = '"';
						else if(inp[j] == 'n') buffer[slen] = '\n';
						else if(inp[j] == 't') buffer[slen] = '\t';
						else buffer[slen] = inp[j];
						buffer[slen+1] = '\0';
					}
					else{
						balanced^=(inp[j]=='"');
						if(inp[j]!='"'){
						int slen = strlen(buffer);
						buffer[slen] = inp[j];
						buffer[slen+1] = '\0';}
					}
				}
				else{
					balanced^=(inp[j]=='"');
					if(inp[j]!='"'){
						int slen = strlen(buffer);
						buffer[slen] = inp[j];
						buffer[slen+1] = '\0';
					}
				}
				last_was_backslash = (inp[j]=='\\');
			}
		}
		printf("%s",buffer);
		if(flags['n']==0) printf("\n");
	}
	else if(!strcmp(args[0],"cd")){
		for(int i =0;i<152;++i)
			if(flags[i])
				printf("Invalid flag -%c, ignoring that...\n",i);
		char *path;
		path = (char *) malloc(1005);
		char *s = args[1];
		if(len<2){
			printf("No directory given.\n");
			return;
		}
		if(*s == '~'){
			char *tmp = getenv("HOME");
			strcat(path,tmp);
			++s;
		}
		strcat(path,s);
		if(chdir(path))
			printf("%s: No such directory\n",path);
	}
	else if(!strcmp(args[0],"pwd")){
		for(int i =0;i<152;++i)
			if(flags[i])
				printf("Invalid flag -%c, ignoring that...\n",i);
		char dir[1005];
		getcwd(dir,1005);
		printf("%s\n",dir);
	}
	else if(!strcmp(args[0],"exit")){
		exit(0);
	}
	else if(!strcmp(args[0],"history")){
		for(int i =0;i<152;++i)
			if(flags[i]&& i!='c' && i!='d')
				printf("Invalid flag -%c, ignoring that...\n",i);
		if(flags['c']) idx=1;
		if(flags['d']){
			int pos = atoi(args[id]);
			if(pos<1 || pos>idx){
				printf("Invalid position to clear\n");
				return;
			}
			for(int i=pos;i<idx-1;++i)
				strcpy(history[i],history[i+1]);
			--idx;
		}
		for(int i=1;i<idx;++i)
			printf("%d : %s\n",i,history[i]);
	}
	else{
		char ex[1005];
		strcpy(ex,executables);
		strcat(ex,args[0]);
		if(fork()==0){
			int ret = execve(ex,args,NULL);
			if(ret) printf("Invalid Command!\n");
			exit(0);
		}
		else{
			wait(NULL);
		}
	}
}
int main(){
	getcwd(executables,1005);
	strcat(executables,"/");
	for(idx=1;;){
		char *s;
		s = (char *) malloc(1005);
		printf("Shell >> ");
		fgets(s,1005,stdin);
		if(strlen(s)==1) continue;
		s[strlen(s)-1]='\0';
		history[idx] = (char *) malloc(strlen(s));
			strcpy(history[idx],s);
		++idx;
		if(s[0]=='!'){
			int x=0;
			for(int i=1;i<strlen(s) && isdigit(s[i]);++i)
				x=x*10+(s[i]-'0');
			if(x<idx) process(history[x]);
			else printf("Invalid command number\n");
			history[idx] = (char *) malloc(strlen(s));
			strcpy(history[idx],history[x]);
		}
		else{
			process(s);
		}
	}
}