/*
  @author Elnaz Chaposhlo
  
  Online Dice Game
 
 Guided By:
	Dr. Boubakeur Boufama
*/

//client side


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


//the C library function strcmp() compares the string pointed to, by str1 to the string pointed to by str2.
int strcmp (const char* str1, const char* str2);


int main(int argc, char *argv[])
{
	char message[255];
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd; 		// server socket address

	int dice = 0;

	//checks whether port number and server's IP address is passes as command line arguement or not
	if(argc != 3){
	   printf("Call model: %s <IP> <Port #>\n", argv[0]);
	   exit(0);
	  }

	//checks whether socket is created or not
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
	   fprintf(stderr, "Cannot create socket\n");
	   exit(1);
	  }

	servAdd.sin_family = AF_INET; 		//Domain for Internet, AF -> Address Family
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber); 		//host to network for short integer : port number

	
	// checks whether input is a valid IPv4 dotted-decimal string or not
	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<0){
	   fprintf(stderr, " inet_pton() has failed\n");
	   exit(2);
	  }

	//checks whether the connection with server is established or not
	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
	   fprintf(stderr, "connect() has failed, exiting\n");
	   exit(3);
	  }

	//reads message from server
	while(1){//An infinite loop
	read(server, message, 255); 		//reading server's messages
	printf("\nMessage received: %s\n\n", message);


	//checks server message if its client's turn
	if(!strcmp(message, "You can now play.")){
	   printf("Playing my dice.\n\n");

	   srand(time(0));		//generate random number
	   dice = (rand() % 6) + 1;

     	   printf("I got %d points.\n\n", dice);		//prints score to the screen
	   write(server, &dice, 255); 		//sending its score to server
	 }

	//checks server message if client has won
	else if(!strcmp(message,"Game over: TOTO! You won the game.")){
	   printf("Wohoo! I won the game.\n");
	   close(server);		//close socket
	   exit(0);
	         }

	//checks server message if client has lose
	 else if(!strcmp(message, "Game over: TITI! You lost the game.")){
	   printf("Sigh! I lose the game.\n");
	   close(server);		//close socket
	   exit(0);
	 	 }
	}
}

