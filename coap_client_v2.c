#include "coap.h"
#include "utf8.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024


int main() {

    /*
    UDP CLIENT STEPS:
    1) Create a UDP socket
    2) Send a message to the server 
    */

    // 1) Create a UDP socket
    int sockfd;
    //char buffer[MAXLINE];
    //char *hello = "Hello from client";
    struct sockaddr_in servaddr;
   
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
       
    //int n, len;




    /* Create the CoAP frame and fill in all the fields */
    struct coap_message message;
    memset(&message, 0, sizeof(message));

    // This example is: An alert from a patient's device to a doctor
    message.version_type_tokenLength = VERSION_REQUESTCON_13B; // It's 68 -> explained in "coap.h"
    message.requestOrResponse_code = REQUEST_POST; // 00000010 -> it's defined in coap.h
    uint16_t messageID = (uint16_t)function_random_messageID();
    message.messageID = htons(messageID);
    printf("The message id in the coap_client_v2 is: %d\n", messageID);
    uint32_t token = (uint32_t)function_random_token();
    /*printf("The size of the uint32_t is: %ld\n", sizeof(uint32_t));
    printf("The size of the unsigned int is: %ld\n", sizeof(unsigned int));
    printf("The size of the int is: %ld\n", sizeof(int)); */
    message.token = htonl(token);
    printf("The message token in the coap_client_v2 is: %d\n", token); 

    
    /* 
    This is for the options:
    int u8_toutf8(char *dest, int sz, uint32_t *src, int srcsz);
    When we know the uri that we want to encode with UTF8 (depending on how many options there are) we can do size of the Option Value
    to put it in the Option Length -> so first we encode the value of the Option Value
    */
    /*
    char *dest;
    int sz;
    uint32_t *src;
    int srcsz;
    */
    
    

    message.options.optionDelta_OptionLength = (uint8_t)3;
    //message.options.optionDeltaExtended = (uintptr_t) NULL;
    message.options.optionDeltaExtended = 0;
    //memcpy(message.options.optionDeltaExtended, (uintptr_t)NULL, sizeof(uint16_t));

    message.options.optionLengthExtended = 0;

    /* Generate the option value field */
    unsigned char optionValue[MAX_OPTIONS];
    for (int i=0; i<MAX_OPTIONS; i++) {
        message.options.optionValue[i] = (unsigned char) i;
    } 

    message.payloadMarker = PAYLOAD_MARKER; 

    /* Generate the payload */
    unsigned char payload[COAP_PAYLOAD];
    for (int i=0; i<COAP_PAYLOAD; i++) {
        message.payload_coap[i] = (unsigned char) i;
    } 


    // printf("The size of message is: %ld\n", sizeof(message));

       
    sendto(sockfd, (struct coap_message *) &message, sizeof(message), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("CoAP message sent from the client.\n");
           
    /*
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer); 
    */
   
    close(sockfd);
    return 0; 
}



