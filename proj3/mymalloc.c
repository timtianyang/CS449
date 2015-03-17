#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
struct Node{
	struct Node* previous_node;
	struct Node* next_node;
	int size;
	int free;
	
};

struct Node *head;
struct Node *tail;

void *my_bestfit_malloc(int size){
	struct Node* node_addr;
	if(head==NULL){//the heap is empty, add a node
		head=(struct Node *)sbrk(size+sizeof(struct Node));//node start
		head->free=0;
		head->previous_node=NULL;
		head->next_node=NULL;
		head->size=size;
		tail=head;
		return (void*)head+sizeof(struct Node);//only return usable space
	}
	else{
		node_addr=find_best_fit(size);
		if(node_addr==NULL){//if cant find best fit, create new node/move brk up
			printf("creating a new node...\n");
			return (void*)new_node(size)+sizeof(struct Node);//only return usable space of the new node
		}
		else{//if it finds a best fit
			node_addr->free=0;//mark as not free
			return (void*)node_addr;
		}
	
	
	}
	
	
}
struct Node* new_node(int size){
	tail->next_node = (struct Node*)sbrk(sizeof(struct Node)+size); 	
	tail->next_node->previous_node=tail;
	tail->next_node->size=size;
	tail->next_node->next_node=NULL;//new tail
	tail->next_node->free=0;//new node is always not free
	tail=tail->next_node;//pass to the tail
	return tail;
}

struct Node* find_best_fit(int size){
	struct Node* current_search=head;
	int min_free_space=0x7fffffff;//max value of signed int
	struct Node* best_fit=NULL;
	while(current_search!=NULL){
		if((current_search->free==1)&&(current_search->size>=size)){//if node has enough free space
			if(current_search->size<min_free_space){
				min_free_space=current_search->size;//swap min value
				best_fit=current_search;
			}
			
			
		}
		current_search=current_search->next_node;
		
	}
	
	return best_fit;
	
}



void my_free(void *ptr){
	struct Node* node_to_free=(struct Node*)(ptr-sizeof(struct Node));
	int neighbours;
	if((node_to_free!=head)&&(node_to_free!=tail)){//if freeing a node in the middle
		printf("freeing the middle\n");
		neighbours=((node_to_free->previous_node->free)<<1)|(node_to_free->next_node->free);//00 01 10 11
		switch(neighbours){
			case 3://both are free
				node_to_free->previous_node->size=node_to_free->previous_node->size+node_to_free->size+node_to_free->next_node->size+2*sizeof(struct Node);
				node_to_free->previous_node->next_node=node_to_free->next_node->next_node;
			break;
			case 2://previous node is free
				printf("merge free previous\n");
				node_to_free->previous_node->size=node_to_free->previous_node->size+node_to_free->size+16;
				node_to_free->previous_node->next_node=node_to_free->next_node;
			break;
			case 1://next node is free
				node_to_free->size=node_to_free->size+node_to_free->next_node->size+16;
				node_to_free->next_node=node_to_free->next_node->next_node;
				node_to_free->free=1;
			break;
			case 0://none is free
				node_to_free->free=1;//only free itself
				printf("free itself\n");
			break;
		}
	}
	else{
		
		
	}
	
	

	
}


int main(){
	//init_list();
	void* addr1;void* addr2;void* addr3;void* addr4;
	printf("The bottom of heap is %d\n",sbrk(0));
	//initialize the first dummy node
	
	addr1=my_bestfit_malloc(10);
	printf("current addr is %d\n",addr1);
	printf("current brk is %d\n",sbrk(0));
	
	addr2=my_bestfit_malloc(10);
	printf("current addr is %d\n",addr2);
	printf("current brk is %d\n",sbrk(0));
	
	addr3=my_bestfit_malloc(10);
	printf("current addr is %d\n",addr3);
	printf("current brk is %d\n",sbrk(0));
	
	addr4=my_bestfit_malloc(10);
	printf("current addr is %d\n",addr4);
	printf("current brk is %d\n",sbrk(0));
	
	my_free(addr2);//free the second node
	my_free(addr3);//free the second node
	addr2=my_bestfit_malloc(10);
	
	printf("current addr is %d\n",addr2);
	printf("current brk is %d\n",sbrk(0));
	addr2=my_bestfit_malloc(10);
	
	printf("current addr is %d\n",addr2);
	printf("current brk is %d\n",sbrk(0));

	return 1;
}