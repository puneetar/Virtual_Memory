/*
 * virtualmem.c
 *
 *  Created on: Nov 18, 2012
 *      Author: puneeet
 */



#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct node
{
	int val;
	int ref;
	struct node *next;
};
int algo_rep=0,optimal_rep=0;
int frames,algo,faults=0,counter=0,no_of_pages=0;
unsigned int algo_time=0,optimal_time,time1,time2;

struct timeval tv,tv1;
FILE *input_file=NULL;
char buff[10000];

struct node *head = NULL;
struct node *curr = NULL;
struct node *curr_my = NULL;


int page[10000];
struct node* create_list(int val,int ref)
{

	struct node *ptr = (struct node*)malloc(sizeof(struct node));
	if(NULL == ptr)
	{
		printf("\n Out of Memory \n");
		return NULL;
	}
	ptr->val = val;
	ptr->ref = ref;
	ptr->next = NULL;

	head = curr = curr_my = ptr;
	return ptr;
}

struct node* add_to_list(int val,int ref, bool add_to_end)
{
	if(NULL == head)
	{
		return (create_list(val,ref));
	}


	struct node *ptr = (struct node*)malloc(sizeof(struct node));
	if(NULL == ptr)
	{
		printf("\n Out of memory \n");
		return NULL;
	}
	ptr->val = val;
	ptr->ref = ref;
	ptr->next = NULL;

	if(add_to_end)
	{
		curr->next = ptr;
		curr = ptr;
	}
	else
	{
		ptr->next = head;
		head = ptr;
	}
	return ptr;
}

struct node* search_in_list_val(int val, struct node **prev)
{
	struct node *ptr = head;
	struct node *tmp = NULL;
	bool found = false;



	while(ptr != NULL)
	{
		if(ptr->val == val)
		{
			found = true;
			break;
		}
		else
		{
			tmp = ptr;
			ptr = ptr->next;
		}
	}

	if(true == found)
	{
		if(prev)
			*prev = tmp;
		return ptr;
	}
	else
	{

		return NULL;
	}
}

struct node* search_in_list_ref(int ref, struct node **prev)
{
	struct node *ptr = head;
	struct node *tmp = NULL;
	bool found = false;



	while(ptr != NULL)
	{
		if(ptr->ref == ref)
		{
			found = true;
			break;
		}
		else
		{
			tmp = ptr;
			ptr = ptr->next;
		}
	}

	if(true == found)
	{
		if(prev)
			*prev = tmp;
		return ptr;
	}
	else
	{

		return NULL;
	}
}

int delete_from_list(int val)
{
	struct node *prev = NULL;
	struct node *del = NULL;



	del = search_in_list_val(val,&prev);
	if(del == NULL)
	{
		return -1;
	}
	else
	{
		if(prev != NULL)
			prev->next = del->next;

		if(del == curr)
		{
			curr = prev;
		}
		else if(del == head)
		{
			head = del->next;
		}
	}

	free(del);
	del = NULL;

	return 0;
}



int replace_from_list_val(struct node *repl,int new_val,int new_ref)
{
	if(repl == NULL)
	{

		return -1;
	}
	else
	{
		repl->val=new_val;
		repl->ref=new_ref;
	}

	return 0;
}

int replace_from_list_ref(struct node *repl,int new_val,int new_ref)
{
	if(repl == NULL)
	{

		return -1;
	}
	else
	{
		repl->val=new_val;
		repl->ref=new_ref;
	}

	return 0;
}

void print_list(void)
{

}

void fifo(){
	int i=0,count=frames;
	struct node *ptr=NULL;
	//for(i=0;i<(sizeof(page)/sizeof(int));i++){
	for(i=0;i<no_of_pages;i++){
		if((ptr=search_in_list_val(page[i],NULL))==NULL){
			if(count>0){
				add_to_list(page[i],0,true);
				count--;
			}
			else{
				replace_from_list_val(curr_my,page[i],0);
				curr_my_next();
			}
			faults++;
		}
		else{

		}

		print_list();

	}

}

