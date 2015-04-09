//Tianyang Chen
//tic27@pitt.edu
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>

int fd;
char user_name[20];
char response[20];
char random_gen(){//because the way the driver is written, the file has tp be closed
	char c;
	unsigned int d;
	fd=open("/dev/dice",O_RDONLY);      //open the device driver
	d=read(fd,&c,1); // read from the device driver
	//printf("read %d from driver \n",d);

	close(fd);
	return c;
}
int point(){
	int rand1;
	int rand2;
	int sum;
	
	rand1=random_gen();
	rand2=random_gen();	
	sum=rand1+rand2;
	printf("you have rolled %d and %d = %d\n",rand1,rand2,sum);
	if(sum==7){
		printf("You Lose!\n");
		return 0;//user loses
	}
	else if(sum!=2&&sum!=3&&sum!=12&&sum!=11){
		printf("You Win!\n");
		return 1;//user wins
	}
	return 3;//retry;
}

int main(){
	unsigned char rand1=1;
	unsigned char rand2=2;
	unsigned char sum;
	printf("Welcome to Jon\'s Casino!\n");
	printf("Please enter your name: ");
	scanf("%s",user_name);
	
	while(1){
	printf("%s, would you like to Play or Quit?",user_name);
	scanf("%s",response);
	if(strncmp(response,"play",4)!=0){break;}//check user input

	srand ( time(NULL) );
	rand1=random_gen();//get random numbers
	rand2=random_gen();	
	sum=rand1+rand2;
	printf("you have rolled %d and %d = %d\n",rand1,rand2,sum);


	if(sum==2||sum==3||sum==12)
		{
			printf("You Lose!\n");
		}
	else if(sum==7||sum==11)
		{
			printf("You Win!\n");
		}
	else
		{
			while(point()==3){//wait for results
			}
		}
	}
	printf("Goodbye, %s\n",user_name);
	return 0;
}










