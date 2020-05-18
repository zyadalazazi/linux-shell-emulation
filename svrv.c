#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <fcntl.h> 
#include <arpa/inet.h>



#define bufSize 1000
#define NmbOfCmd 2
#define max 10

# define MAXLINE 4096
# define BACKLOG 10
# define SERV_PORT 2021

# define LENGTH 512




        char inputString[MAXLINE];
        int n;
        

        void* server_handler(void* arg){
        char buffer[256];
        int* client_socket = (int*) arg;
        processing2(inputString);
        char* fs_name = "naw.txt";
        char sdbuf[LENGTH]; 
        printf("[Client] Sending %s to the Server...", fs_name);
        pid_t po = fork();
        
        //overwriting
        if (po==0){
        fclose(fopen(fs_name, "w"));}
        else{
            FILE *fs = fopen(fs_name, "r");
             if(fs == NULL)
            {
            printf("ERROR: File %s not found.\n", fs_name);
            exit(1);
            }

             bzero(sdbuf, LENGTH); 
            int fs_block_sz; 
            //int success = 0;
            while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0)
            {
            if(send(*client_socket, sdbuf, fs_block_sz, 0) < 0)
            {
                printf("ERROR: Failed to send file %s.\n", fs_name);
                break;
            }
            bzero(sdbuf, LENGTH);
            }
            printf("Ok File %s from Client was Sent!\n", fs_name);
       
            bzero(inputString, MAXLINE);

       
        }
     close(client_socket);
    pthread_exit(0);
}

char *choppy( char *s )
{
    char *n = malloc( strlen( s ? s : "\n" ) );
    if( s )
        strcpy( n, s );
    n[strlen(n)-1]='\0';
    return n;
}
int commandhandler(char * inputString[]){

    char * casee[NmbOfCmd];
    casee[0] = "exit";
    casee [1]= "chdir"; 
    char buf[bufSize];
    int p=100;
    int i=0;

    for(i=0;i<NmbOfCmd;i++){
        if (strcmp(inputString[0],  casee[i])==0){
            p=i;
            break;
        }
    }


    switch(p){
    case 0 :
    printf("thank you for using my OS\n");
    exit(0);
    return 1;
    case 1 : 
    if (chdir(inputString[1])!=0){
        printf("error\n");
    }
        return 1;
        default:
        break;
    }
    return 0;


}
void AxecArg(char * inputString[] ){
    pid_t pid6 = fork();
    if (pid6<0){
        printf("couldnt fork");
        return;
    } else if (pid6==0){
        int file_c = open("naw.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(file_c, 1);

        if (execvp(inputString[0],inputString)<0){
            printf("couldnt execute one arg\n");
            return;
        } 
        close(file_c);
        exit(0);
    }else {
        wait(NULL);
        return;
    }

}

void parsepipe(char * inputString, char *parsed[]){

int i;
for (i=0;i<20;i++){
parsed[i] = strsep(&inputString,"|");
if (parsed[i]==NULL){break;}
if (strlen(parsed[i]) == 0) 
            i--; 
    } 
   
}
void parsespace(char * inputString,char*parsed[]){
int i;
    for (i=0;i<20;i++){
    parsed[i] = strsep(&inputString," ");
    if (parsed[i]==NULL){break;}
    if (strlen(parsed[i]) == 0) 
           i--; 
     } 

}
int nbOfcommands(char * inputString)
{   
    int i;
    int n=0;
    for (i = 0; i < strlen(inputString); i++){
        if ( inputString[i]=='|'){
            n+=1;}
        
    }
    
    return n + 1;   
}


void clearr (char * parsespacef[]){
int i=0;
for (i=0;i<10;i++){
    parsespacef[i]=NULL;
}

}
void multipipe (char* parsedpipe[], int nmbcmd){
    
    if (nmbcmd>10)
        return;
    char* parsespacef[10];  
    int i;
    int fd[10][2];

    for (i=0;i<nmbcmd;i++){

        clearr(parsespacef);
        parsespace(parsedpipe[i],parsespacef);
        
        if (i!=nmbcmd -1){
            if (pipe(fd[i])<0){
                perror("pipe creating failed");
                return;
            }
        }
        if (fork()==0){
            if (i!=nmbcmd-1){
                dup2(fd[i][1],1);
                close(fd[i][0]);
                close(fd[i][1]);
            }
            if (i!=0){
                dup2(fd[i-1][0],0);
                close(fd[i-1][1]);
                close(fd[i-1][0]);
            }
        if (i==nmbcmd-1){
        
            int file_c = open("naw.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(file_c, 1);
            execvp(parsespacef[0],parsespacef);
            close(file_c);

        } else{
            execvp(parsespacef[0],parsespacef);
            perror("exec not success");
            exit(1);
        } }
        if (i!=0){
        close(fd[i-1][0]);
        close(fd[i-1][1]);
        
        
        }
        wait(NULL);
    
    }
}
    
    



void processing2(char * inputString)
{ 
    
    n = nbOfcommands(inputString);

    inputString = choppy(inputString);

    char * parsedpiped[2];
    char * parsed1[max];
    char * parsed2[max];
    parsepipe(inputString, parsedpiped);
    if (parsedpiped[1] == NULL)
    { 
        
        parsespace(inputString, parsed1);

        if (commandhandler(parsed1) == 1)
            ;
        else
            AxecArg(parsed1);
    }
    else 
    { 
        
         multipipe(parsedpiped, n);

        }
}


int main() {


    int server_socket = 0;
    int client_socket = 0;

    struct sockaddr_in servaddr;                      // structure containing the internet address 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);  //  The first is the address domain of the socket // The second argument is the type of socket //  The third argument is the protoco
    int client_count = 0;                               

    if (server_socket < 0)      
        exit(1);

    //////////////////////////////////////////////////////////////////////

    servaddr.sin_family = AF_INET;                  
    servaddr.sin_addr.s_addr = INADDR_ANY;      // This field contains the IP address of the host constant INADDR_ANY which gets this address.
    servaddr.sin_port = htons(SERV_PORT);               //  which contain the port number

    if (bind(server_socket, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)    //The bind() system call binds a socket to an address
    {   
        perror("error on binding");
        exit(1);
    }
   
    //////////////////////////////////////////////////////////////////////

    while (1)
    {
         if (listen(server_socket, BACKLOG) < 0)             // listen on the socket for connections // backlog queue, i.e., the number of connections that can be waiting while the process is handling a particular connection
    {
        perror("error while listening ");
        exit(1);
    }

        client_socket = accept(server_socket, (struct sockaddr *)NULL, NULL);   // The accept() system call causes the process to block until a client connects to the server.
        if (client_socket < 0){
            continue;
        }
      
  
    
        
        client_count++;
        int n = read(client_socket, inputString, sizeof(inputString));

        if (n<0)
            perror("error while reading");
        // else
        //     inputString[n] = '\0';
         pid_t po = fork();
        if (po==0){
        fclose(fopen("naw.txt", "w"));}

        pthread_t thread;
        pthread_attr_t attr; // attribute object for the thread
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // make the thread detachable      
        /* Create a thread to serve each client */
        pthread_create(&thread, &attr, server_handler, &client_socket);


    }

return 0;

}