void lru_stack(){

	int i=0,count=frames;
	struct node *ptr=NULL;
	//for(i=0;i<(sizeof(page)/sizeof(int));i++){
	for(i=0;i<no_of_pages;i++){
		if((ptr=search_in_list_val(page[i],NULL))==NULL){
			if(count>0){
				add_to_list(page[i],0,true);
				count--;
			}
			else{
				add_to_list(page[i],0,true);
				delete_from_list(head->val);
			}
			faults++;
		}
		else{
			add_to_list(page[i],0,true);
			delete_from_list(ptr->val);
		}

		print_list();

	}

}

int optimal_get_reference(int val, int i){
	int j=0,flag=0;
	int ref=0;

	//for(j=i+1;j<(sizeof(page)/sizeof(int));j++){
	for(j=i+1;j<no_of_pages;j++){
		if(page[j]==val){
			ref=j+1;
			flag++;
			break;
		}
		else{

		}
	}

	if(flag==0){
		ref=9999999;

	}
	return ref;

}

void optimal_update_reference(int i){
	//************************UPDATE REFERENCE
	struct node *vict_head=head;
	int j=0,k=0;
	int flag=0;

	for(k=0;k<frames;k++){

		flag=0;
		//for(j=i+1;j<(sizeof(page)/sizeof(int));j++){
		for(j=i+1;j<no_of_pages;j++){
			if(page[j]==vict_head->val){
				vict_head->ref=j+1;
				flag++;
				break;
			}
			else{

			}
		}

		if(flag==0){
			vict_head->ref=9999999;

		}



		if(vict_head->next==NULL){
			vict_head=head;
		}
		else{
			vict_head=vict_head->next;
		}
	}

}

struct node* optimal_search_vict(int i){

	optimal_update_reference(i);

	//*********************FIND VICTIM

	struct node *vict_head=head;
	struct node *vict=vict_head;
	int max=0,l;

	for(l=0;l<frames;l++){
		if(vict_head->ref>max){
			max=vict_head->ref;
			vict=vict_head;
		}
		else{

		}

		if(vict_head->next==NULL){
			vict_head=head;
		}
		else{
			vict_head=vict_head->next;
		}

	}

	//	return vict;

	if(vict->ref==curr_my->ref){
		return curr_my;
	}
	else if(curr_my->ref>vict->ref){
		return curr_my;
	}
	else{
		while(true){
			curr_my_next();
			if(curr_my->ref>=vict->ref){
				printf("i found it");
				break;
			}
		}
		return curr_my;

	}

}

void optimal(){
	int i=0,count=frames;
	faults=0;
	struct node *ptr=NULL;


	for(i=0;i<no_of_pages;i++){

		if((ptr=search_in_list_val(page[i],NULL))==NULL){
			if(count>0){
				add_to_list(page[i],optimal_get_reference(page[i],i),true);
				count--;
			}
			else{

				replace_from_list_val(optimal_search_vict(i),page[i],optimal_get_reference(page[i],i));
				curr_my_next();
			}
			faults++;
		}
		else{

		}

		print_list();


	}
}

int lfu_get_reference(int val, int i){
	int j=0,flag=0;
	int ref=0;

	for(j=0;j<i+1;j++){
		if(page[j]==val){

			flag++;
			//	break;
		}
		else{

		}
	}

	if(flag==0){
		ref=0;

	}
	else{
		ref=flag;
	}


	return ref;

}

void lfu_update_reference(int i){
	//************************UPDATE REFERENCE
	struct node *vict_head=head;
	int j=0,k=0;
	int flag=0;

	for(k=0;k<frames;k++){

		flag=0;
		for(j=0;j<i+1;j++){
			if(page[j]==vict_head->val){

				flag++;
				//	printf("value of flag for [%d]= %d",vict-head->val,flag);
				//	break;
			}
			else{

			}
		}

		if(flag==0){
			vict_head->ref=0;

		}
		else{
			vict_head->ref=flag;
		}



		if(vict_head->next==NULL){
			vict_head=head;
		}
		else{
			vict_head=vict_head->next;
		}
	}

}



