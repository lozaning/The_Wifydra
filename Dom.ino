/*
  Project: ESP8266 ESP-12E module, NEO-6M GY-GPS6MV2 GPS module, 0.96" I2C OLED display module
  Function: This sketch listen to the GPS serial port,
  and when data received from the module, it's displays GPS data on 0.96" I2C OLED display module.

  ESP8266 ESP-12E module -> NEO-6M GY-GPS6MV2 GPS module

  VV (5V)     VCC
  GND         GND
  D6 (GPIO12) TX
  D7 (GPIO13) RX

  ESP8266 ESP-12E module -> White 0.96" I2C OLED display module

  3V3        VCC
  GND        GND
  D1 (GPIO5) SCL
  D2 (GPIO4) SDA

*/
#include <SoftwareSerial.h>       // include library code to allow serial communication on other digital pins of the Arduino board
#include <TinyGPS++.h>            // include the library code for GPS module
#include <Adafruit_ssd1306syp.h>  // include Adafruit_ssd1306syp library for OLED display
#include <ESP8266WiFi.h>
#include <espnow.h>
// constants defined here:

static const int RXPin = 12, TXPin = 13;                                 // GPS module RXPin - GPIO 12 and TXPin - GPIO 13
static const uint32_t GPSBaud = 9600;                                    // GPS module default baud rate is 9600
static const double WAYPOINT_LAT = 51.508131, WAYPOINT_LON = -0.128002;  // change coordinates of your waypoint

String board1SSID;
String board2SSID;

typedef struct struct_message {
  char a[32];
  int b;
  float c;
  //String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

Adafruit_ssd1306syp display(4, 5);  // create OLED display object (display (SDA, SCL)), SDA - GPIO 4, SCL - GPIO 5
TinyGPSPlus gps;                    // create the TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);    // set the software serial connection to the GPS module

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("SSID: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  if (myData.b == 1) {
    board1SSID = myData.a;
    Serial.println("Board 1 SSID: ");
    Serial.println(board1SSID);
  }
  if (myData.b == 2) {
    board2SSID = myData.a;
    Serial.println("Board 2 SSID: ");
    Serial.println(board2SSID);
  }
  Serial.print("Float: ");
  Serial.println(myData.c);
  //Serial.print("String: ");
  //Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.println();
  void displayinfo();
}

void setup() {
  display.initialize();                   // initialize OLED display
  display.clear();                        // clear OLED display
  display.setTextColor(WHITE);            // set the WHITE color
  display.setTextSize(1);                 // set the font size 1
  display.setCursor(0, 0);                // set the cursor to the OLED display top left corner (column 0, row 0)
  display.println("Acoptex GPS-Timing");  // print text on OLED display
  display.println("Check for more");      //
  display.println("DIY projects on");     //
  display.println("http://acoptex.com");  //
  display.update();                       // update OLED display
  delay(3000);                            // set delay for 3 seconds
  ss.begin(GPSBaud);                      // initialise serial communication with GPS module
                                          // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  void displayinfo();
}
void displayinfo() {
  display.clear();
  display.setCursor(0, 0);
  //display.print("DATE:    ");
  //display.print(gps.date.day());
  //display.print("-");
  //display.print(gps.date.month());
  //display.print("-");
  //display.println(gps.date.year());
  display.print("LATITUDE : ");
  display.println(gps.location.lat(), 5);
  display.print("LONGITUDE: ");
  display.println(gps.location.lng(), 5);
  display.println("Radio 1: ");
  display.println(board1SSID);
  display.println("Radio 2: ");
  display.println(board2SSID);
  
  //display.print(gps.altitude.feet(), 2);
  //display.println(" ft");
  display.print("SATELLITE: ");
  display.println(gps.satellites.value());

 
  display.update();
  delay(10);
}
void loop() {
  displayinfo();   // run procedure displayinfo
  smartDelay(50);  // run procedure smartDelay
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    display.setCursor(0, 0);
    display.println("No GPS detected:");
    display.println(" check wiring.");
    display.println("More projects are");
    display.println(" on Acoptex.com");
  }
}
static void smartDelay(unsigned long ms)  // custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}