/*
 ============================================================================
 Name        : avratech_final.c
 Author      : cg
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define MAX 12
#define BUFF 100

struct Date{
	unsigned int day;
	unsigned int month;
	unsigned int year;
};

struct Client{
	char first[MAX];
	char last[MAX];
	unsigned int ID;
	char phone[MAX];
	double debt;
	struct Date date;
	struct Client* next;
};

struct Client *head = NULL;

void readfile(char path[]);
void set(struct Client *head);
void check(struct Client *head,char data[][MAX]);
void bubbleSort(struct Client *start);
void swap(struct Client *a, struct Client *b);
void searchByDate(struct Client *node);
void searchByDebt(struct Client *node);
void searchByName(struct Client *node);
void printList(struct Client *node);
int exist(struct Client *node,unsigned int id);
int earlier(unsigned int t1[],unsigned int t2[]);
void quit(struct Client *node,char path[]);
void deleteList(struct Client** head_ref);
//================================
int firstOk(char value[]);
int lastOk(char value[]);
int idOK(char value[]);
int phoneOk(char value[]);
int debtOk(char value[]);
int dateOk(char value[]);
//================================
void insertAfter(struct Client* prev_node,char first[],char last[],unsigned int id,char phone[],double debt,unsigned int tm[]);
void update(struct Client *node,int num, char first[],char last[],char phone[],double debt,unsigned int tm[]);
void append(struct Client** head_ref, char first[],char last[],unsigned int id,char phone[],double debt,unsigned int tm[]);
//================================
int main(int argc, char *argv[]) {
	readfile(argv[1]);//path came from arguments' vector
	bubbleSort(head);//sorting the linked list we created
	printList(head);//print it
	//=====================
	char buffer[BUFF], *order;
	char *b = buffer;
	int i;
	size_t bufsize = BUFF;
	while(1)//until order to stop, taking orders from user
	{
		printf("\n===>");
		getline(&b,&bufsize,stdin);
		for(i=0;isspace(buffer[i]);i++);
		order = strtok(buffer," ");
		if (strcmp(order,"quit") == 0 || strcmp(order,"quit\n") == 0)  {
			break;
		}
		else if (strcmp(order,"set") == 0 || strcmp(order,"set\n") == 0)  {
			set(head);
			/*VERY IMPORTENT
			 * to insert new data  to create new client like this:
			 * John Smith 123456789 0548412345 345.67 12/12/2012
			 * Separated by blank space and tabs, with no words between
			 * I know this is not what you explain on the phone, but i don't have time to change it
			 * year with 4 digits*/
		}
		else if (strcmp(order,"name") == 0 || strcmp(order,"name\n") == 0)  {
			searchByName(head);
			/*VERY IMPORTENT
			 * the right way to insert input is like
			 * john
			 * I know this is not what you explain on the phone, but i don't have time to change it*/
		}
		else if (strcmp(order,"debt") == 0 || strcmp(order,"debt\n") == 0)  {
			searchByDebt(head);
			/*VERY IMPORTENT
			 * the right way to insert input is like
			 * > 350.78
			 * or
			 * < 432.98
			 * or
			 * = 876.4
			 * I know this is not what you explain on the phone, but i don't have time to change it*/
		}
		else if (strcmp(order,"date") == 0 || strcmp(order,"date\n") == 0)  {
			searchByDate(head);
			/* VERY IMPORTENT
			 * the right way to insert input is like
			 * > 23/7/1986
			 * or
			 * < 14/5/2020
			 * I know this is not what you explain on the phone, but i don't have time to change it*/
		}
		else if (strcmp(order,"print") == 0 || strcmp(order,"print\n") == 0)  {
			printList(head);
		}
	}
	quit(head,argv[1]);
	deleteList(&head);//free all dynamic allocated memory
	return EXIT_SUCCESS;
}
//=================================
void readfile(char path[])
{
	char tempfirst[MAX];
	char templast[MAX],t[MAX];
	unsigned int tempID,td[3];
	char tempPhone[MAX];
	double tempDebt;
	int tempok;
	FILE* fp = fopen(path, "r");
	if (!fp){
		printf("Can't open file\n");
		exit(1);
	}
	else {
		char buffer[1024];
		int row = 0;
		int column = 0;
		while (fgets(buffer,1024, fp)) {
			column = 0;
			row++;
			// To avoid printing of column names in file. can be changed according to need
			//if (row == 1)
			//    continue;
			// Splitting the data: from each column check if is ok, then put it in temp variable
			char* value = strtok(buffer, ", ");
			tempok = 0;
			while (value) {
				switch(column){
				// Column 1
				case 0:{
					int wrong = firstOk(value);
					if(wrong == 0){
						strcpy(tempfirst,value);
						tempok++;
					}
					break;
				}
				// Column 2
				case 1:{
					int wrong = lastOk(value);
					if(wrong == 0){
						strcpy(templast,value);
						tempok++;
					}
					break;
				}
				// Column 3
				case 2:{
					int wrong = idOK(value);
					if(wrong == 0){
						tempID = atoi(value);
						tempok++;
					}
					break;
				}
				// Column 4
				case 3:{
					int wrong = phoneOk(value);
					if(wrong == 0){
						strcpy(tempPhone,value);
						tempok++;
					}
					break;
				}
				// Column 5
				case 4:{
					int wrong = debtOk(value);
					char *ptr;
					if(wrong == 0){
						tempDebt = strtod(value,&ptr);
						tempok++;
					}
					break;
				}
				// Column 6
				case 5:{
					int i,wrong = dateOk(value);
					if(wrong==0){
						strcpy(t,value);
						td[0]= atoi(strtok(t,"/"));
						for(i=1;i<3;i++){
							td[i]=atoi(strtok(NULL,"/"));
						}
						tempok++;
					}
					break;
				}
				default:
					break;
				}
				value = strtok(NULL, ", ");
				column++;
			}
			if(tempok == 6){//if all columns in this row are ok then:
				int e = exist(head,tempID);
				if(e == -1){//if not - append to list
					append(&head,tempfirst,templast,tempID,tempPhone,tempDebt,td);
				}
				else//if this ID already, just update this node
					update(head,e,tempfirst,templast,tempPhone,tempDebt,td);
			}
		}
		// Close the file
		fclose(fp);
	}
}
//================================
void set(struct Client *head)//to insert new client
{
	/*VERY IMPORTENT
	 * to insert new data  to create new client like this:
	 * John Smith 123456789 0548412345 345.67 12/12/2012
	 * Separated by blank space and tabs, with no words between
	 * I know this is not what you explain on the phone, but i don't have time to change it
	 * year with 4 digits*/
	char buffer[BUFF],data[6][MAX];
	char *b = buffer;
	size_t bufsize = BUFF;
	int i,j,k;
	printf("please enter data for new client\n");
	getline(&b,&bufsize,stdin);
	for (i=0,k = 0; k < 6; k++) {
		for(;isspace(buffer[i]);i++);
		for(j=0;isalnum(buffer[i])|| buffer[i] =='.' || buffer[i] =='+'|| buffer[i] =='-' || buffer[i] =='/';i++,j++)
			data[k][j] = buffer[i];//here we separate the whole line to 6 different fields
		data[k][j] ='\0';
	}
	check(head,data);//there is different function for the rest
}
//================================
void check(struct Client *head,char data[][MAX])
{
	int i,ok,allok = 0;// check every field
	ok=firstOk(data[0]);
	if(ok ==0)
		allok++;
	ok=lastOk(data[1]);
	if(ok ==0)
		allok++;
	ok=idOK(data[2]);
	if(ok ==0)
		allok++;
	ok=phoneOk(data[3]);
	if(ok ==0)
		allok++;
	ok=debtOk(data[4]);
	if(ok ==0)
		allok++;
	ok=dateOk(data[5]);
	if(ok ==0)
		allok++;
	if(allok == 6){//if every field is ok
		char *ptr,t[MAX];
		unsigned int data2 = atoi(data[2]),data5[3];
		double data4 = strtod(data[4],&ptr);
		strcpy(t,data[5]);
		data5[0]= atoi(strtok(t,"/"));
		for(i=1;i<3;i++){
			data5[i]=atoi(strtok(NULL,"/"));
		}
		int e = exist(head,data2);//check if exist already
		if(e == -1){//if not, insert in the right place
			while (head != NULL)
			{
				if(head->next == NULL){//if we reach almost the end, this means that the place is in the end
					insertAfter(head,data[0],data[1],data2,data[3],data4,data5);
					printf("client %s %s added\n",data[0],data[1]);
					break;
				}
				else if(data4 < head->debt) {//if it is suppose to be the first
					insertAfter(head,data[0],data[1],data2,data[3],data4,data5);//we put it second
					swap(head,head->next);//the switch places
					printf("client %s %s added\n",data[0],data[1]);
					break;
				}
				else if(head->debt < data4 &&(data4 < head->next->debt|| head->next == NULL)){//if the place is in the middle
					insertAfter(head,data[0],data[1],data2,data[3],data4,data5);
					printf("client %s %s added\n",data[0],data[1]);
					break;
				}
				head = head->next;
			}
		}
		else{//if already exist just update
			update(head,e,data[0],data[1],data[3],data4,data5);
			bubbleSort(head);// after we update the debt we need to sort again
			printf("client %s %s exist and updated\n",data[0],data[1]);
		}
	}
	else{
		printf("we can't add this client. some of the data is wrong, please try again\n");
	}
}
//==============================
int dateOk(char value[])//check if date ok. we assume years between 1950 -> 2022 are right
{
	int i,leap,wrong = 0;
	int daytab[][13] ={//from the ansi c book
			{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
			{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
	unsigned int date[3];
	char t[MAX];
	if(strlen(value)<1){
		printf("missing Date\n");//if there is no input
		return 1;
	}
	for (i = 0; i < strlen(value)-1 && wrong == 0; i++) {
		if(!isdigit(value[i]) && value[i] != '/'){
			printf("Date can't have non digit characters\n");//if there is wrong characters
			wrong = 1;
		}
	}
	strcpy(t,value);
	date[0]= atoi(strtok(t,"/"));
	for(i=1;i<3;i++){
		date[i]=atoi(strtok(NULL,"/"));
	}
	if(date[2]>2022 || date[2]<1950){
		printf("year not make sense\n");//right years
		wrong = 1;
	}
	if(date[1]>12 || date[1]<1){
		printf("month not make sense\n");//only 12 month
		wrong = 1;
	}
	leap = (((date[2]%4 == 0) && (date[2]%100 != 0)) || (date[2]%400 == 0));
	if(date[0]>daytab[leap][date[1]] || date[0]<1){//each month has different number of days
		printf("day not make sense\n");
		wrong = 1;
	}
	return wrong;
}
//================================
int debtOk(char value[])
{
	int i,wrong =0;
	if(strlen(value)<1){//if there is no input
		printf("missing Debt\n");
		return 1;
	}
	for (i = 0; i < strlen(value) && wrong == 0; i++) {
		if(!isdigit(value[i]) && value[i] != '.' && value[i] != '-' && value[i] != '+'){
			printf("Debt can't have non digit characters\n");//if there is wrong characters
			wrong = 1;
		}
	}
	return wrong;
}
//================================
int phoneOk(char value[])
{
	int i,wrong =0;
	if(strlen(value)<1){
		printf("missing Debt\n");//if there is no input
		return 1;
	}
	for (i = 0; i < strlen(value) && wrong == 0; i++) {
		if(!isdigit(value[i])){
			printf("Phone can't have non digit characters\n");//if there is wrong characters
			wrong = 1;
		}
	}
	return wrong;
}
//===================================
int idOK(char value[])
{
	int i,wrong =0;
	if(strlen(value)<1){
		printf("missing ID\n");//if there is no input
		return 1;
	}
	if(strlen(value)!=9 && strlen(value)!=8){
		printf("missing ID\n");//if there is wrong number of characters
		wrong = 1;
	}
	for (i = 0; i < strlen(value) && wrong == 0; i++) {
		if(!isdigit(value[i])){
			printf("ID can't have non digit characters\n");//if there is wrong characters
			wrong = 1;
		}
	}
	return wrong;
}
//================================
int lastOk(char value[])
{
	int i,wrong =0;
	if(strlen(value)<1){
		printf("missing last name\n");//if there is no input
		return 1;
	}
	for (i = 0; i < strlen(value) && wrong == 0; i++) {
		if(!isalpha(value[i])){
			printf("last name can't have non alpha characters\n");//if there is wrong characters
			wrong = 1;
		}
	}
	return wrong;
}
//================================
int firstOk(char value[])
{
	int i,wrong =0;
	if(strlen(value)<1){
		printf("missing first name\n");//if there is no input
		return 1;
	}
	for (i = 0; i < strlen(value) && wrong == 0; i++) {
		if(!isalpha(value[i])){
			printf("first name can't have non alpha characters\n");//if there is wrong characters
			wrong = 1;
		}
	}
	return wrong;
}
//================================
/* Bubble sort the given linked list */
void bubbleSort(struct Client *start)
{
	int swapped;
	struct Client *ptr1;
	struct Client *lptr = NULL;

	/* Checking for empty list */
	if (start == NULL)
		return;

	do
	{
		swapped = 0;
		ptr1 = start;

		while (ptr1->next != lptr)
		{
			if (ptr1->debt > ptr1->next->debt)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
	while (swapped);
}
//=====================================
/* function to swap data of two nodes a and b*/
void swap(struct Client *a, struct Client *b)
{
	//int temp = a->data;
	char tempfirst[MAX];
	char templast[MAX];
	unsigned int tempID = a->ID,d = a->date.day,m = a->date.month,y = a->date.year;
	char tempPhone[MAX];
	double tempDebt = a->debt;
	strcpy(tempfirst,a->first);
	strcpy(templast,a->last);
	strcpy(tempPhone,a->phone);

	//a->data = b->data;
	strcpy(a->first,b->first);
	strcpy(a->last,b->last);
	a->ID = b->ID;
	strcpy(a->phone,b->phone);
	a->debt = b->debt;
	a->date.day = b->date.day;
	a->date.month = b->date.month;
	a->date.year = b->date.year;

	//b->data = temp;
	strcpy(b->first,tempfirst);
	strcpy(b->last,templast);
	b->ID = tempID;
	strcpy(b->phone,tempPhone);
	b->debt = tempDebt;
	b->date.day = d;
	b->date.month = m;
	b->date.year = y;
}
//=====================================
void searchByDate(struct Client *node)
{
	/* VERY IMPORTENT
	 * the right way to insert input is like
	 * > 23/7/1986
	 * or
	 * < 14/5/2020
	 * I know this is not what you explain on the phone, but i don't have time to change it*/
	char buffer[BUFF],sign,date[MAX];
	char *b = buffer;
	size_t bufsize = BUFF;
	int i,j,ok;
	unsigned int d1[3],d2[3];
	printf("please enter date\n");
	getline(&b,&bufsize,stdin);
	for(i=0;isspace(buffer[i]);i++);
	for(j=0;buffer[i] == '<' ||buffer[i] == '>' ||buffer[i] == '=' ;i++,j++)//find out if is before or after
		sign = buffer[i];
	for(;isspace(buffer[i]);i++);
	for(j=0;isdigit(buffer[i]) || buffer[i] =='/';i++,j++)
		date[j] = buffer[i];
	date[j] ='\0';
	ok = dateOk(date);//check if date ok
	if(ok == 1){
		printf("Illegal date\n");
		return;
	}
	else{
		d1[0]= atoi(strtok(date,"/"));
		for(i=1;i<3;i++){
			d1[i]=atoi(strtok(NULL,"/"));//extract the date from the string
		}
		while (node != NULL)
		{
			d2[0]= node->date.day;
			d2[1]= node->date.month;
			d2[2]= node->date.year;//extract the date from the node
			if(sign =='<' && earlier(d2,d1))//print only what before
				printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
			if(sign =='>' && earlier(d1,d2))//print only what after
				printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
			node = node->next;
		}
	}
}
//=====================================
void searchByDebt(struct Client *node)
{
	/*VERY IMPORTENT
	 * the right way to insert input is like
	 * > 350.78
	 * or
	 * < 432.98
	 * or
	 * = 876.4
	 * I know this is not what you explain on the phone, but i don't have time to change it*/
	char buffer[BUFF],sign,debt[MAX];
	char *b = buffer,*ptr;
	size_t bufsize = BUFF;
	int i,j;
	printf("please enter debt\n");
	getline(&b,&bufsize,stdin);
	for(i=0;isspace(buffer[i]);i++);
	for(j=0;buffer[i] == '<' ||buffer[i] == '>' ||buffer[i] == '=' ;i++,j++)
		sign = buffer[i];
	for(;isspace(buffer[i]);i++);
	for(j=0;isdigit(buffer[i]);i++,j++)
		debt[j] = buffer[i];
	debt[j] ='\0';
	double d = strtod(debt,&ptr);////extract the debt from the string
	while (node != NULL)
	{
		if(sign =='<' && node->debt < d)//print only what below
			printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
		if(sign =='>' && node->debt > d)//print only what above
			printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
		if(sign =='=' && node->debt == d)//print only what equal
			printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);

		node = node->next;
	}
}
//================================
void searchByName(struct Client *node)
{
	/*VERY IMPORTENT
	 * the right way to insert input is like
	 * john
	 * I know this is not what you explain on the phone, but i don't have time to change it*/
	char buffer[BUFF],name[MAX];
	char *b = buffer;
	size_t bufsize = BUFF;
	int i,j;
	printf("please enter name\n");
	getline(&b,&bufsize,stdin);
	for(i=0;isspace(buffer[i]);i++);
	for(j=0;isalpha(buffer[i]);i++,j++)
		name[j] = buffer[i];
	name[j] ='\0';//extract the name from the string
	while (node != NULL)//
	{
		if(strcmp(name,node->first)== 0 || strcmp(name,node->last)== 0)
			printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
		node = node->next;
	}
}
//================================
void printList(struct Client *node)
{
	while (node != NULL)//Traverse on the list and print it
	{
		printf(" %10s | %10s | %d | %10s | %f | %d/%d/%d \n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
		node = node->next;
	}
}
//================================
int exist(struct Client *node,unsigned int id)
{
	int i = 0;
	while(node != NULL)//Traverse on the list to check for the id. if exist return its location,else -1
	{
		if(node->ID == id)
			return i;
		node = node->next;
		i++;
	}
	return -1;
}
//================================
/* Given a node prev_node, insert a new node after the given
   prev_node */
void insertAfter(struct Client* prev_node,char first[],char last[],unsigned int id,char phone[],double debt,unsigned int tm[])
{
	/*1. check if the given prev_node is NULL */
	if (prev_node == NULL)
	{
		printf("the given previous node cannot be NULL");
		return;
	}

	/* 2. allocate new node */
	struct Client* new_node =(struct Client*) malloc(sizeof(struct Client));

	/* 3. put in the data  */
	//new_node->data  = new_data;
	strcpy(new_node->first,first);
	strcpy(new_node->last,last);
	new_node->ID = id;
	strcpy(new_node->phone,phone);
	new_node->debt = debt;
	new_node->date.day = tm[0];
	new_node->date.month = tm[1];
	new_node->date.year = tm[2];

	/* 4. Make next of new node as next of prev_node */
	new_node->next = prev_node->next;

	/* 5. move the next of prev_node as new_node */
	prev_node->next = new_node;
}
//================================
void update(struct Client *node,int num, char first[],char last[],char phone[],double debt,unsigned int tm[])
{
	//if already exist client with this ID we traverse to its location and update the data
	int i,c;
	unsigned int t[3];
	for (i = 0; i < num; i++) {//traverse
		node = node->next;
	}
	strcpy(node->first,first);
	strcpy(node->last,last);
	strcpy(node->phone,phone);
	node->debt += debt;//add debt
	t[0] = node->date.day;
	t[1] = node->date.month;
	t[2] = node->date.year;
	c = earlier(tm,t);
	if(c==1)//set the earlier date
	{
		node->date.day = tm[0];
		node->date.month = tm[1];
		node->date.year = tm[2];
	}
}
//================================
int earlier(unsigned int t1[],unsigned int t2[])//given to dates as unsigned int array, return 1 if t1 earlier then t2 else 0
{
	int i;
	/* if year is earlier no need to check month
	 * if month is earlier no need to check day
	 * if day is earlier - t1 is earlier the t2
	 * */
	for (i = 2; i >= 0; i--) {
		if(t1[i] < t2[i])
			return 1;
		if(t1[i] > t2[i])
			return 0;
	}
	return 0;
}
//================================
void append(struct Client** head_ref, char first[],char last[],unsigned int id,char phone[],double debt,unsigned int tm[])
{
	/* 1. allocate node */
	struct Client* new_node = (struct Client*) malloc(sizeof(struct Client));

	struct Client *lastNode = *head_ref;  /* used in step 5*/

	/* 2. put in the data  */
	//new_node->data  = new_data;
	strcpy(new_node->first,first);
	strcpy(new_node->last,last);
	new_node->ID = id;
	strcpy(new_node->phone,phone);
	new_node->debt = debt;
	new_node->date.day = tm[0];
	new_node->date.month = tm[1];
	new_node->date.year = tm[2];

	/* 3. This new node is going to be the last node, so make next of
          it as NULL*/
	new_node->next = NULL;

	/* 4. If the Linked List is empty, then make the new node as head */
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
		return;
	}

	/* 5. Else traverse till the last node */
	while (lastNode->next != NULL)
		lastNode = lastNode->next;

	/* 6. Change the next of last node */
	lastNode->next = new_node;
	return;
}
//================================
void quit(struct Client *node,char path[])//to close the program nicely
{
	FILE* fp = fopen(path, "w");
	if (!fp) {
		// Error in file opening
		printf("Can't open file\n");
		return ;
	}
	while (node != NULL)//write back to file
	{
		fprintf(fp,"%s, %s, %d, %s, %f, %d/%d/%d\n", node->first,node->last,node->ID,node->phone,node->debt,node->date.day,node->date.month,node->date.year);
		node = node->next;
	}
	fclose(fp);
	printf("\n\tgoodbye");
	exit(0);
}
//================================
/* Function to delete the entire linked list */
void deleteList(struct Client** head_ref)
{
	/* deref head_ref to get the real head */
	struct Client* current = *head_ref;
	struct Client* next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}

	/* deref head_ref to affect the real head back
      in the caller. */
	*head_ref = NULL;
}