struct node* lfu_search_vict(int i){

	lfu_update_reference(i);

	//*********************FIND VICTIM

	struct node *vict_head=head;
	struct node *vict=vict_head;
	int min=1000000,l;

	for(l=0;l<frames;l++){
		if(vict_head->ref<min){
			min=vict_head->ref;
			vict=vict_head;
		}
		else{

		}

		if(vict_head->next==NULL){
			vict_head=head;
		}
		else{
			vict_head=vict_head->next;
		}

	}


	if(vict->ref==curr_my->ref){
		return curr_my;
	}
	else if(curr_my->ref<vict->ref){
		return curr_my;
	}
	else{
		while(true){
			curr_my_next();

			if(curr_my->ref<=vict->ref){

				break;
			}
		}
		return curr_my;

	}




}


void lfu(){
	int i=0,count=frames;
	faults=0;
	struct node *ptr=NULL;

	//for(i=0;i<(sizeof(page)/sizeof(int));i++){
	for(i=0;i<no_of_pages;i++){
		//	update_reference(i);
		if((ptr=search_in_list_val(page[i],NULL))==NULL){
			if(count>0){
				add_to_list(page[i],lfu_get_reference(page[i],i),true);
				count--;
			}
			else{

				replace_from_list_val(lfu_search_vict(i),page[i],lfu_get_reference(page[i],i));
				curr_my_next();
			}
			faults++;

		}
		else{

			ptr->ref=ptr->ref+1;
		}

		print_list();


	}
}


int lruref8_get_reference(int val, int i){
	int j=0,flag=0;
	int ref=0;

	for(j=0;j<i+1;j++){
		if(page[j]==val){
			ref=ref>>1;
			ref=ref+128;
			//	break;
		}
		else{
			ref=ref>>1;
		}
	}

	return ref;

}

void lruref8_update_reference(int i){
	//************************UPDATE REFERENCE
	struct node *vict_head=head;
	int j=0,k=0;
	int flag=0;

	for(k=0;k<frames;k++){

		flag=0;
		for(j=0;j<i+1;j++){
			if(page[j]==vict_head->val){
				vict_head->ref=vict_head->ref>>1;
				vict_head->ref=vict_head->ref+128;
			}
			else{
				vict_head->ref=vict_head->ref>>1;
			}
		}



		if(vict_head->next==NULL){
			vict_head=head;
		}
		else{
			vict_head=vict_head->next;
		}
	}

}



struct node* lruref8_search_vict(int i){

	lruref8_update_reference(i);

	//*********************FIND VICTIM

	struct node *vict_head=head;
	struct node *vict=vict_head;
	int min=1000000,l;

	for(l=0;l<frames;l++){
		if(vict_head->ref<min){
			min=vict_head->ref;
			vict=vict_head;
		}
		else{

		}

		if(vict_head->next==NULL){
			vict_head=head;
		}
		else{
			vict_head=vict_head->next;
		}

	}



	if(vict->ref==curr_my->ref){
		return curr_my;
	}
	else if(curr_my->ref<vict->ref){
		return curr_my;
	}
	else{
		while(true){
			curr_my_next();

			if(curr_my->ref<=vict->ref){
				printf("i found it");
				break;
			}
		}
		return curr_my;

	}




}


