#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <SD.h>

// Constants defined here:
#define LOG_FILE_PREFIX "/gpslog"
#define MAX_LOG_FILES 100
#define LOG_FILE_SUFFIX "csv"
#define LOG_COLUMN_COUNT 11
#define LOG_RATE 500
#define NOTE_DH2 661
const int chipSelect = 10;
File myFile;
char logFileName[13];
int totalNetworks = 0;
unsigned long lastLog = 0;

// Static const int RXPin = 0, TXPin = 2;  // GPS module RXPin - GPIO 12 and TXPin - GPIO 13
static const uint32_t GPSBaud = 9600;   // GPS module default baud rate is 9600

const String wigleHeaderFileFormat = "WigleWifi-1.4,appRelease=2.26,model=Feather,release=0.0.0,device=The_Wifydra,display=3fea5e7,board=esp8266,brand=Adafruit";
char* log_col_names[LOG_COLUMN_COUNT] = {
  "MAC", "SSID", "AuthMode", "FirstSeen", "Channel", "RSSI", "CurrentLatitude", "CurrentLongitude", "AltitudeMeters", "AccuracyMeters", "Type"
};

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

String recentSSID;
String recentSSID1;
String recentSSID2;
String board1SSID;
String board2SSID;
String board3SSID;
String board4SSID;
String board5SSID;
String board6SSID;
String board7SSID;
String board8SSID;
String board9SSID;
String board10SSID;
String board11SSID;
String board12SSID;
String board13SSID;
String board14SSID;

int board1Seen = 0;
int board2Seen = 0;
int board3Seen = 0;
int board4Seen = 0;
int board5Seen = 0;
int board6Seen = 0;
int board7Seen = 0;
int board8Seen = 0;
int board9Seen = 0;
int board10Seen = 0;
int board11Seen = 0;
int board12Seen = 0;
int board13Seen = 0;
int board14Seen = 0;

void updateFileName() {
  int i = 0;
  for (; i < MAX_LOG_FILES; i++) {
    memset(logFileName, 0, strlen(logFileName));
    sprintf(logFileName, "%s%d.%s", LOG_FILE_PREFIX, i, LOG_FILE_SUFFIX);
    if (!SD.exists(logFileName)) {
      Serial.println("we picked a new file name");
      Serial.println(logFileName);
      break;
    } else {
      Serial.print(logFileName);
      Serial.println(" exists");
    }
  }
  Serial.println("File name: ");
  Serial.println(logFileName);
}

void printHeader() {
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    int i = 0;
    logFile.println(wigleHeaderFileFormat);  // comment out to disable Wigle header
    for (; i < LOG_COLUMN_COUNT; i++) {
      logFile.print(log_col_names[i]);
      if (i < LOG_COLUMN_COUNT - 1)
        logFile.print(',');
      else
        logFile.println();
    }
    logFile.close();
  }
}

typedef struct struct_message {
  char bssid[64];
  char ssid[32];
  char encryptionType[16];
  int32_t channel;
  int32_t rssi;
  int boardID;
} struct_message;

// Create a struct_message called myData
struct_message myData;

