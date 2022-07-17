// MAC Address of responder - edit as required
// uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xF9, 0x0E, 0xF4}; 0x0E, 0x28}
 
//ESP32
 
#include <esp_now.h>
#include <WiFi.h>

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xF9, 0x0E, 0x28};
 
  
//Displays
// ST7789 libraries//
#include <SPI.h>            // for use with ST7789 using TFT_eSPI library
#include <TFT_eSPI.h>       // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

// This CODE has been updated to use 2 encoders with the built-in switch and 
// posts to the serial monitor. 

#include <rotary.h>

// Rotary encoder is wired with the common to ground and the three
// outputs to pins A0, A1 and A2.

Rotary rotary1 = Rotary(34,35, 32);

Rotary rotary2 = Rotary(33,25, 26);
 
// Counter that will be incremented or decremented by rotation.
int OWNcounter1 = 0;
int OWNcounter2 = 0;

int OWNSW1 = 0;
int OWNSW2 = 0;

// DATA STRUCTURE TRANSMIT // 

typedef struct TxStruct     // Define a data structure
{
  int OWNcounter1;
  int OWNcounter2;

  int OWNSW1;
  int OWNSW2;
  
 } TxStruct;

// Create a structured object
TxStruct sentData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//  Serial.print("\r\nLast Packet Send Status:\t");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// DATA STRUCTURE RECEIVE // 

typedef struct RxStruct
{
  int OWNcounter1;
  int OWNcounter2;

  int OWNSW1;
  int OWNSW2;
  
}RxStruct;
RxStruct receivedData;

//-------------------------------------------------------------------------------------
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&receivedData, incomingData, sizeof(receivedData));
}
//======================================================================================

 void setup() { 

 // Setup Serial Monitor and encoders
  Serial.begin(250000);
  Serial.println("Start...");

// SET-UP CODE for ST7789
 tft.init();
 tft.setRotation(1);
 tft.fillScreen(TFT_BLACK);
 
 tft.setCursor(100, 0, 4);            // Code replaced by OWN and OPP
 tft.setTextColor(TFT_WHITE, TFT_BLACK);
 tft.println("OPP");
 
  tft.setCursor(0, 20, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("val1: ");  

  tft.setCursor(120, 20, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("sw1: ");  

  tft.setCursor(0, 40, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("val2: ");  

  tft.setCursor(120, 40, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("sw2: ");  

//**own
tft.setCursor(100, 60, 4);            // Code replaced by OWN and OPP
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.println("OWN");
 
  tft.setCursor(0, 80, 4);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("val1: ");  

  tft.setCursor(120, 80, 4);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("sw1: ");  

  tft.setCursor(0, 100, 4);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("val2: ");  

  tft.setCursor(120, 100, 4);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("sw2: ");  


// SET-UP CODE for ESP32 wifi
 WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }     
} 
 
 void loop() { 
 //******************//************* TRANSMIT SECTION 3
 sentData.OWNcounter1 = OWNcounter1;
  sentData.OWNcounter2 = OWNcounter2;

 sentData.OWNSW1 = OWNSW1;
  sentData.OWNSW2 = OWNSW2;
  
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &sentData, sizeof(sentData));
   
 /* if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(200  );   /// HAHAHAHAHA THIS STUPID DELAY SCREWED THE ENCODERS UP!!
  */
  //******************//************* End TRANSMIT SECTION 3

//**** ENCODER1 and ENCODER2****
 unsigned char result1 = rotary1.process();
  if (result1 == DIR_CW) {
    OWNcounter1++;
    
    Serial.println(OWNcounter1);
    
  } else if (result1 == DIR_CCW) {
    OWNcounter1--;
        Serial.println(OWNcounter1);
  }
   // Check to see if the button has been pressed.
  // Passes in a debounce delay of 20 milliseconds
  
   if (rotary1.buttonPressedReleased(20)) {
    Serial.println("Push button1");
    OWNSW1 = 1;
    
   }
  //endif buttonPressedReleased

//// Encoder2
    unsigned char result2 = rotary2.process();
  if (result2 == DIR_CW) {
    OWNcounter2++;
    Serial.println(OWNcounter2);
  } else if (result2 == DIR_CCW) {
    OWNcounter2--;
    Serial.println(OWNcounter2);
  }
 
   if (rotary2.buttonPressedReleased(20)) {
    Serial.println("Push button2");
    OWNSW2 = 1;
   } 
  //endif buttonPressedReleased

//******* Active code ST7789 display*********//
//OWN 
  
 tft.setCursor(60, 80, 4); 
 tft.setTextColor(TFT_RED, TFT_BLACK);
 tft.println(OWNcounter1);

 tft.setCursor(180, 80, 4); 
 tft.setTextColor(TFT_RED, TFT_BLACK);
 tft.println(OWNSW1);

tft.setCursor(60, 100, 4); 
 tft.setTextColor(TFT_GREEN, TFT_BLACK);
 tft.println(OWNcounter2);

 tft.setCursor(180, 100, 4); 
 tft.setTextColor(TFT_GREEN, TFT_BLACK);
 tft.println(OWNSW2);

tft.setCursor(60, 20, 4); 
 tft.setTextColor(TFT_RED, TFT_BLACK);
 tft.println(receivedData.OWNcounter1);

tft.setCursor(180, 20, 4); 
 tft.setTextColor(TFT_RED, TFT_BLACK);
 tft.println(receivedData.OWNSW1);

 
tft.setCursor(60, 40, 4); 
 tft.setTextColor(TFT_GREEN, TFT_BLACK);
 tft.println(receivedData.OWNcounter2);

tft.setCursor(180, 40, 4); 
 tft.setTextColor(TFT_GREEN, TFT_BLACK);
 tft.println(receivedData.OWNSW2);
   
 }
