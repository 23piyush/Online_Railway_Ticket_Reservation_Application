#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"struc.h"
#define MAX 20
char cur_user[20];

void mainMenu(int );
void displayMenu(int,int);
void admin_functions(int);
void signUp(int, int);
void login(int);
void adminLogin(int);
void add_train(int);
void view_train(int);
void book_ticket(int);
void view_booking(int,int);
void cancel_ticket(int,int);
void view_userList(int);
void update_booking(int);
//------------------------------------------------------------Main Menu-----------------------------------------------------//
void mainMenu(int sd)
{
	int choice,fd;
	read(sd, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: signUp(sd,1);
				break;		
		case 2: login(sd);
				break;
		case 3: adminLogin(sd);
				break;
		/*case 4: jointAccountLogin(sd);
				break;*/
		case 5: exit(0);
				break;

	}
}

//------------------------------------------------------------SignUp function-----------------------------------------------------//
int checkAccountExists(char uname[])
{
	struct database db;

	int fd1 = open("Accounts.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	
	while(read(fd1, (char *)&db, sizeof(struct database)))
	{

		if(!strcmp(db.uname, uname))  // strcmp() returns zero
			{
				close(fd1);
				return 1;
			}
	}
	close(fd1);
	return 0;	
	
}

void addAccount(char uname[], char pass[],int sd)
{
	srand(time(0));

	int fd1 = open("Accounts.dat", O_CREAT | O_APPEND | O_RDWR , 0666);

	struct database db;

	strcpy(db.uname, uname);
	strcpy(db.pass, pass);


	db.user_id=(rand()%1001)+3000;

	write(fd1, (char *)&db, sizeof(struct database));

	write(sd, "-------Account Created-------", 29);
	
	close(fd1);
}

void signUp(int sd,int flag)
{
	char uname[20], pass[20];

	read(sd, uname, sizeof(uname));
	read(sd, pass, sizeof(pass));

	if(!checkAccountExists(uname))
	{
		addAccount(uname, pass, sd);
		if(flag ==1)
			mainMenu(sd);
		else
			admin_functions(sd);
	}
	else
	{
		write(sd, "Username already exist\n", 23);
		if(flag ==1){
			printf("Please enter your login details\n");
			mainMenu(sd);
			}
		else
			admin_functions(sd);
	}	

}


//------------------------------------------------------------Login-----------------------------------------------------//
void login(int sd)
{
	
	char uname[20], pass[20];

	read(sd, uname, sizeof(uname));
	read(sd, pass, sizeof(pass));
	printf("server is reading");
	struct database db;
	int flag=1;

	int fd1 = open("Accounts.dat", O_RDWR , 0666);
	
	while(read(fd1, (char *)&db, sizeof(struct database)))
	{

		if(!strcmp(db.uname, uname) && !strcmp(db.pass, pass))
			{
				write(sd, &flag, sizeof(flag));
				write(sd, &db.user_id,sizeof(db.user_id));
				write(sd, "--Login Successful--",20);
				displayMenu(sd, db.user_id);
				close(fd1);
			
				return;		
			}
	}

	close(fd1);
	flag=0;
	write(sd, &flag, sizeof(flag));
	write(sd, "Invalid Credentials", 19);
	mainMenu(sd);	
}

void displayMenu(int sd, int id)
{
	int choice;
	read(sd, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: view_train(sd);
			displayMenu(sd, id);
				break;
		case 2: book_ticket(sd);
			displayMenu(sd, id);
				break;
		case 3: view_booking(sd,id);
			displayMenu(sd, id);
				break;
		case 4: update_booking(sd);
			displayMenu(sd, id);
				break;
		case 5: cancel_ticket(sd,id);
			displayMenu(sd, id);
				break;

	}

}

//--------------------------------------------Book Ticket----------------------------------------//


//--------------------------------------------Add Train----------------------------------------//
void adminLogin(int sd)
{
	
	char uname[20], pass[20];
	int flag=1;
	read(sd, uname, sizeof(uname));
	read(sd, pass, sizeof(pass));
	if(!strcmp(uname,"admin") && !strcmp(pass,"admin"))
	{
			
		write(sd, &flag, sizeof(flag));
		write(sd,"Welcome Admin", 13);
		admin_functions(sd);
	}
	else
	{	
		flag=0;
		write(sd, &flag, sizeof(flag));
		write(sd,"Invalid Credentials", 20);
		
		
		mainMenu(sd);
	}

}

void admin_functions(int sd)
{
	int choice;
	char uname[20], pass[20], uname1[20], uname2[20];
	read(sd, &choice, sizeof(choice));
	switch(choice)
	{
		case 1:add_train(sd);
			break;
		case 2: view_train(sd);
			break;
		case 3: view_userList(sd);
			break;
		/*case 4:deleteAccount(sd);
			break;
		case 3:read(sd, uname, sizeof(uname));
				if(checkAccountExists(uname))
			{
				write(sd, "Account Found for uname\0", 20);
			}
			else
			{
				write(sd, "Account does not exist with the given Username", 46);
			}
			admin_functions(sd);
			break;
		case 4:newJointAccount(sd,0);
			break;
		case 5:deleteJointAccount(sd);
		     	break;
		case 6:read(sd, uname1, sizeof(uname1));
				read(sd, uname2, sizeof(uname2));
				if(jointAccountExists(uname1,uname2))
			{
				write(sd, "Account Found\0", 14);
			}
			else
			{
				write(sd, "Account does not exist with the given Username", 46);
			}
			admin_functions(sd);
			break;*/
			
		default:mainMenu(sd);
	}
}

void add_train(int sd){
	int valid = 0;
	char tname[50];
	int tid = 0;
	read(sd,&tname,sizeof(tname));
	struct train tdb,temp;
	struct flock lock;
	int fd_train = open("db_train", O_CREAT|O_APPEND|O_RDWR, 0666);
	
	tdb.train_number = tid;
	strcpy(tdb.train_name,tname);
	tdb.total_seats = 10;				// by default, we are taking 10 seats
	tdb.available_seats = 10;

	int fp = lseek(fd_train, 0, SEEK_END); 

	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();

	fcntl(fd_train, F_SETLKW, &lock);

	if(fp == 0){
		valid = 1;
		write(fd_train, &tdb, sizeof(tdb));
		lock.l_type = F_UNLCK;
		fcntl(fd_train, F_SETLK, &lock);
		close(fd_train);
		write(sd, &valid, sizeof(valid));
	}
	else{
		valid = 1;
		lseek(fd_train, -1 * sizeof(struct train), SEEK_END);
		read(fd_train, &temp, sizeof(temp));
		tdb.train_number = temp.train_number + 1;
		write(fd_train, &tdb, sizeof(tdb));
		write(sd, &valid,sizeof(valid));	
	}
	lock.l_type = F_UNLCK;
	fcntl(fd_train, F_SETLK, &lock);
	close(fd_train);
	admin_functions(sd);
}

void view_userList(int sd){
	struct flock lock;
	struct database udb;
	int fd_user = open("Accounts.dat", O_RDONLY);
	
	lock.l_type = F_RDLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	
	fcntl(fd_user, F_SETLKW, &lock);
	int fp = lseek(fd_user, 0, SEEK_END);
	int no_of_users = fp / sizeof(struct database);
	printf("\nuser = %d", no_of_users);
	//no_of_users--;
	write(sd, &no_of_users, sizeof(int));

	lseek(fd_user,0,SEEK_SET);
	while(fp != lseek(fd_user,0,SEEK_CUR)){
		read(fd_user,&udb,sizeof(udb));
		write(sd,&udb.user_id,sizeof(int));
		write(sd,&udb.uname,sizeof(udb.uname));
	}
	lock.l_type = F_UNLCK;
	fcntl(fd_user, F_SETLK, &lock);
	close(fd_user);
}
//------------------------User-------------------------------//
void view_train(int sd){
	struct flock lock;
	struct train tdb;
	int fd_train = open("db_train", O_RDONLY);
	
	lock.l_type = F_RDLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	
	fcntl(fd_train, F_SETLKW, &lock);
	int fp = lseek(fd_train, 0, SEEK_END);
	int no_of_trains = fp / sizeof(struct train);
	write(sd, &no_of_trains, sizeof(int));

	lseek(fd_train,0,SEEK_SET);
	while(fp != lseek(fd_train,0,SEEK_CUR)){
		read(fd_train,&tdb,sizeof(tdb));
		write(sd,&tdb.train_number,sizeof(int));
		write(sd,&tdb.train_name,sizeof(tdb.train_name));
		write(sd,&tdb.total_seats,sizeof(int));
		write(sd,&tdb.available_seats,sizeof(int));
	}
	//valid = 1;
	lock.l_type = F_UNLCK;
	fcntl(fd_train, F_SETLK, &lock);
	close(fd_train);
}

void book_ticket(int sd){
	int valid = 0, id;
	struct flock lockt;
	struct flock lockb;
	struct train tdb;
	struct booking bdb;
	int fd_train = open("db_train", O_RDWR);
	int fd_book = open("db_booking", O_CREAT|O_RDWR|O_APPEND,0666);
	int tid,seats;
	read(sd,&tid,sizeof(tid));	//1	
	read(sd,&id,sizeof(id));	//2
	lockt.l_type = F_WRLCK;
	lockt.l_start = tid*sizeof(struct train);
	lockt.l_len = sizeof(struct train);
	lockt.l_whence = SEEK_SET;
	lockt.l_pid = getpid();

	lockb.l_type = F_WRLCK;
	lockb.l_start = 0;
	lockb.l_len = 0;
	lockb.l_whence = SEEK_END;
	lockb.l_pid = getpid();

	fcntl(fd_train, F_SETLKW, &lockt);
	lseek(fd_train,tid*sizeof(struct train),SEEK_SET);

	read(fd_train,&tdb,sizeof(tdb)); //reading from train database
	read(sd,&seats,sizeof(seats));  //3

	if(tdb.train_number==tid)
	{		
		if(tdb.available_seats>=seats){
			valid = 1;
			tdb.available_seats = tdb.available_seats-seats;
			fcntl(fd_book, F_SETLKW, &lockb);
			int fp = lseek(fd_book, 0, SEEK_END);
			
			if(fp > 0){
				lseek(fd_book, -1*sizeof(struct booking), SEEK_CUR);
				read(fd_book, &bdb, sizeof(struct booking));
				bdb.booking_id++;
			}
			else 
				bdb.booking_id = 0;

			bdb.uid = id;
			bdb.tid = tid;
			bdb.seats = seats;
			write(fd_book, &bdb, sizeof(struct booking));
			lockb.l_type = F_UNLCK;
			//fcntl(fd_book, F_SETLK, &lockb);
		 	close(fd_book);
		}

	lseek(fd_train, -1*sizeof(struct train), SEEK_CUR);
	write(fd_train, &tdb, sizeof(tdb));
	}

	lockt.l_type = F_UNLCK;
	fcntl(fd_train, F_SETLK, &lockt);
	close(fd_train);
	write(sd,&valid,sizeof(valid));
}

void view_booking(int sd, int id){
	struct flock lock;
	struct booking bdb;
	int fd_book = open("db_booking", O_RDONLY);
	int no_of_bookings = 0;

	lock.l_type = F_RDLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	
	fcntl(fd_book, F_SETLKW, &lock);

	while(read(fd_book,&bdb,sizeof(bdb))){
		if (bdb.uid==id)
			no_of_bookings++;
	}
	printf("inside view booking\n");
	write(sd, &no_of_bookings, sizeof(int));
	lseek(fd_book,0,SEEK_SET);

	while(read(fd_book,&bdb,sizeof(bdb))){
		if(bdb.uid==id){
			write(sd,&bdb.booking_id,sizeof(int));
			write(sd,&bdb.tid,sizeof(int));
			write(sd,&bdb.seats,sizeof(int));
		}
	}
	lock.l_type = F_UNLCK;
	fcntl(fd_book, F_SETLK, &lock);
	close(fd_book);
	return;
}

void update_booking(int sd){
	int bid, choice, val, valid = 0;
	struct booking bdb;
	struct train tdb;
	struct flock lockb;
	struct flock lockt;
	int fd_book = open("db_booking", O_RDWR);
	int fd_train = open("db_train", O_RDWR);
	read(sd,&bid,sizeof(bid));

	lockb.l_type = F_WRLCK;
	lockb.l_start = bid*sizeof(struct booking);
	lockb.l_len = sizeof(struct booking);
	lockb.l_whence = SEEK_SET;
	lockb.l_pid = getpid();
	
	fcntl(fd_book, F_SETLKW, &lockb);
	lseek(fd_book,bid*sizeof(struct booking),SEEK_SET);
	read(fd_book,&bdb,sizeof(bdb));
	lseek(fd_book,-1*sizeof(struct booking),SEEK_CUR);
	
	lockt.l_type = F_WRLCK;
	lockt.l_start = (bdb.tid)*sizeof(struct train);
	lockt.l_len = sizeof(struct train);
	lockt.l_whence = SEEK_SET;
	lockt.l_pid = getpid();

	fcntl(fd_train, F_SETLKW, &lockt);
	lseek(fd_train,(bdb.tid)*sizeof(struct train),SEEK_SET);
	read(fd_train,&tdb,sizeof(tdb));
	lseek(fd_train,-1*sizeof(struct train),SEEK_CUR);

	read(sd,&choice,sizeof(choice));

	if(choice==1){							// increase number of seats required of booking id
		read(sd,&val,sizeof(val));
		if(tdb.available_seats>=val){
			valid=1;
			tdb.available_seats -= val;
			bdb.seats += val;
		}
	}
	else if(choice==2){						// decrease number of seats required of booking id
		valid=1;
		read(sd,&val,sizeof(val));
		tdb.available_seats += val;
		bdb.seats -= val;	
	}
	
	write(fd_train,&tdb,sizeof(tdb));
	lockt.l_type = F_UNLCK;
	fcntl(fd_train, F_SETLK, &lockt);
	close(fd_train);
	
	write(fd_book,&bdb,sizeof(bdb));
	lockb.l_type = F_UNLCK;
	fcntl(fd_book, F_SETLK, &lockb);
	close(fd_book);
	
	write(sd,&valid,sizeof(valid));
}

void cancel_ticket(int sd, int id){
	int valid = 0, bid;
	struct booking bdb;
	struct train tdb;
	struct flock lockb;
	struct flock lockt;
	int fd_book = open("db_booking", O_RDWR);
	int fd_train = open("db_train", O_RDWR);
	read(sd,&bid,sizeof(bid));  //1

	lockb.l_type = F_WRLCK;
	lockb.l_start = bid*sizeof(struct booking);
	lockb.l_len = sizeof(struct booking);
	lockb.l_whence = SEEK_SET;
	lockb.l_pid = getpid();
	
	//setting lseek position in booking structure
	fcntl(fd_book, F_SETLKW, &lockb);
	lseek(fd_book,bid*sizeof(struct booking),SEEK_SET);
	read(fd_book,&bdb,sizeof(bdb));
	lseek(fd_book,-1*sizeof(struct booking),SEEK_CUR);
	
	lockt.l_type = F_WRLCK;
	lockt.l_start = (bdb.tid)*sizeof(struct train);
	lockt.l_len = sizeof(struct train);
	lockt.l_whence = SEEK_SET;
	lockt.l_pid = getpid();

	fcntl(fd_train, F_SETLKW, &lockt);
	lseek(fd_train,(bdb.tid)*sizeof(struct train),SEEK_SET);
	read(fd_train,&tdb,sizeof(tdb));
	lseek(fd_train,-1*sizeof(struct train),SEEK_CUR);

	tdb.available_seats += bdb.seats;
	if(tdb.available_seats>10)
		tdb.available_seats=10;
	bdb.seats = 0;
	valid = 1;

	write(fd_train,&tdb,sizeof(tdb));
	lockt.l_type = F_UNLCK;
	fcntl(fd_train, F_SETLK, &lockt);
	close(fd_train);
	
	write(fd_book,&bdb,sizeof(bdb));
	lockb.l_type = F_UNLCK;
	fcntl(fd_book, F_SETLK, &lockb);
	close(fd_book);
	
	write(sd,&valid,sizeof(valid));
}
