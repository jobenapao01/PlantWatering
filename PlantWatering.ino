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



// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.pagasa.dost.gov.ph", utcOffsetInSeconds);

void setup() {
  Serial.begin(115290);
  initializeWifi();
  initializePins();
  sendSoilData();
  timeClient.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  timeClient.update();
  blinkLed();
  sendSoilData();
  pumpPower();
  triggerSched();
}

void timeDisplay() {
  //FirebaseObject obj = Firebase.get("MainSchedule");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
 
}
void pumpPower() {
  //TODO: save logs when pump is on
  int pumpStatus = Firebase.getString("Pump/pumpState").toInt();
  Serial.println(pumpStatus);
  if (pumpStatus == 1) {
    pumpOn();
  } else if (pumpStatus == 0) {
    pumpOff();
  }
}

void pumpOn(){
  if(digitalRead(soilMoisture) == HIGH){
    digitalWrite(pump, HIGH);
    Firebase.setString("Pump/pumpState", "1");
    delay(1000);
  }
  
  else if(digitalRead(soilMoisture) == LOW){
         digitalWrite(pump,LOW);
         Firebase.setString("Pump/pumpState", "0");
         delay(1000);
    }
}

void pumpOff(){
  digitalWrite(pump, LOW);
  Firebase.setString("Pump/pumpState", "0");
}

void waterValue() {
  int waterLevel = digitalRead(waterSensor);

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println("%");
  delay(100);
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
    delay(2000);
  }
  //Firebase.setInt();
  else if (digitalRead(soilMoisture) == LOW) {
    Serial.println("WET");
    Firebase.setString("Soil/soilValue", "WET");
    delay(2000);
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

void triggerSched(){
  int dow = timeClient.getDay();
  switch(dow){
    case 1:
      mondaySched();
      break;
    case 2:
      tuesdaySched();
      break;
    case 3:
      wednesdaySched();
      break;
    case 4:
      thursdaySched();
      break;
    case 5:
      fridaySched();
      break;
    case 6:
      saturdaySched();
      break;
    case 7:
      sundaySched();
      break;
  }
}

void mondaySched(){
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String monday = obj.getString("monday");
  String m[monday.length()];
  int m1 = 0;
   for (int i = 0; i < monday.length(); i++) {
    if (monday.substring(i, i+1) == ",") {
      m[m1] = monday.substring(i-5,i);
      m1++;
    }
  }
  m1++;
  m[m1] = monday.substring(monday.length()-5, monday.length());
  for (int i = 0; i <= m1; i++) {
    if (curTime == m[i]) {
      pumpOn();
    }
  }
}

void tuesdaySched(){
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String tuesday = obj.getString("tuesday");
  String t[tuesday.length()];
  int t1 = 0;
   for (int i = 0; i < tuesday.length(); i++) {
    if (tuesday.substring(i, i+1) == ",") {
      t[t1] = tuesday.substring(i-5,i);
      t1++;
    }
  }
  t1++;
  t[t1] = tuesday.substring(tuesday.length()-5, tuesday.length());
  for (int i = 0; i <= t1; i++) {
    if (curTime == t[i]) {
      pumpOn();
    }
  }
}

void wednesdaySched(){  
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String wednesday = obj.getString("wednesday");
  String w[wednesday.length()];
  int w1 = 0;
   for (int i = 0; i < wednesday.length(); i++) {
    if (wednesday.substring(i, i+1) == ",") {
      w[w1] = wednesday.substring(i-5,i);
      w1++;
    }
  }
  w1++;
  w[w1] = wednesday.substring(wednesday.length()-5, wednesday.length());
  for (int i = 0; i <= w1; i++) {
    if (curTime == w[i]) {
      pumpOn();
    }else{
      pumpOff();
    }
  }
}

void thursdaySched(){
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String thursday = obj.getString("thursday");
  String th[thursday.length()];
  int th1 = 0;
   for (int i = 0; i < thursday.length(); i++) {
    if (thursday.substring(i, i+1) == ",") {
      th[th1] = thursday.substring(i-5,i);
      th1++;
    }
  }
  th1++;
  th[th1] = thursday.substring(thursday.length()-5, thursday.length());
  for (int i = 0; i <= th1; i++) {
    if (curTime == th[i]) {
      pumpOn();
    }
  }
}

void fridaySched(){
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String friday = obj.getString("friday");
  String f[friday.length()];
  int f1 = 0;
   for (int i = 0; i < friday.length(); i++) {
    if (friday.substring(i, i+1) == ",") {
      f[f1] = friday.substring(i-5,i);
      f1++;
    }
  }
  f1++;
  f[f1] = friday.substring(friday.length()-5, friday.length());
   for (int i = 0; i <= f1; i++) {
    if (curTime == f[i]) {
      pumpOn();
    }
      
  }
}

void saturdaySched(){
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String saturday = obj.getString("saturday");
  String s[saturday.length()];
  int s1 = 0;
   for (int i = 0; i < saturday.length(); i++) {
    if (saturday.substring(i, i+1) == ",") {
      s[s1] = saturday.substring(i-5,i);
      s1++;
    }
  }
  s1++;
  s[s1] = saturday.substring(saturday.length()-5, saturday.length());
  for (int i = 0; i <= s1; i++) {
    if (curTime == s[i]) {
      pumpOn();
    }
  }
}

void sundaySched(){
  FirebaseObject obj = Firebase.get("MainSchedule");
  String currentTime = timeClient.getFormattedTime();
  String curTime = currentTime.substring(0,5);
  String sunday = obj.getString("sunday");
  String su[sunday.length()];
  int su1 = 0;
   for (int i = 0; i < sunday.length(); i++) {
    if (sunday.substring(i, i+1) == ",") {
      su[su1] = sunday.substring(i-5,i);
      su1++;
    }
  }
  su1++;
  su[su1] = sunday.substring(sunday.length()-5, sunday.length());
  for (int i = 0; i <= su1; i++) {
    if (curTime == su[i]) {
      pumpOn();
    }
  }
}
