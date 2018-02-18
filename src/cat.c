#include <stdio.h>
#include <string.h>
int flags[152];
int main(int argc, char *argv[]){
	int idx=1,cnt=1;
	while(idx<argc && argv[idx][0]=='-'){
		for(int i=1;i<strlen(argv[idx]);++i)
			flags[argv[idx][i]]=1;
		++idx;
	}
	for(int i =0;i<152;++i)
			if(flags[i]&& i!='s' && i!='n')
				printf("Invalid flag -%c, ignoring that...\n",i);
	for(;idx<argc;++idx){
	FILE *file = fopen(argv[idx],"r");
	for(char c,prev='.';(c=getc(file))!=EOF;){
		if(!flags['s']){
			if(flags['n']==1 && (cnt==1 || prev=='\n'))
				printf("%d ",cnt++);
		}
		else{
			if(flags['n']==1 && ((cnt==1 || prev=='\n') && c!='\n'))
				printf("%d ", cnt++);
		}
		if(flags['s']==1 && prev=='\n'&&c=='\n');
		else printf("%c",c);
		prev=c;
	}
	fclose(file);
	}
}