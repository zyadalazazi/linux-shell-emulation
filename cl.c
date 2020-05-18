#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

# define MAXLINE 4096
# define BACKLOG 10
# define SERV_PORT 2021

# define LENGTH 16001


int main(int argc, char *argv[])
{
     while(1)
    {   
    int client_socket = 0;
    struct sockaddr_in serveraddr;
    char* add = "127.0.0.1";		

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    	exit(1);

  ///////////////////////////////////////////////////////////////////

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, add, &serveraddr.sin_addr) <= 0) {
    	perror("error in inet_pton");
    }

    if (connect(client_socket, &serveraddr, sizeof(serveraddr)) < 0)		// The connect function is called by the client to establish a connection to the server
    	perror("error connecting");

  ////////////////////////////////////////////////////////////////////


   
        char buff[MAXLINE];
        printf("\033[0;33m");
        printf("MyOperSys%s: ", getcwd(buff,MAXLINE) );
        printf("\033[0m");
        char *inputString=NULL;
        int sizo = 1000;
        getline(&inputString,&sizo,stdin);
        // printf("IN THE CLIEENTT%s\n",inputString );


	   int n = write(client_socket, inputString, strlen(inputString));
	   if (n<0)
	   {
	       perror("error in writing");
		  exit(1);
	   }   

       char revbuf[LENGTH];
       // char* fr_name = "haha.txt";
       //  FILE *fr = fopen(fr_name, "a");
        // if(fr == NULL)
        //     printf("File %s Cannot be opened file on server.\n", fr_name);
        // else
        // {
            bzero(revbuf, LENGTH); 
            int fr_block_sz = 0;
            //int success = 0;
            //while(success == 0)
            //{
                while(fr_block_sz = recv(client_socket, revbuf, LENGTH, 0)) //could it be sockfd?
                {
                    if(fr_block_sz < 0)
                    {
                        error("Error receiving file from client to server.\n");
                    }
                    // int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
                    // if(write_sz < fr_block_sz)
                    // {
                    //     error("File write failed on server.\n");
                    // }
                    printf("here : %s\n",revbuf );
                    if(fr_block_sz)
                    {
                        break;
                    }
                    bzero(revbuf, LENGTH);
                }
                // printf("Ok received from client!\n");
                // fclose(fr);
            //}
        // }
	   // bzero(inputString,MAXLINE);
	   // n = read(client_socket, inputString, sizeof(inputString));
	   // inputString[n] = '\0';
	   // if (n<0)	
		  // perror("error in reading");

	   // printf("%s\n", inputString);

    }   

	return 0;

}
