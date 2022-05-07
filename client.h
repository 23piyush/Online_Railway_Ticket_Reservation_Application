#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"struc.h"

void mainMenu(int);
void displayMenu(int,int);
void admin_functions(int);
void signUp(int, int);
void login(int);
void adminLogin(int);
void add_train(int);
void view_train(int);
void book_ticket(int,int);
void view_booking(int);
void cancel_ticket(int);
void view_userList(int);
void update_booking(int);

//-----------------------------------MainMenu------------------------------------//
void mainMenu(int sd)
{
	int choice;

	printf("\n1.No account? Signup\n2.INDIVIDUAL ACCOUNT LOGIN\n3.ADMIN LOGIN\n4.AGENT ACCOUNT LOGIN\n5.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));

	switch(choice)
	{
		case 1: signUp(sd,1);
				break;		
		case 2: login(sd);
				break;
		case 3: adminLogin(sd);
				break;
		//case 4: agentAccountLogin(sd);
		//		break;
		case 5: exit(0);
				break;

	}
}

//-----------------------------------SignUp------------------------------------//
void signUp(int sd, int flag){
	int account_id;
	char uname[20],pass[20], buffer[30];
	struct database db;

	printf("Enter username: ");
	scanf(" %s",uname);
	printf("Enter password: ");
	scanf(" %s",pass);

	write(sd, uname,sizeof(uname));
	write(sd, pass, sizeof(pass));

	read(sd, buffer, sizeof(buffer));
	printf("%s", buffer);
	if(flag==1)
		mainMenu(sd);
	else
		admin_functions(sd);
}

//-----------------------------------Login------------------------------------//
void login(int sd)
{
	char buffera[20];
	char uname[20],pass[20];
	int flag,id;

	printf("Enter username: ");
	scanf(" %s",uname);
	printf("Enter password: ");
	scanf(" %s",pass);

	write(sd, uname,sizeof(uname));
	write(sd, pass, sizeof(pass));

	read(sd, &flag, sizeof(flag));
	read(sd, &id, sizeof(id));
	read(sd, buffera, sizeof(buffera));
	printf("%s\n",buffera);

	if(flag==1)
		displayMenu(sd,id);
	else
		mainMenu(sd);

}

void displayMenu(int sd, int id)
{
	int choice;
	printf("1.View Train Details\n2.Book Tickets\n3.View Previous Bookings\n4.Update Booking\n5.Cancel Ticket\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));

	switch(choice)
	{
		
		case 1: view_train(sd); 
			displayMenu(sd,id); 
				break;
		case 2: book_ticket(sd,id);
			displayMenu(sd,id);
				break;
		case 3: view_booking(sd);
			displayMenu(sd, id);
				break;
		case 4: update_booking(sd);
			displayMenu(sd,id);
				break;
		case 5: cancel_ticket(sd);
			displayMenu(sd, id);
		            break;
							
		default: printf("Enter proper choice");
				break;

	}
}


//--------------------------------------------Admin functions----------------------------------------//

void adminLogin(int sd)
{
	char uname[20], pass[20], buffer[20];
	int flag;
	
	printf("\nEnter the admin username: ");
	scanf(" %s",uname);
	printf("\nEnter admin password: ");
	scanf(" %s",pass);
	
	write(sd, uname, sizeof(uname));
	write(sd, pass, sizeof(pass));
	read(sd, &flag, sizeof(flag));
	read(sd, buffer, sizeof(buffer));
	
	printf("%s",buffer);
	if(flag == 1)
		admin_functions(sd);
	else
		mainMenu(sd);
}


void admin_functions(int sd)
{
	int choice;
	
	printf("\n1.Add Train Deatils\n2.View Train Details\n3.View Users List\n3.Search an Account\n4.Add Joint Account\n5.Delete Joint Account\n6.Search Joint Account\n7.Exit\n");
	
	printf("Enter your choice: ");
	scanf("%d", &choice);
	
	char buffer[46], uname[20], pass[20], uname1[20], uname2[20];
	write(sd, &choice, sizeof(choice));
	
	switch(choice)
	{
		case 1:add_train(sd);
			break;
		case 2:view_train(sd);
			break;
		case 3: view_userList(sd);
			break;
		/*case 3:printf("\nEnter username to search: ");
			scanf(" %s", uname);
			write(sd, uname, sizeof(uname));
			read(sd, &buffer, sizeof(buffer));
			printf(" %s", buffer);
			admin_functions(sd);
			break;
		case 4:newJointAccount(sd,0);
			break;
		case 5:deleteJointAccount(sd);
			break;
		case 6:printf("\nEnter username 1 to search: ");
			scanf(" %s", uname1);
			printf("\nEnter username 2 to search: ");
			scanf(" %s", uname2);
			write(sd, uname1, sizeof(uname1));
			write(sd, uname2, sizeof(uname2));
			read(sd, &buffer, sizeof(buffer));
			printf("%s", buffer);
			admin_functions(sd);
			break;*/
			
		default:mainMenu(sd);
	}
}

void add_train(int sd){
	int valid = 0;
	char tname[50];
	printf("\n\tEnter train name: ");
	scanf("%s",tname);
	write(sd, &tname, sizeof(tname));
	read(sd,&valid,sizeof(valid));	
	if(valid)
		printf("\n\tTrain added successfully\n");
		
	admin_functions(sd);

}

void view_userList(int sd){
	int no_of_users;
	int id,type;
	char uname[50];
	read(sd,&no_of_users,sizeof(no_of_users));

	printf("\tU_id\tU_name\n");
	while(no_of_users--){
		read(sd,&id,sizeof(id));
		read(sd,&uname,sizeof(uname));
		
		
		if(strcmp(uname, "deleted")!=0)
			printf("\t%d\t%s\n",id,uname);
	}
}

//---------------------------User----------------------------------------------//
void view_train(int sd){
	int no_of_trains;
	int tno;
	char tname[50];
	int tseats;
	int aseats;
	read(sd,&no_of_trains,sizeof(no_of_trains));

	printf("\tT_no\tT_name\tT_seats\tA_seats\n");
	while(no_of_trains--){
		read(sd,&tno,sizeof(tno));
		read(sd,&tname,sizeof(tname));
		read(sd,&tseats,sizeof(tseats));
		read(sd,&aseats,sizeof(aseats));
		
		printf("\t%d\t%s\t%d\t%d\n",tno,tname,tseats,aseats);
	}
}

void book_ticket(int sd,int id){
	int valid = 0;
	int tid,seats;
	//crud_train(sock,view);
	printf("\n\tEnter the train number you want to book: ");
	scanf("%d",&tid);
	write(sd,&tid,sizeof(tid)); //1
	write(sd,&id,sizeof(id));   //2
	printf("\nEnter the no. of seats you want to book: ");
	scanf("%d",&seats);
	write(sd,&seats,sizeof(seats)); //3

	read(sd,&valid,sizeof(valid));
	if(valid)
		printf("\n\tTicket booked successfully.\n");
	else
		printf("\n\tSeats were not available.\n");


}

//view booking for induvidual login
void view_booking(int sd){   //no need of user id
	
	int no_of_bookings;
	int id,tid,seats;
	read(sd,&no_of_bookings,sizeof(no_of_bookings));

	printf("\tB_id\tT_no\tSeats\n");
	while(no_of_bookings--){
		read(sd,&id,sizeof(id));
		read(sd,&tid,sizeof(tid));
		read(sd,&seats,sizeof(seats));
		
		if(seats!=0)
			printf("\t%d\t%d\t%d\n",id,tid,seats);
	}
	return;
}

void update_booking(int sd){
		int bid, choice, val, valid = 0;
		printf("\n\t Enter the B_id you want to modify: ");  //booking id is unique for every booking irrepsective of users
		scanf("%d",&bid);
		write(sd,&bid,sizeof(bid));

		printf("\n\t1. Increase number of seats\n\t2. Decrease number of seats\n");
		printf("\t Your Choice: ");
		scanf("%d",&choice);
		write(sd,&choice,sizeof(choice));

		if(choice==1){
			printf("\n\tNo. of tickets to increase");
			scanf("%d",&val);
			write(sd,&val,sizeof(val));
		}
		else if(choice==2){
			printf("\n\tNo. of tickets to decrease");
			scanf("%d",&val);
			write(sd,&val,sizeof(val));
		}
		read(sd,&valid,sizeof(valid));
		if(valid)
			printf("\n\tBooking updated successfully.\n");
		else
			printf("\n\tUpdation failed. No more seats available.\n");
}

void cancel_ticket(int sd){
	int valid = 0, bid;
	printf("\n\t Enter the Booking id you want to cancel: ");
	scanf("%d",&bid);
	write(sd,&bid,sizeof(bid));  //1
	read(sd,&valid,sizeof(valid));
	if(valid)
		printf("\n\tBooking cancelled successfully.\n");
	else
		printf("\n\tCancellation failed.\n");
}
