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

#define AWS_CLIENT_PORT 25734
#define AWS_MONITOR_PORT 26734
#define AWS_UDP_PORT 24734
#define A_UDP_PORT 21734
#define B_UDP_PORT 22734
#define C_UDP_PORT 23734

int main(int argc, char *argv[])
{


    int server_fdm, new_socketm, addrlenm;
    int server_fd, new_socket, addrlen;
    int i=0;

     char client_buffer[20] = {0};
    char monitor_buffer[20] = {0};
    char c_new_buffer[10][20]={0};

    struct sockaddr_in address, addressm;                                                
    memset(&address, 0, sizeof(address));    
    addrlen = sizeof(address); 
    address.sin_family = AF_INET;  
    address.sin_port = htons(AWS_CLIENT_PORT); 
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0)                         
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    printf("The AWS is up and running \n");

    memset(&addressm, 0, sizeof(addressm));                                                
    addrlenm = sizeof(addressm); 
    addressm.sin_family = AF_INET;  
    addressm.sin_port = htons(AWS_MONITOR_PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &addressm.sin_addr)<=0)                         
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }

    int sockfd, Aaddrlen, Baddrlen, Caddrlen; 
    
    char buffer[20]={0}; 

    struct sockaddr_in cliaddr, Aservaddr, Bservaddr, Cservaddr;                                     
    memset(&cliaddr, 0, sizeof(cliaddr)); 
    cliaddr.sin_family = AF_INET;                                                      
    cliaddr.sin_port = htons(AWS_UDP_PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &cliaddr.sin_addr)<=0)                                     
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    memset(&Aservaddr, 0, sizeof(Aservaddr));                                             
    Aaddrlen = sizeof(Aservaddr); 
    Aservaddr.sin_family = AF_INET;                                                     
    Aservaddr.sin_port = htons(A_UDP_PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &Aservaddr.sin_addr)<=0)                                     
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    memset(&Bservaddr, 0, sizeof(Bservaddr));                                             
    Baddrlen = sizeof(Bservaddr); 
    Bservaddr.sin_family = AF_INET;                                                     
    Bservaddr.sin_port = htons(B_UDP_PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &Bservaddr.sin_addr)<=0)                                     
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    memset(&Cservaddr, 0, sizeof(Cservaddr));                                             
    Caddrlen = sizeof(Cservaddr); 
    Cservaddr.sin_family = AF_INET;                                                     
    Cservaddr.sin_port = htons(C_UDP_PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &Cservaddr.sin_addr)<=0)                                     
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)                                     
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address) )<0)                                 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    if ((server_fdm = socket(AF_INET, SOCK_STREAM, 0)) == 0)                                     
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (bind(server_fdm, (struct sockaddr *)&addressm, sizeof(addressm) )<0)                             
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }  

    
    if (listen(server_fdm, 10) < 0)                                                
    {     
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    
    if ((new_socketm = accept(server_fdm, (struct sockaddr *)&addressm, (socklen_t *)&addrlenm))<0)                 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 


    if (listen(server_fd, 10) < 0)                                                     
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen))<0)                     
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 


    recv(new_socket, client_buffer, 20, 0);                                                
    char *token =strtok(client_buffer, ",");
    while(token != NULL)
    {
        strcpy(c_new_buffer[i], token);
        i++;
        token = strtok(NULL, ",");
    }    
            
printf("The AWS server recieved link_Id=< %s >, Size=< %s >, Power=< %s > dB from Client using TCP over Port Number= < %d > \n",c_new_buffer[0], c_new_buffer[1], c_new_buffer[2], AWS_CLIENT_PORT);



    strcat(client_buffer,",");                                                    
    strcat(client_buffer,c_new_buffer[1]);
    strcat(client_buffer,",");
    strcat(client_buffer,c_new_buffer[2]);
    send(new_socketm, client_buffer, 20, 0 );                                                
