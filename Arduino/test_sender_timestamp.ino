/*
  To create this document, the starting point was the example by Aaron Lee contained in the GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "coap.h"
#include "heltec.h"
#include "images.h"

#include <WiFi.h> 
#include "time.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

unsigned int numberPacketsSent = 0;
String rssi = "RSSI --";
String packSize = "--";

const char *ssid     = "**********"; // Write the ssid
const char *password = "**********"; // Write the password
const char* ntpServer = "north-america.pool.ntp.org";
// Variable to save current epoch time
unsigned long epochTime;

void printLocalTime(unsigned long epochTime) {
  struct timeval tv;

  if (gettimeofday(&tv, NULL) != 0) {
   Serial.println("Failed to obtain time");
   return;
  }

  double microsec = tv.tv_usec;

  double finalResult = (double)(epochTime+(microsec/1000000));

  Serial.println(finalResult, 6);
}


// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return(0);
  }
  time(&now);
  return now;
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}


void setup()
{

  Serial.begin(115200); // send and receive at 9600 baud -> BORRAR

  initWiFi();
  configTime(0, 0, ntpServer);

  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000); // It's in ms
  
}

void loop()
{

  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(numberPacketsSent));
  Heltec.display->display();

  // send packet
  LoRa.beginPacket();


/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);

  struct coap_message message;

  /* This example is: An alert from a patient's device to a doctor */ 
  message.version_type_tokenLength = (uint8_t)VERSION_REQUESTCON_13B; /* It's 68 -> explained in "coap.h" */
  message.requestOrResponse_code = (uint8_t)2; /* 00000010 -> it's defined in coap.h */
  uint16_t messageID = function_random_messageID();
  message.messageID = htons(messageID);
  uint32_t token = function_random_token();
  message.token = htonl(token);
    
  message.options.optionDelta_OptionLength = htons((uint8_t)3);
  message.options.optionDeltaExtended = (uint16_t)0;
  message.options.optionLengthExtended = (uint16_t)0; 

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

  Serial.print(message.messageID);
  Serial.print("\t");

  epochTime = getTime();
      
  LoRa.write((uint8_t*)&message, sizeof(message));

  //Serial.println(epochTime);
  printLocalTime(epochTime);

  numberPacketsSent++;
  
  //Serial.print("The number of packets sent is: ");
  //Serial.println(numberPacketsSent);
  
  LoRa.endPacket();
  
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW

  if(numberPacketsSent==100) {
    Serial.println("The 100 messages have been sent!");

    delay(60000);
  }

  delay(1000);                       // wait for a second
}
