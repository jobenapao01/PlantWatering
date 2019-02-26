#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <string>
using namespace std;

#define WIFI_SSID "SHEENA" //Wifi SSID
#define WIFI_PASSWORD "HYDROCORTISONE2005" //Wifi password

#define FIREBASE_HOST "pwsystem-6e02c.firebaseio.com"
#define FIREBASE_AUTH "75rzCPuqnO0hiRjwAIcTf6Numsdc1Kn7ghIXftfn"

#define pump 14 //d5
#define soilMoisture 13 //D7
#define waterSensor 15 // D8


WiFiClient client;
int soilOutput = 0;
const long utcOffsetInSeconds = 28800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

FirebaseObject obj = Firebase.get("MainSchedule");

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.pagasa.dost.gov.ph", utcOffsetInSeconds);

void setup() {
  Serial.begin(115290);
  initializeWifi();
  initializePins();
  timeClient.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  blinkLed();
  timeDisplay();
}

void timeDisplay() {
  //FirebaseObject obj = Firebase.get("MainSchedule");
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  String tuesday = obj.getString("tuesday");
  String t[tuesday.length()];
 
  int t1 = 0;
  for (int i = 0; i < tuesday.length(); i++) {
    if (tuesday.substring(i, i + 1) == ",") {
      t[t1] = tuesday.substring(i - 5, i);
      t1++;
    }
  }
  t[t1] = tuesday.substring(tuesday.length() - 5, tuesday.length());

  for (int i = 0; i < t1; i++) {
//    if (curTime == t[i]) {
//      Serial.println("SUCCESS");
//    }
  }
  //  switch(dow){
  //    case 1:
  //      Serial.println("monday");
  //      break;
  //    case 2:
  //     Serial.println("tuesday");
  ////     String t[tuesday.length()];
  ////      int t1 = 0;
  ////       for (int i = 0; i < tuesday.length(); i++) {
  ////        if (tuesday.substring(i, i+1) == ",") {
  ////          t[t1] = tuesday.substring(i-5,i);
  ////          t1++;
  ////        }
  ////      }
  ////      t[t1] = tuesday.substring(tuesday.length()-5, tuesday.length());
  //      break;
  //    case 3:
  //     Serial.println("wednesday");
  //      break;
  //    case 4:
  //     Serial.println("thursday");
  //      break;
  //    case 5:
}
void pumpPower() {
  //TODO: save logs when pump is on
  int pumpStatus = Firebase.getString("Pump/pumpState").toInt();
  Serial.println(pumpStatus);
  if (pumpStatus == 1) {
    digitalWrite(pump, HIGH);
    Serial.println("Relay is open");
    if (digitalRead(soilMoisture) != HIGH) {
      Firebase.setString("Pump/pumpState", "0");
    }
  } else if (pumpStatus == 0) {
    digitalWrite(pump, LOW);
    Serial.println("Relay is closed");
  }
  //  if (digitalRead(soilMoisture) != HIGH){
  //    Firebase.setInt("Pump/pumpValue", 0);
  //    Serial.println("Relay is Off");
  //   }
}




void waterValue() {
  int waterLevel = digitalRead(waterSensor);

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println("%");
}

void blinkLed() {
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}

void sendSoilData() {
  if (digitalRead(soilMoisture) == HIGH) {
    Serial.println("DRY");
    Firebase.setString("Soil/soilValue", "DRY");
  }
  //Firebase.setInt();
  else if (digitalRead(soilMoisture) == LOW) {
    Serial.println("WET");
    Firebase.setString("Soil/soilValue", "WET");
  }
  else
    Serial.println("Error");
}

void initializeWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected");
  Serial.println(WiFi.localIP());
}

void initializePins() {
  pinMode(pump, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(soilMoisture, INPUT);
}

void getSchedule() {
  //String sched = Firebase.get("/");
  FirebaseObject obj = Firebase.get("MainSchedule");
  String Monday = obj.getString("monday");
  String Tuesday = obj.getString("tuesday");
  String Wednesday = obj.getString("wednesday");
  String Thursday = obj.getString("thursday");
  String Friday = obj.getString("friday");
  String Saturday = obj.getString("saturday");
  String Sunday = obj.getString("sunday");
}
void addData() {
  String key = Firebase.push("Logs", "Test");
}

void mondaySched(){
  String monday = obj.getString("monday");
  String m[monday.length()];
  int m1 = 0;
   for (int i = 0; i < monday.length(); i++) {
    if (monday.substring(i, i+1) == ",") {
      m[m1] = monday.substring(i-5,i);
      m1++;
    }
  }
  m[m1] = monday.substring(monday.length()-5, monday.length());
  for (int i = 0; i < m1; i++) {

    
  }
}
//
//void tuesdaySched(){
//  RtcDateTime currentTime = rtcObject.GetDateTime();    //get the time from the RTC
//  String tuesday = obj.getString("tuesday");
//  String t[tuesday.length()];
//  int t1 = 0;
//   for (int i = 0; i < tuesday.length(); i++) {
//    if (tuesday.substring(i, i+1) == ",") {
//      t[t1] = tuesday.substring(i-5,i);
//      t1++;
//    }
//  }
//  t[t1] = tuesday.substring(tuesday.length()-5, tuesday.length());
//  String st = t[t1];
//  String curTime = currentTime.Hour() + ":" + currentTime.Minute();
//    for(int i=0; i<st[t1]; i++){
//      pumpPower();
//   }
//}
//
//void wednesdaySched(){
//  String wednesday = obj.getString("wednesday");
//  String w[wednesday.length()];
//  int w1 = 0;
//   for (int i = 0; i < wednesday.length(); i++) {
//    if (wednesday.substring(i, i+1) == ",") {
//      w[w1] = wednesday.substring(i-5,i);
//      w1++;
//    }
//  }
//  w[w1] = wednesday.substring(wednesday.length()-5, wednesday.length());
//}
//
//void thursdaySched(){
//   String thursday = obj.getString("thursday");
//  String th[thursday.length()];
//  int th1 = 0;
//   for (int i = 0; i < thursday.length(); i++) {
//    if (thursday.substring(i, i+1) == ",") {
//      th[th1] = thursday.substring(i-5,i);
//      th1++;
//    }
//  }
//  th[th1] = thursday.substring(thursday.length()-5, thursday.length());
//}
//
//void fridaySched(){
//  String friday = obj.getString("friday");
//  String f[friday.length()];
//  int f1 = 0;
//   for (int i = 0; i < friday.length(); i++) {
//    if (friday.substring(i, i+1) == ",") {
//      f[f1] = friday.substring(i-5,i);
//      f1++;
//    }
//  }
//  f[f1] = friday.substring(friday.length()-5, friday.length());
//}
//
//void saturdaySched(){
//  String saturday = obj.getString("saturday");
//  String s[saturday.length()];
//  int s1 = 0;
//   for (int i = 0; i < saturday.length(); i++) {
//    if (saturday.substring(i, i+1) == ",") {
//      s[s1] = saturday.substring(i-5,i);
//      s1++;
//    }
//  }
//  s[s1] = saturday.substring(saturday.length()-5, saturday.length());
//}
//
//void sundaySched(){
//  String sunday = obj.getString("sunday");
//  String su[sunday.length()];
//  int su1 = 0;
//   for (int i = 0; i < sunday.length(); i++) {
//    if (sunday.substring(i, i+1) == ",") {
//      su[su1] = sunday.substring(i-5,i);
//      su1++;
//    }
//  }
//  su[su1] = sunday.substring(sunday.length()-5, sunday.length());
//}