printf("The AWS server sent link_Id= < %s >, Size= < %s >, Power= < %s > dB to the Monitor using TCP over Port Number= < %d > \n", c_new_buffer[0], c_new_buffer[1], c_new_buffer[2], AWS_MONITOR_PORT);



        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )                                        
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
        } 
    if ( bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0 )                             
    { 
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }

    
    sendto(sockfd, client_buffer, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 



    char m[10]={0};
    float ID=0, BW=0, L=0, V=0, NP=0;
    char *link_ID;
    char B1[100]={0};
    char B2[100]={0};
    char B3[100]={0};
    char *match = "4";
    char *match1 = "8";

    link_ID= strtok(client_buffer, ",");
    sendto(sockfd, link_ID , 20, 0 , (const struct sockaddr *) &Aservaddr, (socklen_t )Aaddrlen);                    // Send DATA to A
    printf("The AWS sent link ID= < %s > to Backend Server A using UDP over Port Number= < %d > \n" , link_ID, AWS_UDP_PORT);
    
    recvfrom(sockfd, (char *)m, 10, 0 , ( struct sockaddr *) &Aservaddr, &Aaddrlen);                         // Receive DATA from A

    if(strcmp(m,"1") == 0)
    {
        printf("The AWS received < 1 > match from Backend Server A using UDP over Port Number= < %d >\n", AWS_UDP_PORT);
        recvfrom(sockfd, &ID , 20, 0 , ( struct sockaddr *) &Aservaddr, &Aaddrlen);
         recvfrom(sockfd, &BW, 20, 0 , ( struct sockaddr *) &Aservaddr, &Aaddrlen);
        recvfrom(sockfd, &L, 20, 0 , ( struct sockaddr *) &Aservaddr, &Aaddrlen);
         recvfrom(sockfd, &V, 20, 0 , ( struct sockaddr *) &Aservaddr, &Aaddrlen);
        recvfrom(sockfd, &NP, 20, 0 , ( struct sockaddr *) &Aservaddr, &Aaddrlen);
        //printf("aws received from A: %.2f, %.2f, %.2f, %.2f, %.2f\n",ID,BW,L,V,NP);
        
    printf("The AWS sent link_Id= < %s >, Size= < %s >, Power= < %s > dB to the Server C using UDP over Port Number= < %d > \n", c_new_buffer[0], c_new_buffer[1], c_new_buffer[2], AWS_UDP_PORT);
        sendto(sockfd, (const char *)match1, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen);
        
        sendto(sockfd, &BW, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen);                 
        sendto(sockfd, &L, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 
        sendto(sockfd, &V, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 
        sendto(sockfd, &NP, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 

        recvfrom(sockfd, B1, 100, 0 , ( struct sockaddr *) &Cservaddr, &Caddrlen);    
        recvfrom(sockfd, B2, 100, 0 , ( struct sockaddr *) &Cservaddr, &Caddrlen);                         
        recvfrom(sockfd, B3, 100, 0 , ( struct sockaddr *) &Cservaddr, &Caddrlen);
    
        printf("The AWS received output from Backend-Server C using UDP over port < %d >  \n",AWS_UDP_PORT);
        
        send(new_socket, match1, 20 , 0);
        send(new_socket, B3 , 10 , 0 );
        printf("The AWS sent delay = < %s > ms to the client using TCP over port < %d > \n", B3, AWS_CLIENT_PORT);    
    
        send(new_socketm, match1, 20 , 0);
        send(new_socketm, B1, 20, 0 );
        send(new_socketm, B2, 20, 0 );
        send(new_socketm, B3, 20, 0 );
        printf("The AWS server sent  detailed result to the Monitor using TCP over Port Number= < %d > \n", AWS_MONITOR_PORT);
    }
    else  
    {
        printf("The AWS received < 0 > match from Backend Server A using UDP over Port Number= < %d >\n", AWS_UDP_PORT);    
    } 

     

    char m1[10]={0};
    float ID1=0, BW1=0, L1=0, V1=0, NP1=0;
    int s1,s2,s3,s4,s5;
    
    sendto(sockfd, link_ID, 20, 0 , (const struct sockaddr *) &Bservaddr, (socklen_t )Baddrlen);                     
    printf("The AWS sent link ID= < %s > to Backend Server B using UDP over Port Number= < %d >\n", link_ID, AWS_UDP_PORT);

    recvfrom(sockfd, (char *)m1, 10, 0, ( struct sockaddr *) &Bservaddr, &Baddrlen);                         

    if(strcmp(m1,"1") == 0)
    {
        printf("The AWS received < 1 > match from Backend Server B using UDP over Port Number= < %d >\n", AWS_UDP_PORT);
        recvfrom(sockfd, &ID1 , 20, 0 , ( struct sockaddr *) &Bservaddr, &Baddrlen);
         recvfrom(sockfd, &BW1, 20, 0 , ( struct sockaddr *) &Bservaddr, &Baddrlen);
        recvfrom(sockfd, &L1, 20, 0 , ( struct sockaddr *) &Bservaddr, &Baddrlen);
         recvfrom(sockfd, &V1, 20, 0 , ( struct sockaddr *) &Bservaddr, &Baddrlen);
        recvfrom(sockfd, &NP1, 20, 0 , ( struct sockaddr *) &Bservaddr, &Baddrlen);
       
        
    printf("The AWS sent link_Id= < %s >, Size= < %s >, Power= < %s > dB to the Server C using UDP over Port Number= < %d > \n", c_new_buffer[0], c_new_buffer[1], c_new_buffer[2], AWS_UDP_PORT);
        s1=sendto(sockfd,(const char *)match1, 10 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen);

        s2=sendto(sockfd, &BW1, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen);                 
        s3=sendto(sockfd, &L1, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 
        s4=sendto(sockfd, &V1, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 
        s5=sendto(sockfd, &NP1, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen); 
    
        recvfrom(sockfd, B1, 100, 0 , ( struct sockaddr *) &Cservaddr, &Caddrlen);    
        recvfrom(sockfd, B2, 100, 0 , ( struct sockaddr *) &Cservaddr, &Caddrlen);                             
        recvfrom(sockfd, B3, 100, 0 , ( struct sockaddr *) &Cservaddr, &Caddrlen);

        printf("The AWS received output from Backend-Server C using UDP over port < %d >  \n",AWS_UDP_PORT);

        send(new_socket, match1, 20 , 0);
        send(new_socket, B3 , 10 , 0 );
        printf("The AWS sent delay = < %s > ms to the client using TCP over port < %d > \n", B3, AWS_CLIENT_PORT);
    
        send(new_socketm, match1, 20 , 0);
        send(new_socketm, B1, 20, 0 );
        send(new_socketm, B2, 20, 0 );
        send(new_socketm, B3, 20, 0 );
        printf("The AWS server sent  detailed result to the Monitor using TCP over Port Number= < %d > \n", AWS_MONITOR_PORT);
    }
    else
    {
        printf("The AWS received < 0 > match from Backend Server B using UDP over Port Number= < %d >\n", AWS_UDP_PORT);    
    }
    


    if(strcmp(m1,"1") !=0 && strcmp(m,"1") != 0)
    {
    send(new_socket, match, 20 , 0);
    send(new_socketm, match, 20 , 0);
    sendto(sockfd, (char *)match, 20 , 0 , (const struct sockaddr *) &Cservaddr, (socklen_t )Caddrlen);
    printf("The AWS sent No Match to the Monitor and the client using TCP over ports < %d > and < %d > respectively \n", AWS_MONITOR_PORT, AWS_CLIENT_PORT);
    }

    close(sockfd);

    return 0;
}