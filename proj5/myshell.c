#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char userInput[200];
char *token;
const char space[] = " ";
char token1[200];char token6[200];
char token2[200];char token7[200];
char token3[200];char token8[200];
char token4[200];char token9[200];
char token5[200];char token10[200];
char *temparray[2000];
int token_num=0;//number of token read
char *tokenList[10]={token1,token2,token3,token4,token5,
token6,token7,token8,token9,token10};
int IO=0;//2 is append 1 is input 3 is write
FILE *fp;
char* filename;
void switchDir(char* dir){//function to handle cd command
	int status;
	char cwd[200];
	if(dir==NULL){
		printf("No such file or directory\n");
		return;
	}
	//dir[strlen(dir)-1]=0x00;//get rid off \n
	if(*dir=='/'){//absolute dir
		status=chdir((const char *)dir);
		if(status==-1){
			printf("No such file or directory\n");	
			}else{
			getcwd(cwd, 200);
			printf("Directory has been changed to: %s\n",cwd);	
		}
		
		}else{//relative dir
		getcwd(cwd, 200);
		
		strcat(cwd,"/");//appending the dest
		strcat(cwd,dir);
		
		status=chdir((const char *)cwd);
		if(status==-1){
			printf("No such file or directory\n");	
			}else{
			getcwd(cwd, 200);
			printf("Directory has been changed to: %s\n",cwd);	
		}
	}
	
}


void DoUNIXcommand(){
	if(fork()==0)
	{			
		(temparray[token_num])=NULL;
		
		if(IO==2){fp = freopen(filename, "a", stdout);}
		else if(IO==3){fp = freopen(filename, "w", stdout);}
		else if(IO==1){fp = freopen(filename, "r", stdin);}
		else{
			if(fp!=NULL){
				fclose(fp);
				}
			}

		execvp(temparray[0], temparray);
		//printf("child is done\n");
		
	}
	else
	{
		int status;
		wait(&status);
		//printf("Hi from the parent\n");
	}
}

int main(){
	char *tempStr;
	while(1){
		IO=0;
		token_num=0;
		printf("myshell $ ");
		fgets(userInput, 200, stdin);
		userInput[strlen(userInput)-1]=0x00;//get rid off \n
		if(strcmp(userInput,"exit")==0)//check for exit
		{			
			exit(0);
		}else {
			token= strtok(userInput, space);
			while(token!=NULL){

				if(strcmp(token,">>")==0){
					IO=2;//>> is detected					
				}
				else if(strcmp(token,"<")==0){
					IO=1;//< is detected
				}
				else if(strcmp(token,">")==0){
					IO=3;//>is detected
				}
				
				strncpy(tokenList[token_num++],token,200);				
				token= strtok(NULL, space);				
			}
			
			
			if(IO!=0){filename=tokenList[token_num-1];token_num=token_num-2;}
			
			//printf("%s is the first token\n",token1);
			if(strcmp(token1,"cd")==0){//handle cd				
				switchDir(token2);
			}
			else{//handle UNIX command
				memcpy(temparray, tokenList, sizeof(tokenList));//temp array with NULL
				DoUNIXcommand();				
			}
		}
	}
	return 0;
}
