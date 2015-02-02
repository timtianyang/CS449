//tic27@pitt.edu Tianyang Chen
//CS449 Proj1 Rock paper scissor
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(){	
	char user_input[30];//a buffer to hold user input
	int com_value; //hold a random value
	char* RPS[]={"rock","paper","scissors"};// an array to hold options
	int man_value; //0 rock, 1 paper, 2 scissors
	int temp=1; //keeps the loop running
	int man_score; //score for human
	int com_score; //score for computer
	
	srand((unsigned int)time(NULL));
	printf("Welcome to Rock, Paper, Scissors\n");
	
	printf("Would you like to play? ");
	scanf("%s",user_input);
	temp=strcmp("yes",user_input);
	if(temp!=0){
		return -1;
	}
	else{
		while(!temp){
			man_score=0;
			com_score=0;

			while(man_score!=3&&com_score!=3){//check winning condition	
				printf("What is your choice? ");
				scanf("%s",user_input);
				if(strcmp(RPS[0],user_input)==0){
					man_value=0; 	
				}
				else if(strcmp(RPS[1],user_input)==0){
					man_value=1;
				}
				else if(strcmp(RPS[2],user_input)==0){
					man_value=2;
				}
				else{
					printf("unknown input......exiting........\n");
					return -1;
				}
				com_value=rand()%3; //get between 0 - 2
			
				printf("computer chooses %s.",RPS[com_value]);
				printf("You choose %s.\n",RPS[man_value]);
				if(man_value==com_value){
					printf("It\'s a tie!\n");
				}
				else if(man_value-com_value==1){
					man_score++;
					printf("You win this game\n\n");
				}
				else if(com_value-man_value==1){
          		                com_score++;
			                printf("you lose this game!\n\n");
				 }
				else if(com_value-man_value==2){
					man_score++;
					printf("you win this game!\n\n");
				}
				else if(com_value-man_value==-2){
               		         	com_score++;
               		 	        printf("you lose this game!\n\n");
 				}
				printf("The score is now you: %d computer: %d\n",man_score,com_score);
			}
			printf("The game is over\n");
			printf("Would you like to play again? ");
			scanf("%s",user_input);
      	 	 	temp=strcmp("yes",user_input);
			if(temp!=0){
				 return 0;
			}			
		}
	
	}
	return 0;
}
