/*
  @author Elnaz Chaposhlo
  
  Online Dice Game
 
 Guided By:
	Dr. Boubakeur Boufama
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
	struct sockaddr_in servAdd;		 // server socket address

	//checks whether port number is passed as command line arguement
	if(argc != 2){ 
	   printf("Call model: %s <Port #>\n", argv[0]);
	   exit(0);
	}

	//socket() creates an endpoint for communication
	if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){		//checks whether socket is created or not
	   printf("Cannot create socket\n");
	   exit(1);
	}

	servAdd.sin_family = AF_INET; 		//Domain for Internet, AF -> Address Family
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY); 		//host to network for long integer : INADDR_ANY(binds the socket to all available interfaces)
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber); 		//host to network for short integer : port number 

	
	//casts socket file descriptor to the protocal-specific address structure
	bind(sd,(struct sockaddr*)&servAdd, sizeof(servAdd));
	
	//listening socket listens the connections requested by client
	listen(sd, 6);

		while(1){
			
		//accepts pending connection requests from queue(FIFO) and create connected socket of each request
		client1 = accept(sd, (struct sockaddr *)NULL, NULL);
		client2 = accept(sd, (struct sockaddr *)NULL, NULL);
		printf("Now I Got two clients\n");

		     if(!fork())		//For every 2 players joined, child process is created for a new game between them
			{ 
			     servicePlayers(client1, client2); 		//child process calls servicePlayer method for client 1 and client 2 to play dice game
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


	while(1){

	    printf("\nReferee: TOTO plays:\n\n");
	    write(client1, "You can now play.", 18); 		//sends message to client 1 to play its turn

		//reads score from client 1
		if(read(client1, &score1, 255)){
	    	   printf("TOTO: My score is: %d\n\n", score1);
		   point1 += score1; 		//points earned by client 1
		   printf("TOTO: My total score is: %d\n\n", point1);		 //prints total points earned by client 1 so far
		}

		//checks if total score of client 1 reaches 100
		if(point1 >= 100){
                         
			write(client1, "Game over: TOTO! You won the game.", 35); 		//sends winning message to client 1
                         close(client1);		//close client's socket
                         write(client2, "Game over: TITI! You lost the game.", 36); 		//sends losing message to client 2
                         close(client2);

			 printf("TOTO is the winner.\n\nTITI is the loser.\n\n");

                         exit(0);
			}

		sleep(2);

	    printf("\nReferee: TITI plays:\n\n");
	    write(client2, "You can now play.", 18); 		//sends message to client 2 to play its turn
		
		//reads score from client 2
		if(read(client2, &score2, 255)) {
	    	   
		   printf("TITI: My score is: %d\n\n", score2);
		   point2 += score2; 		//points earned by client 2
		   printf("TITI: My total score is: %d\n\n", point2); 		//prints total points earned by client 2 so far
	          
	           }

		//checks if total score of client 2 reaches 100
		 if(point2 >= 100){
			 
			 write(client2, "Game over: TITI! You won the game.", 35); 		//sends winning message to client 2
			 close(client2); 		//closes client 2 socket
			 write(client1, "Game over: TOTO! You lost the game.", 36); 		//sends losing message to client 2
			 close(client1); 		//closes client 1 socket

			 printf("TITI is the winner.\n\nTOTO is the loser.\n\n");

			 exit(0);
			 }

		sleep(2);

	    }
}