void lruref8(){
	int i=0,count=frames;
	faults=0;
	struct node *ptr=NULL;

	//for(i=0;i<(sizeof(page)/sizeof(int));i++){
	for(i=0;i<no_of_pages;i++){
		//	update_reference(i);
		if((ptr=search_in_list_val(page[i],NULL))==NULL){
			if(count>0){
				add_to_list(page[i],lruref8_get_reference(page[i],i),true);
				count--;
			}
			else{

				replace_from_list_val(lruref8_search_vict(i),page[i],lruref8_get_reference(page[i],i));
				curr_my_next();
			}
			faults++;

		}
		else{

			ptr->ref=ptr->ref+1;

		}

		print_list();


	}
}

int lruclock_get_reference(int val, int i){

	return 0;

}




struct node* lruclock_search_vict(int i){

	//	lruclock_update_reference(i);

	//*********************FIND VICTIM

	if(curr_my->ref==0){

		return curr_my;
	}
	else{

		curr_my->ref=0;
		while(true){

			curr_my_next();

			if(curr_my->ref==0){

				break;
			}
			else{
				curr_my->ref=0;
			}

		}
		return curr_my;
	}
}


void lruclock(){
	int i=0,count=frames;
	faults=0;
	struct node *ptr=NULL;

	//for(i=0;i<(sizeof(page)/sizeof(int));i++){
	for(i=0;i<no_of_pages;i++){
		//	update_reference(i);
		if((ptr=search_in_list_val(page[i],NULL))==NULL){
			if(count>0){
				add_to_list(page[i],lruclock_get_reference(page[i],i),true);
				count--;
			}
			else{

				replace_from_list_val(lruclock_search_vict(i),page[i],lruclock_get_reference(page[i],i));
				curr_my_next();
			}
			faults++;

		}
		else{

			ptr->ref=1;

		}

		print_list();


	}
}




void curr_my_next(){
	if(curr_my->next==NULL){
		curr_my=head;
	}
	else{
		curr_my=curr_my->next;
	}
}





