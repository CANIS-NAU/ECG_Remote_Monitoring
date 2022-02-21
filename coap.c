#include "coap.h"
#include "utf8.h"

#include <timerms.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <libgen.h>


#define COAP_HEADER_SIZE_WITHOUT_OPTIONS 12 // In bytes

// CAREFUL with this value look at the "Implementation Note" at the end of section 3 (Message Format) in the RFC
#define PAYLOAD_MARKER 0xFF // The payload marker is always 11111111 just before the payload



int function_random_messageID() { // The message ID are 16 bits

    // Initialize the seed for rand() 
    unsigned int seed = time(NULL);
    srand(seed);

    // Generate random number between 0 and RAND_MAX
	int dice = rand();
	
	int random_messageID = 0 + (int) (65535.0 * dice / (RAND_MAX)); // This field has 16 bits (2^16-1)
	//printf("%i\n", dice);
    
    //printf("El resultado del random message id es: %d\n", random_messageID);

    return random_messageID;
}


int function_random_token() { // The token field are 64 bits maximum -> but in this example is going to be 32 bits

    // Initialize the seed for rand() 
    unsigned int seed = time(NULL);
    srand(seed);

    // Generate random number between 0 and RAND_MAX
	int dice = rand();
	
    int random_token = 0 + (int) (4294967295.0 * dice / (RAND_MAX)); // This field has 32 bits (2^32-1)
	//printf("%i\n", dice);

    printf("The result of the random token is: %d\n", random_token);

    return random_token;
}

