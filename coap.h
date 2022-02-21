#ifndef _CoAP_H
#define _CoAP_H

#include <stdint.h>

#define COAP_PAYLOAD 13 // 13 bytes

#define MAX_OPTIONS 15 // 15 bytes maximum of option value

#define COAP_PORT 5683 // For the transport protocol UDP or TCP this is the port

/*
- VERSION: it's always 01
- TYPE: in this case it's a request and confirmable so 00
- TOKEN LENGHT: in this case is 13 bytes
The final answer is 01000100 which is number 68
*/
#define VERSION_REQUESTCON_13B 68 // The version is always

#define REQUEST_POST 0x02 // The class is 0 and the "dd" is 02 that is a POST so the final value is 00000010

#define PAYLOAD_MARKER 0xFF // Always this value


/* The option field:
    - The minimum (mandatory) is 8 bits (option delta and option length)
    - The maximum is:
        - 8 bits of the option delta and option length
        - 16 bits of the option delta (extended)
        - 16 bits of the option length (extended)
        - The size of option value is defined in the option lenght (in bytes) so: the maximum is 15 bytes which is 120 bits. 
        So it's going to be 2 uint_64
*/
struct options_message { // Another struct for the options field

    uint8_t optionDelta_OptionLength;
    uint16_t optionDeltaExtended;
    uint16_t optionLengthExtended;

    /* 
    I'm not sure about this because the maximum value of this field is 120 bits but there is not a value uint120_t so the next 
    value is 128 bits, so 2 units of uint64_t but it's 120 bits not more
    */
    // Look an array !!!!
    // It can be an array of "unsigned char because each one is 1 byte"
    unsigned char optionValue[MAX_OPTIONS]; // 15 bytes of maximum option value 
}; 


// This is CoAP's message
struct coap_message {

    uint8_t version_type_tokenLength;
    uint8_t requestOrResponse_code;
    uint16_t messageID;
    uint32_t token; // It can go from 0 to 8 bytes
    struct options_message options; 
    uint8_t payloadMarker; // Always 0xFF
    unsigned char payload_coap[COAP_PAYLOAD]; // 13 bytes de payload 
};



int function_random_messageID();
int function_random_token();

#endif /* _CoAP_H */

