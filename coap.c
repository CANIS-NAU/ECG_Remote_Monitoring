#include "coap.h"
#include <timerms.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <libgen.h>


uint16_t function_random_messageID() { // The message ID is 16 bits

    // Initialize the seed for rand() 
    unsigned short seed = time(NULL);
    srand(seed);

    uint16_t random_messageID = rand() % 65536;
    //printf("The random_messageID is: %i\n", random_messageID);

    return random_messageID;
}


uint32_t function_random_token() { // The token field is 64 bits maximum -> but in this example is going to be 32 bits

    // Initialize the seed for rand() 
    unsigned int seed = time(NULL);
    srand(seed);

    uint32_t random_token = rand() % 4294967295;
    //printf("The random_token is: %i\n", random_token);

    return random_token;
}



