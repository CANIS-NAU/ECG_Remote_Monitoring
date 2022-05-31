#include "coap.h"
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main() {

    /*
    UDP CLIENT STEPS:
    1) Create a UDP socket
    2) Send a message to the server 
    */

    // 1) Create a UDP socket
    int sockfd;
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
       

    /* Create the CoAP frame and fill in all the fields */
    struct coap_message message;
    memset(&message, 0, sizeof(message));
    printf("The size of the message is: %ld\n", sizeof(message));

    // This example is: An alert from a patient's device to a doctor
    message.version_type_tokenLength = (uint8_t)VERSION_REQUESTCON_13B; // It's 68 -> explained in "coap.h"
    message.requestOrResponse_code = (uint8_t)REQUEST_POST; // 00000010 -> it's defined in coap.h
    uint16_t messageID = (uint16_t)function_random_messageID();
    message.messageID = htons(messageID);
    printf("The message id in the coap_client_v2 is: %d\n", messageID);
    uint32_t token = (uint32_t)function_random_token();
    message.token = htonl(token);
    printf("The message token in the coap_client_v2 is: %d\n", token); 

    message.options.optionDelta_OptionLength = (uint8_t)3;
    //message.options.optionDeltaExtended = (uintptr_t) NULL;
    message.options.optionDeltaExtended = (uint16_t)2;
    //message.options.optionDeltaExtended = htons(messageID); // DEBUG
    //memcpy(message.options.optionDeltaExtended, (uintptr_t)NULL, sizeof(uint16_t));

    message.options.optionLengthExtended = (uint16_t)2; 
    //message.options.optionLengthExtended = htons(messageID); // DEBUG

    /* Generate the option value field */
    unsigned char optionValue[MAX_OPTIONS];
    for (int i=0; i<MAX_OPTIONS; i++) {
        message.options.optionValue[i] = (unsigned char) i;
    } 

    message.payloadMarker = (uint8_t)PAYLOAD_MARKER; 

    /* Generate the payload */
    unsigned char payload[COAP_PAYLOAD];
    for (int i=0; i<COAP_PAYLOAD; i++) {
        message.payload_coap[i] = (unsigned char) i;
    } 

    // printf("The size of message is: %ld\n", sizeof(message));

    sendto(sockfd, (struct coap_message *) &message, sizeof(message), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("CoAP message sent from the client.\n");
   
    close(sockfd);
    return 0; 
}