HardwareSerial GPSSerial(2);
TinyGPSPlus gps;

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  File logFile = SD.open(logFileName, FILE_APPEND);
  memcpy(&myData, incomingData, sizeof(myData));
  char buffer[10];
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Mac: ");
  Serial.println(myData.bssid);
  logFile.print(myData.bssid);
  logFile.print(",");
  Serial.print("SSID: ");
  Serial.println(myData.ssid);
  String SSIDString = myData.ssid;
  if(SSIDString.indexOf(",") > 0) {
    SSIDString = "\""+SSIDString+"\""
  }
  logFile.print(SSIDString);
  logFile.print(",");
  Serial.print("Encryption: ");
  Serial.println(myData.encryptionType);
  logFile.print(myData.encryptionType);
  logFile.print(",");
  Serial.print("First seen: ");

  //date
  Serial.println(gps.date.year());
  logFile.print(gps.date.year());
  logFile.print("-");

  snprintf(buffer , sizeof buffer, "%02d", gps.time.month());
  Serial.print(buffer);
  logFile.print(buffer);
  logFile.print("-");

  snprintf(buffer , sizeof buffer, "%02d", gps.time.day());
  Serial.print(buffer);
  logFile.print(buffer);

  logFile.print(" ");

  //time
  snprintf(buffer , sizeof buffer, "%02d", gps.time.hour());
  Serial.print(buffer);
  logFile.print(buffer);
  logFile.print(":");

  snprintf(buffer , sizeof buffer, "%02d", gps.time.minute());
  Serial.print(buffer);
  logFile.print(buffer);
  logFile.print(":");

  snprintf(buffer , sizeof buffer, "%02d", gps.time.second());
  Serial.print(buffer);
  logFile.print(buffer);

  logFile.print(",");
  Serial.print("Channel: ");
  Serial.println(myData.channel);
  logFile.print(myData.channel);
  logFile.print(",");
  Serial.print("RSSI:  ");
  Serial.println(myData.rssi);
  logFile.print(myData.rssi);
  logFile.print(",");
  Serial.print("Lat: ");
  Serial.println(gps.location.lat(), 8);
  logFile.print(gps.location.lat(), 8);
  logFile.print(",");
  Serial.print("Lon: ");
  Serial.println(gps.location.lng(), 8);
  logFile.print(gps.location.lng(), 8);
  logFile.print(",");
  Serial.print("Altitude: ");
  integerAltMeters = int(round(gps.altitude.meters()));
  Serial.println(integerAltMeters);
  logFile.print(integerAltMeters);
  logFile.print(",");
  Serial.print("HDOP: ");
  Serial.println(gps.hdop.value());
  logFile.print(gps.hdop.value());
  Serial.print("BiD: ");
  Serial.println(myData.boardID);  
  logFile.print(".0,");
  logFile.print("WIFI");
  logFile.println();
  logFile.close();
  recentSSID2 = recentSSID1;
  recentSSID1 = recentSSID;
  recentSSID = myData.ssid;
  if (myData.boardID == 1) {
    board1SSID = myData.ssid;
    Serial.println("Board 1 SSID: ");
    Serial.println(board1SSID);
    board1Seen += 1;
    Serial.println(board1Seen);
  }
  if (myData.boardID == 2) {
    board2SSID = myData.ssid;
    Serial.println("Board 2 SSID: ");
    Serial.println(board2SSID);
    board2Seen += 1;
    Serial.println(board2Seen);
  }
  if (myData.boardID == 3) {
    board3SSID = myData.ssid;
    Serial.println("Board 3 SSID: ");
    Serial.println(board3SSID);
    board3Seen += 1;
    Serial.println(board3Seen);
  }
  if (myData.boardID == 4) {
    board4SSID = myData.ssid;
    Serial.println("Board 4 SSID: ");
    Serial.println(board4SSID);
    board4Seen += 1;
    Serial.println(board4Seen);
  }
  if (myData.boardID == 5) {
    board5SSID = myData.ssid;
    Serial.println("Board 5 SSID: ");
    Serial.println(board5SSID);
    board5Seen += 1;
    Serial.println(board5Seen);
  }
  if (myData.boardID == 6) {
    board6SSID = myData.ssid;
    Serial.println("Board 6 SSID: ");
    Serial.println(board6SSID);
    board6Seen += 1;
    Serial.println(board6Seen);
  }
  if (myData.boardID == 7) {
    board7SSID = myData.ssid;
    Serial.println("Board 7 SSID: ");
    Serial.println(board7SSID);
    board7Seen += 1;
    Serial.println(board7Seen);
  }
  if (myData.boardID == 8) {
    board8SSID = myData.ssid;
    Serial.println("Board 8 SSID: ");
    Serial.println(board8SSID);
    board8Seen += 1;
    Serial.println(board8Seen);
  }
  if (myData.boardID == 9) {
    board9SSID = myData.ssid;
    Serial.println("Board 9 SSID: ");
    Serial.println(board9SSID);
    board9Seen += 1;
    Serial.println(board9Seen);
  }
  if (myData.boardID == 10) {
    board10SSID = myData.ssid;
    Serial.println("Board 10 SSID: ");
    Serial.println(board10SSID);
    board10Seen += 1;
    Serial.println(board10Seen);
  }
  if (myData.boardID == 11) {
    board11SSID = myData.ssid;
    Serial.println("Board 11 SSID: ");
    Serial.println(board11SSID);
    board11Seen += 1;
    Serial.println(board11Seen);
  }
  if (myData.boardID == 12) {
    board12SSID = myData.ssid;
    Serial.println("Board 12 SSID: ");
    Serial.println(board12SSID);
    board12Seen += 1;
    Serial.println(board12Seen);
  }
  if (myData.boardID == 13) {
    board13SSID = myData.ssid;
    Serial.println("Board 13 SSID: ");
    Serial.println(board13SSID);
    board13Seen += 1;
    Serial.println(board13Seen);
  }
  if (myData.boardID == 14) {
    board14SSID = myData.ssid;
    Serial.println("Board 14 SSID: ");
    Serial.println(board14SSID);
    board14Seen += 1;
    Serial.println(board14Seen);
  }
  Serial.println();
 totalNetworks +=1;
 Serial.print("Total Networks: ");
 Serial.println(totalNetworks);

  Serial.println();
  Serial.println();
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  GPSSerial.begin(GPSBaud, SERIAL_8N1, 2, 1);
  Serial.println("GPS initialized");

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(0, 0);
  

  if (!SD.begin(chipSelect)) {
    tft.println("Initialization failed. Things to check:");
    tft.println("1. Is a card inserted?");
   
    while (true)
      ;
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  updateFileName();
  printHeader();
  void displayinfo();
}