int main(int argc, char **argv)
{
	frames=5;
	algo=1;

	int i;
	for(i=1;i<argc;i++)
	{
		if(!strcmp(argv[i],"-h"))
		{
			printf("\n\tvirtualmem [−h] [-f available-frames] [−r replacement-policy] [−i input_file]");
			printf("\n\n\tOPTIONS :");
			printf("\n\n\t−h : Print a usage summary with all options and exit.");
			printf("\n\t−f : Set the number of available frames. By default it is 5.");
			printf("\n\t−r : Set the page replacement policy. It can be either\n\t\tFIFO (First-in-first-out)\n\t\tLFU (Least-frequently-used)\n\t\tLRU-STACK (Least-recently-used stack implementation)\n\t\tLRU-CLOCK ((Least-recently-used clock implementation –second chance alg.).\n\t\tLRU-REF8 (Least-recently-used Reference-bit Implementation, using 8 reference bits)\n\t\tThe default is FIFO.");
			printf("\n\t−i : Read the page reference sequence from a specified file. If not given, the sequence should be read from STDIN (ended with ENTER).");
			printf("\n");
			exit(0);
		}

		if(!strcmp(argv[i],"-f"))
		{
			frames=atoi(argv[i+1]);
			if(frames==0){
				printf("\nINVALID NUMBER OF FRAMES");
				exit(EXIT_FAILURE);
			}
			else{
				printf("\nNo.of Frames: %d",frames);
			}


		}

		if(!strcmp(argv[i],"-r"))
		{
			if(!strcmp(argv[i+1],"FIFO")){
				algo = 1;
			}
			else if(!strcmp(argv[i+1],"LFU")){
				algo = 2;
			}
			else if(!strcmp(argv[i+1],"LRU-STACK")){
				algo = 3;
			}
			else if(!strcmp(argv[i+1],"LRU-CLOCK")){
				algo = 4;
			}
			else if(!strcmp(argv[i+1],"LRU-REF8")){
				algo = 5;
			}
			else{
				printf("\nINVALID ALGORITHM");
				exit(EXIT_FAILURE);

			}

			printf("\nNo.of algo: %d",algo);
		}

		if(!strcmp(argv[i],"-i"))
		{
			if ((input_file = fopen(argv[i+1], "r")) == NULL){
				printf("Cannot open %s for reading.\n", argv[i+1]);

			}
			else {
				printf("\n");

				char ch;
				while((ch=fgetc(input_file))!='\n'){
					if(ch=='\t')
						ch=32;
					if((ch<48 || ch>57) && ch!=32){
						printf("\nPAGE REFERENCE IS INVALD\n");
						fclose(input_file);
						exit(0);
					}
					buff[counter++]=ch;
				}

				buff[counter]='\0';
				fclose(input_file);

				counter=0;
				char *tok;
				tok=strtok(buff," ");
				while(tok != NULL){
					page[counter++]=atoi(tok);
					no_of_pages++;
					tok=strtok(NULL," ");
				}


				printpages();
			}

		}
	}

	if(input_file==NULL){
		printf("\n\nEnter the PAGE REFERENCE SEQUENCE :\n");

		char ch;
		while((ch=getchar())!='\n'){
			if(ch=='\t')
				ch=32;
			if((ch<48 || ch>57) && ch!=32){
				printf("\nPAGE REFERENCE IS INVALD\n");
				exit(0);
			}
			buff[counter++]=ch;
		}

		buff[counter]='\0';

		counter=0;
		char *tok;
		tok=strtok(buff," ");
		while(tok != NULL){
			page[counter++]=atoi(tok);
			no_of_pages++;
			tok=strtok(NULL," ");
		}

		printpages();
	}





	if(algo==1){
		gettimeofday(&tv, NULL);
		fifo();
		gettimeofday(&tv1, NULL);
		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		algo_time= time2-time1;
		//    printf("%u",algo_time);

		algo_rep=(faults-frames);
		head=curr=curr_my=NULL;
		faults=0;

		gettimeofday(&tv, NULL);
		optimal();
		gettimeofday(&tv1, NULL);

		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		optimal_time= time2-time1;
		//		    printf("%u",optimal_time);


		optimal_rep=(faults-frames);

		printf("\n\n# of page replacements with FIFO 	: %d",algo_rep);
		printf("\n# of page replacements with Optimal	: %d",optimal_rep);
		printf("\n%% page replacement penalty using FIFO: %3.1f",((((float)algo_rep-(float)optimal_rep)/(float)optimal_rep)*(float)100));

		printf("\n");printf("Total time to run FIFO algorithm 	:%u",algo_time);
		printf("\n");printf("Total time to run Optimal algorithm	:%u",optimal_time);
		printf("\n");printf("FIFO is %3.1f%% faster than Optimal algorithm",((((float)algo_time-(float)optimal_time)/(float)optimal_time)*(float)100));

	}
	if(algo==2){
		gettimeofday(&tv, NULL);
		lfu();
		gettimeofday(&tv1, NULL);
		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		algo_time= time2-time1;
		//    printf("%u",algo_time);

		algo_rep=(faults-frames);
		head=curr=curr_my=NULL;
		faults=0;

		gettimeofday(&tv, NULL);
		optimal();
		gettimeofday(&tv1, NULL);

		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		optimal_time= time2-time1;
		//		    printf("%u",optimal_time);


		optimal_rep=(faults-frames);

		printf("\n\n# of page replacements with LFU 	: %d",algo_rep);
		printf("\n# of page replacements with Optimal	: %d",optimal_rep);
		printf("\n%% page replacement penalty using LFU: %3.1f",((((float)algo_rep-(float)optimal_rep)/(float)optimal_rep)*(float)100));

		printf("\n");printf("Total time to run LFU algorithm 	:%u",algo_time);
		printf("\n");printf("Total time to run Optimal algorithm	:%u",optimal_time);
		printf("\n");printf("LFU is %3.1f%% faster than Optimal algorithm",((((float)algo_time-(float)optimal_time)/(float)optimal_time)*(float)100));


	}
	if(algo==3){
		gettimeofday(&tv, NULL);
		lru_stack();
		gettimeofday(&tv1, NULL);
		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		algo_time= time2-time1;
		//    printf("%u",algo_time);

		algo_rep=(faults-frames);
		head=curr=curr_my=NULL;
		faults=0;

		gettimeofday(&tv, NULL);
		optimal();
		gettimeofday(&tv1, NULL);

		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		optimal_time= time2-time1;
		//		    printf("%u",optimal_time);


		optimal_rep=(faults-frames);
		printf("\n\n# of page replacements with LRU-STACK 	: %d",algo_rep);
		printf("\n# of page replacements with Optimal	: %d",optimal_rep);
		printf("\n%% page replacement penalty using LRU-STACK: %3.1f",((((float)algo_rep-(float)optimal_rep)/(float)optimal_rep)*(float)100));
		printf("\n");
		printf("Total time to run LRU-STACK algorithm 	:%u",algo_time);
		printf("\n");
		printf("Total time to run Optimal algorithm	:%u",optimal_time);

		printf("\n");printf("LRU-STACK is %3.1f%% faster than Optimal algorithm",((((float)algo_time-(float)optimal_time)/(float)optimal_time)*(float)100));


	}

	if(algo==4){
		gettimeofday(&tv, NULL);
		lruclock();
		gettimeofday(&tv1, NULL);
		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		algo_time= time2-time1;
		//    printf("%u",algo_time);

		algo_rep=(faults-frames);
		head=curr=curr_my=NULL;
		faults=0;

		gettimeofday(&tv, NULL);
		optimal();
		gettimeofday(&tv1, NULL);

		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		optimal_time= time2-time1;
		//		    printf("%u",optimal_time);


		optimal_rep=(faults-frames);
		printf("\n\n# of page replacements with LRU-STACK 	: %d",algo_rep);
		printf("\n# of page replacements with Optimal	: %d",optimal_rep);
		printf("\n%% page replacement penalty using LRU-STACK: %3.1f",((((float)algo_rep-(float)optimal_rep)/(float)optimal_rep)*(float)100));
		printf("\n");
		printf("Total time to run LRU-STACK algorithm 	:%u",algo_time);
		printf("\n");
		printf("Total time to run Optimal algorithm	:%u",optimal_time);
		printf("\n");
		printf("LRU-STACK is %3.1f%% faster than Optimal algorithm",((((float)algo_time-(float)optimal_time)/(float)optimal_time)*(float)100));

	}

	if(algo==5){
		gettimeofday(&tv, NULL);
		lruref8();
		gettimeofday(&tv1, NULL);
		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		algo_time= time2-time1;
		//    printf("%u",algo_time);

		algo_rep=(faults-frames);
		head=curr=curr_my=NULL;
		faults=0;

		gettimeofday(&tv, NULL);
		optimal();
		gettimeofday(&tv1, NULL);

		time1=((uint)tv.tv_sec*1000000)+(uint)tv.tv_usec;

		time2=((uint)tv1.tv_sec*1000000)+(uint)tv1.tv_usec;

		optimal_time= time2-time1;


		optimal_rep=(faults-frames);
		printf("\n\n# of page replacements with LRU-REF8 	: %d",algo_rep);
		printf("\n# of page replacements with Optimal	: %d",optimal_rep);
		printf("\n%% page replacement penalty using LRU-REF8: %3.1f",((((float)algo_rep-(float)optimal_rep)/(float)optimal_rep)*(float)100));
printf("\n");
		printf("Total time to run LRU-REF8 algorithm 	:%u",algo_time);
		printf("\n");
		printf("Total time to run Optimal algorithm	:%u",optimal_time);
		printf("\n");
		printf("LRU-REF8 is %3.1f%% faster than Optimal algorithm",((((float)algo_time-(float)optimal_time)/(float)optimal_time)*(float)100));


	}




	return 0;
}


void printpages(){


}


