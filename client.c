#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORTc 25734

int main(int argc, char const *argv[]) 
{
    int client_fd;                                            
    char para_buffer[20] = {0};
    char B3[100] = {0};
    char matchbuffer[10] = {0};

    struct sockaddr_in serv_addr;                                    
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORTc); 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)                     
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)                         
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    printf("The client is up and running \n");
    
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)             
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    strcat(para_buffer,argv[1]);                                    
    strcat(para_buffer,",");
    strcat(para_buffer,argv[2]);
    strcat(para_buffer,",");
    strcat(para_buffer,argv[3]); 
    send(client_fd , para_buffer, 20, 0 );
    printf("The client sent link_Id= < %s >, size= < %s >,and power= < %s > to AWS \n",argv[1],argv[2],argv[3]);

    recv(client_fd, (char *)matchbuffer, 20 , 0);

    if(strcmp(matchbuffer,"8") == 0)
    {
    recv(client_fd, B3 , 10, 0);                                 // Receive DATA 
    printf("The delay of the link < %s > is < %s > ms \n",argv[1] ,B3 );
    }
    else
    {
    printf("Found no match for link < %s > \n",argv[1]);
    }

    return 0;
}