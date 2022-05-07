#include<stdio.h>

struct database
{
	char uname[15];
	char pass[15];
	int  user_id;
	int  user_type;
};

struct train{
	char train_name[50];
	int  totalBooked;
	int  train_number;
	int  total_seats;
	int  available_seats;
};

struct booking{
	int booking_id;
	int tid;
	int seats;
	int uid;
};

struct clientRequest{
	int command;
	int uid;
	int trainNo;
	int deleteIndex;
	int updateIndex;
	int updatedSeats;
	struct train tr;
	struct booking bookReq;
	struct database newuser;
};

