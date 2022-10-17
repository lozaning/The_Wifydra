
#include <SoftwareSerial.h>       // include library code to allow serial communication on other digital pins of the Arduino board
#include <TinyGPS++.h>            // include the library code for GPS module
#include <Adafruit_ssd1306syp.h>  // include Adafruit_ssd1306syp library for OLED display
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SD.h>


// constants defined here:
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


static const int RXPin = 0, TXPin = 2;  // GPS module RXPin - GPIO 12 and TXPin - GPIO 13
static const uint32_t GPSBaud = 9600;   // GPS module default baud rate is 9600


const String wigleHeaderFileFormat = "WigleWifi-1.4,appRelease=2.26,model=Feather,release=0.0.0,device=arduinoWardriving,display=3fea5e7,board=esp8266,brand=Adafruit";
char *log_col_names[LOG_COLUMN_COUNT] = {
  "MAC", "SSID", "AuthMode", "FirstSeen", "Channel", "RSSI", "Latitude", "Longitude", "AltitudeMeters", "AccuracyMeters", "Type"
};

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
  uint8_t encryptionType;
  int32_t channel;
  int32_t rssi;
  int boardID;
} struct_message;



// Create a struct_message called myData
struct_message myData;

Adafruit_ssd1306syp display(D1, D2);  // create OLED display object (display (SDA, SCL)), SDA - GPIO 4, SCL - GPIO 5
TinyGPSPlus gps;                      // create the TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);      // set the software serial connection to the GPS module

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  File logFile = SD.open(logFileName, FILE_WRITE);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");

  Serial.println(len);
  Serial.print("Mac: ");
  Serial.println(myData.bssid);
  logFile.print(myData.bssid);
  logFile.print(",");

  Serial.print("SSID: ");
  Serial.println(myData.ssid);
  String SSIDString = myData.ssid;
  SSIDString.replace(",", ".");  //commas in ssid braks the csv
  logFile.print(SSIDString);
  logFile.print(",");

  Serial.print("encryption : ");
  Serial.println(myData.encryptionType);
  logFile.print(myData.encryptionType);
  logFile.print(",");

  Serial.print("first seen : ");
  Serial.println(gps.date.year());
  logFile.print(gps.date.year());
  logFile.print("-");
  Serial.println(gps.date.month());
  logFile.print(gps.date.month());
  logFile.print("-");
  Serial.println(gps.time.hour());
  logFile.print(gps.time.hour());
  logFile.print("-");
  Serial.println(gps.time.minute());
  logFile.print(gps.time.minute());
  logFile.print("-");
  Serial.println(gps.time.second());
  logFile.print(gps.time.second());
  logFile.print(",");



  Serial.print("Channel: ");
  Serial.println(myData.channel);
  logFile.print(myData.channel);
  logFile.print(",");



  Serial.print("RSSI:  ");
  Serial.println(myData.rssi);
  logFile.print(myData.rssi);
  logFile.print(",");

  Serial.print("Lat");
  Serial.println(gps.location.lat(), 8);
  logFile.print(gps.location.lat(), 8);
  logFile.print(",");




  Serial.print("Lon");
  Serial.println(gps.location.lng(), 8);
  logFile.print(gps.location.lng(), 8);
  logFile.print(",");


  Serial.print("Altitude:  ");
  Serial.println(gps.altitude.meters());
  logFile.print(gps.altitude.meters());
  logFile.print(",");




  Serial.print("HDOP:  ");
  Serial.println(gps.hdop.hdop());
  logFile.print(gps.hdop.hdop());
  logFile.print(",");


  logFile.print("WIFI");
  logFile.println(); //need ot go to a new line in the csv
  logFile.close(); // need to close the file
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
  if (myData.boardID ==2) {
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
  if (myData.boardID ==6) {
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
  if (myData.boardID ==9) {
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
  Serial.println();
  Serial.println();
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
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    display.println("Is a card inserted?");
    //display.update();
    while (true)
      ;
  }


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
  updateFileName();
  printHeader();
}
void displayinfo() {
  display.clear();
  display.setCursor(0, 0);

  display.print("LT ");
  display.print(gps.location.lat(), 1);
  //Serial.print(gps.location.lat());
  display.print(" LN");
  display.print(gps.location.lng(), 1);
  //Serial.println(gps.location.lng);
  display.print(" S");
  display.println(gps.satellites.value());

  display.print("1:");
  display.print(board1Seen);
  display.print(" | ");
  display.print("2:");
  display.print(board2Seen);
  display.print(" | ");
  display.print("3:");
  display.println(board3Seen);


  
  display.print("4:");
  display.print(board4Seen);
  display.print(" | ");
  display.print("5:");
  display.print(board5Seen);
  display.print(" | ");
  display.print("6:");
  display.println(board6Seen);

  
  display.print("7:");
  display.print(board7Seen);
  display.print(" | ");
  display.print("8:");
  display.print(board8Seen);
  display.print(" | ");
  display.print("9:");
  display.println(board9Seen);

  display.print("10:");
  display.print(board10Seen);
  display.print(" | ");
  display.print("11:");
  display.print(board11Seen);
  display.print(" | ");
  display.print("12:");
  display.println(board12Seen);

  display.print("13:");
  display.print(board13Seen);
  display.print(" | ");
  display.print("14:");
  display.println(board14Seen);







  display.print(recentSSID);
  display.print(", ");
  display.print(recentSSID1);
  display.print(", ");
  display.print(recentSSID2);
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
