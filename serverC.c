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
#include <math.h>

#define C_UDP_PORT 23734 

int main() 
{ 
    int sockfd, addrlen, storagelen, i; 
    char client_buffer[30] ={0};
    float BW1=0, L1=0, V1=0, NP1=0;    
    char matchbuffer[10] = {0};
    

    struct sockaddr_in servaddr;                                                     
    memset(&servaddr, 0, sizeof(servaddr)); 
    addrlen = sizeof(servaddr); 
    servaddr.sin_family = AF_INET;                                                     
    servaddr.sin_port = htons(C_UDP_PORT); 
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
    printf("The Server C is up and running using UDP on port < %d > \n",C_UDP_PORT);


    recvfrom(sockfd, client_buffer, 20, 0, ( struct sockaddr *) &storage, &storagelen);                        
    
    
    char cli_para[10][20]={0},*token;
    if ((token = strtok(client_buffer, ",")) != NULL)
    {    
        i=0;
            do
        {
                strcpy(cli_para[i],token);
            i=i+1;
            } while ((token = strtok(NULL, ",")) != NULL);
    }

    recvfrom(sockfd, (char *)matchbuffer, 20, 0, ( struct sockaddr *) &storage, &storagelen);                
    

    if(strcmp(matchbuffer,"8") == 0)        
    {

    printf("The server C received link information of link_Id= < %s >, Size= < %s >, Power= < %s > dB \n", cli_para[0],cli_para[1],cli_para[2]);

    recvfrom(sockfd, &BW1, 20, 0, ( struct sockaddr *) &storage, &storagelen);                            
    recvfrom(sockfd, &L1, 20, 0, ( struct sockaddr *) &storage, &storagelen);
    recvfrom(sockfd, &V1, 20, 0, ( struct sockaddr *) &storage, &storagelen);
    recvfrom(sockfd, &NP1, 20, 0, ( struct sockaddr *) &storage, &storagelen);
    
    
    
    float Size = atof(cli_para[1]);                                                    
    float SP1 = atof(cli_para[2]);

    float Tprop , Ttrans , Delay, Capacity, normal_SPower, normal_NPower,SNR; 

    normal_SPower = pow(10,(SP1/10))/1000;
    normal_NPower = pow(10,(NP1/10))/1000;
    SNR = normal_SPower/normal_NPower;
    Capacity = BW1 * 1000000 * log2 (1+SNR);
    
    Ttrans = Size/Capacity*1000;
    Tprop = L1*1000/(V1*10000000)*1000;
    Delay = Tprop + Ttrans;    

    char B1[100] = {0};
    char B2[100] = {0};
    char B3[100] = {0};
    sprintf(B1,"%.2f",Ttrans);
    sprintf(B2,"%.2f",Tprop);
    sprintf(B3,"%.2f",Delay);

    printf("The server C finished the calculations for link < %s >\n",cli_para[0]);

    sendto(sockfd, B1, 100, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);                     
    sendto(sockfd, B2, 100, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);
    sendto(sockfd, B3, 100, 0 , (const struct sockaddr *) &storage, (socklen_t)storagelen);

    
    printf("The server C finished sending the output to AWS\n");
    }

    return 0; 
}