void displayinfo() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextWrap(true);
  tft.setTextSize(1);
  tft.print("LT ");
  tft.print(gps.location.lat(), 1);
  //Serial.print(gps.location.lat());
  tft.print(" LN");
  tft.print(gps.location.lng(), 1);
  //Serial.println(gps.location.lng);
  tft.print(" S");
  tft.println(gps.satellites.value());

  tft.print("1:");
  tft.print(board1Seen);
  tft.print(" | ");
  tft.print("2:");
  tft.println(board2Seen);
  

  tft.print("3:");
  tft.print(board3Seen);
  tft.print(" | ");
  tft.print("4:");
  tft.println(board4Seen);
  
  
  tft.print("5:");
  tft.print(board5Seen);
  tft.print(" | ");
  tft.print("6:");
  tft.println(board6Seen);

  
  tft.print("7:");
  tft.print(board7Seen);
  tft.print(" | ");
  tft.print("8:");
  tft.println(board8Seen);

  
  tft.print("9:");
  tft.print(board9Seen);
  tft.print(" | ");
  tft.print("10:");
  tft.println(board10Seen);
  

  tft.print("11:");
  tft.print(board11Seen);
  tft.print(" | ");
  tft.print("12:");
  tft.println(board12Seen);

  tft.print("13:");
  tft.print(board13Seen);
  tft.print(" | ");
  tft.print("14:");
  tft.println(board14Seen);







  tft.print(recentSSID);
  tft.print(", ");
  tft.print(recentSSID1);
  tft.print(", ");
  tft.println(recentSSID2);
  tft.println();
  tft.print("Nets: ");
  tft.print(totalNetworks);
  
/**
  display.println("");
  display.print(board1SSID);
  display.print(", ");
  display.print(board2SSID);
  display.print(", ");
  display.print(board3SSID);
  display.print(", ");
  display.print(board4SSID);
  display.print(", ");
  display.print(board5SSID);
  display.print(", ");
  display.print(board6SSID);
  display.print(", ");
  display.print(board7SSID);
  display.print(", ");

*/

  /*


  display.update();
  delay(10);
*/  


}
void loop() {
  //displayinfo();   // run procedure displayinfo
  smartDelay(5000);  // run procedure smartDelay
  displayinfo();  
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    //display.setCursor(0, 0);
    //display.println("No GPS detected:");
    //display.println(" check wiring.");
    
  }
}
static void smartDelay(unsigned long ms)  // custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do {
    while (GPSSerial.available())
      gps.encode(GPSSerial.read());
  } while (millis() - start < ms);
}
