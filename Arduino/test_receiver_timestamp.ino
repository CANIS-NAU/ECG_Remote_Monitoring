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

String rssi = "RSSI --";
String packSize = "--";

unsigned int numberPacketsReceived = 0;

struct coap_message* message = NULL;

const char *ssid     = "***********"; // Write the ssid
const char *password = "***********"; // Write the password

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
  //Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  
  //Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  
  packSize = String(packetSize,DEC);

  unsigned char buffer[packetSize];

  for (int i = 0; i < packetSize; i++) { 
    buffer[i] = (unsigned char) LoRa.read(); 
  }

  message = (struct coap_message *) buffer;
  
  Serial.print(message->messageID);
  Serial.print("\t");

  //Serial.print(epochTime);
  printLocalTime(epochTime);
  
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  LoRaData();
}

void setup() { 

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

  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) { 

    epochTime = getTime();
  
    numberPacketsReceived++;
    //Serial.print("The number of packets received is: ");
    //Serial.println(numberPacketsReceived);
    cbk(packetSize);  
  }

  delay(1000);                       // wait for a second
}
