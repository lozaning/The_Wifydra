/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//say how many macs we should keep in the buffer to compare for uniqueness
#define mac_history_len 512

struct mac_addr {
  unsigned char bytes[6];
};
struct mac_addr mac_history[mac_history_len];
unsigned int mac_history_cursor = 0;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {

  char bssid[64];
  char ssid[32];
  uint8_t encryptionType;
  int32_t channel;
  int32_t rssi;
  int boardID;
} struct_message;

String AP;
String BSSIDchar;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;
unsigned long timerDelay = 200;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void save_mac(unsigned char* mac) {
  //Save a MAC address into the recently seen array.
  if (mac_history_cursor >= mac_history_len) {
    mac_history_cursor = 0;
  }
  struct mac_addr tmp;
  for (int x = 0; x < 6 ; x++) {
    tmp.bytes[x] = mac[x];
  }

  mac_history[mac_history_cursor] = tmp;
  mac_history_cursor++;
  Serial.print("Mac len ");
  Serial.println(mac_history_cursor);
}

boolean seen_mac(unsigned char* mac) {
  //Return true if this MAC address is in the recently seen array.

  struct mac_addr tmp;
  for (int x = 0; x < 6 ; x++) {
    tmp.bytes[x] = mac[x];
  }

  for (int x = 0; x < mac_history_len; x++) {
    if (mac_cmp(tmp, mac_history[x])) {
      return true;
    }
  }
  return false;
}

void print_mac(struct mac_addr mac) {
  //Print a mac_addr struct nicely.
  for (int x = 0; x < 6 ; x++) {
    Serial.print(mac.bytes[x], HEX);
    Serial.print(":");
  }
}

boolean mac_cmp(struct mac_addr addr1, struct mac_addr addr2) {
  //Return true if 2 mac_addr structs are equal.
  for (int y = 0; y < 6 ; y++) {
    if (addr1.bytes[y] != addr2.bytes[y]) {
      return false;
    }
  }
  return true;
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(2, OUTPUT); //setup built in led
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  //Serial.println("Starting");
  char Buf[50];
  char bufBSSID[64];
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send

    //myData.b = random(1,20);
    //myData.c = 1.2;
    int n = WiFi.scanNetworks(false, true);
    if (n == 0) {
      Serial.println("No networks found");
      Serial.println("No networks found");
    } else {
      for (int8_t i = 0; i < n; i++) {
        if (seen_mac(WiFi.BSSID(i))) {
          Serial.println("We've already seen it");
          //BSSIDchar = WiFi.BSSID(i);
          //BSSIDchar.toCharArray(bufBSSID, 64);
          //strcpy(myData.bssid, Buf);
          //Serial.println(myData.bssid);
          Serial.println(myData.boardID);
          continue;
        }
        Serial.println("We havent seen it");
        String MacString = WiFi.BSSIDstr(i).c_str();
        //myData.bssid = MacString;
        MacString.toCharArray(bufBSSID, 64);
        strcpy(myData.bssid, bufBSSID);
        Serial.println(myData.bssid);
        //myData.bssid = WiFi.BSSID(i);
        //Serial.print("MyData.bssid: ");

        //Serial.println(myData.bssid);
        AP = WiFi.SSID(i);
        AP.toCharArray(Buf, 50);
        strcpy(myData.ssid, Buf);
        myData.encryptionType = 2;
        myData.channel = WiFi.channel(i);
        myData.rssi = WiFi.RSSI(i);
        myData.boardID = 14; //YOU NEED TO CHANGE THE BOARDID TO BE UNIQUE FOR EVERY SUB BVEFORE YOU FLASH IT. 
        Serial.println(myData.boardID);
        save_mac(WiFi.BSSID(i));
        esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        digitalWrite(2, LOW);
        delay(200);
        digitalWrite(2, HIGH);
      }



      lastTime = millis();
    }
  }
}
