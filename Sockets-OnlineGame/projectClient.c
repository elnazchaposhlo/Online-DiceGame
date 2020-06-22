/*
 * @author Elnaz Chaposhlo
 * Project
 * Sockets - Online Game
*/

//Client side


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int strcmp (const char* str1, const char* str2);


int main(int argc, char *argv[])
{
	char message[255];
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	int dice = 0;


	if(argc != 3){
	   printf("Call model: %s <IP> <Port #>\n", argv[0]);
	   exit(0);
	  }

	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){//Create socket
	   fprintf(stderr, "Cannot create socket\n");
	   exit(1);
	  }

	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<0){
	   fprintf(stderr, " inet_pton() has failed\n");
	   exit(2);
	  }

	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){//Client connect to server
	   fprintf(stderr, "connect() has failed, exiting\n");
	   exit(3);
	  }

	while(1){//An infinite loop
	read(server, message, 255); //reading server's messages
	printf("\nMessage received: %s\n\n", message);


	if(!strcmp(message, "You can now play.")){//Client receive server's message and allow to play dice
	   printf("Playing my dice.\n\n");

	   srand(time(0));//Generate random number
	   dice = (rand() % 6) + 1;

     	   printf("I got %d points.\n\n", dice);//Client prints obtained score on it's screen
	   write(server, &dice, 255);//Send score to the server
	 }

	else if(!strcmp(message,"Game over: TOTO! You won the game.")){
	   printf("Wohoo! I won the game.\n");
	   close(server);//Close socket
	   exit(0);
	         }

	 else if(!strcmp(message, "Game over: TITI! You lost the game.")){
	   printf("Sigh! I lose the game.\n");
	   close(server);
	   exit(0);
	 	 }
	}
}

