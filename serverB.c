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

#define B_UDP_PORT 22734 

int search(double inp[], int link_id, int size)
{
    int k=0;
    for (k=0; k<size; k+=5)
    {
            if(inp[k] == link_id)
            {
        return k;
        break;
            }
    }
        if(k == size)
        {
        return 0;
        }
}

int main() 
{ 
    int sockfd ,n, addrlen, storagelen; 
    char buffer[20]={0}; 
    
    struct sockaddr_in servaddr;                                                    
    memset(&servaddr, 0, sizeof(servaddr));     
    addrlen = sizeof(servaddr); 
    servaddr.sin_family = AF_INET;                                                     
    servaddr.sin_port = htons(B_UDP_PORT); 
    if(inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr)<=0)                                     
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    
    struct sockaddr_in storage;                                                     
    storagelen = sizeof(storage);

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )                                        
     { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )                             
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    printf("The Server B is up and running using UDP on port < %d > \n",B_UDP_PORT);

    n = recvfrom(sockfd, (char *)buffer, 20, 0, ( struct sockaddr *) &storage, &storagelen);                     
    buffer[n] = '\0'; 
    printf("The Server B received input < %s >\n", buffer);
    
    int a_para = atoi(buffer);
    FILE* stream = fopen("database_b.csv", "r");                                          
        char seps[] = ",";
        double input[1000]={0};                                                         
        char line[1000]={0};
        int i = 0;
        float var;
        char *token;
    float ID=0, BW=0, L=0, V=0, NP=0;
    while (fgets(line, 1024, stream))
        {
            char* tmp = strdup(line);
            token = strtok (tmp, seps);
            while (token != NULL)
            {
                sscanf (token, "%f", &var);
                input[i++] = var;
                token = strtok (NULL, seps);
        }
        free(tmp);
        }
        int tmp1 = search(input, a_para, i);
    
    if (tmp1 == 0)
    {
    sendto(sockfd, "0" , 10, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen); 
    printf ("The Server B found < 0 > match\n");
    }
    else
    {                                                        // Send DATA    
    sendto(sockfd, "1" , 10, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);
     printf ("The Server B found < 1 > match\n");
        ID = input[tmp1];
    BW = input[tmp1+1];
    L = input[tmp1+2];
    V = input[tmp1+3];
    NP = input[tmp1+4];
    sendto(sockfd, &ID , 20, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);
    sendto(sockfd, &BW , 20, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);
    sendto(sockfd, &L , 20, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);
    sendto(sockfd, &V , 20, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);
    sendto(sockfd, &NP , 20, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);

    }
    printf("The Server B finished sending the output to AWS\n");    

    return 0; 
}