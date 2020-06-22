/*
 * @author Elnaz Chaposhlo
 * Project
 * Sockets - Online Game
*/

//server side


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


void servicePlayers(int, int);


int main(int argc, char *argv[])
{
	int sd, client1, client2, portNumber, status;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 2){ //Ask user to provide port number.
	   printf("Call model: %s <Port #>\n", argv[0]);
	   exit(0);
	}

	if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){//Create socket for Server
	   printf("Cannot create socket\n");
	   exit(1);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);//Server listen to all it's interfaces
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);


	bind(sd,(struct sockaddr*)&servAdd, sizeof(servAdd));
	listen(sd, 6);//server can have at most 6 clients pendding

		while(1){//An infinite loop to wait for clients
		client1 = accept(sd, (struct sockaddr *)NULL, NULL);
		client2 = accept(sd, (struct sockaddr *)NULL, NULL);
		printf("Now I Got two clients\n");

		     if(!fork()){//Create a child process
			servicePlayers(client1, client2);//Function to take care of clients
			}
		}
	return 0;
}

void servicePlayers(int client1, int client2)
{

	char message[255];
	int score1 = 0;
	int score2 = 0;
	int point1 = 0;
	int point2 = 0;


	while(1){//The referee gets in an infinite loop

	    printf("\nReferee: TOTO plays:\n\n");
	    write(client1, "You can now play.", 18);//Server sends a messge to client1

		if(read(client1, &score1, 255)){//Server reads the score obtained by client1
	    	   printf("TOTO: My score is: %d\n\n", score1);
		   point1 += score1;//Total score of client1
		   printf("TOTO: My total score is: %d\n\n", point1);
		}

		if(point1 >= 100){//Total score that reaches 100
                         write(client1, "Game over: TOTO! You won the game.", 35);
                         close(client1);//Close client's socket
                         write(client2, "Game over: TITI! You lost the game.", 36);
                         close(client2);

			 printf("TOTO is the winner.\n\nTITI is the loser.\n\n");

                         exit(0);
			}

		sleep(2);

	    printf("\nReferee: TITI plays:\n\n");
	    write(client2, "You can now play.", 18);
		if(read(client2, &score2, 255)){//Server reads the score obtained by client2
	    	   printf("TITI: My score is: %d\n\n", score2);
		   point2 += score2;//Total score of client2
		   printf("TITI: My total score is: %d\n\n", point2);
	           }

		 if(point2 >= 100){
			 write(client2, "Game over: TITI! You won the game.", 35);
			 close(client2);
			 write(client1, "Game over: TOTO! You lost the game.", 36);
			 close(client1);

			 printf("TITI is the winner.\n\nTOTO is the loser.\n\n");

			 exit(0);
			 }

		sleep(2);

	    }
}





