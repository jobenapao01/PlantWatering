#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <string>

#define WIFI_SSID "SHEENA" //Wifi SSID
#define WIFI_PASSWORD "HYDROCORTISONE2005" //Wifi password

#define FIREBASE_HOST "pwsystem-6e02c.firebaseio.com"
#define FIREBASE_AUTH "75rzCPuqnO0hiRjwAIcTf6Numsdc1Kn7ghIXftfn"

#define pump 14 //d5
#define soilMoisture 15 //D8
#define waterSensor A0 // Analog pin 0


WiFiClient client;
int soilOutput = 0;
const long utcOffsetInSeconds = 28800;


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.pagasa.dost.gov.ph", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);
  initializeWifi();
  initializePins();
  timeClient.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
}

void loop() {
  timeClient.update();
   blinkLed();
  pumpPower();
  triggerSched();
  sendSoilData();
  waterValue();
  
}

void pumpPower() {
   int pumpStatus = Firebase.getString("Pump/pumpState").toInt();
  if (pumpStatus == 1) {
      pumpOn();
    } else {
      pumpOff(); 
    }
}

void pumpOn(){
  while(digitalRead(soilMoisture) == HIGH){
    digitalWrite(pump, HIGH);
    Firebase.setString("Pump/pumpState", "1");

    if(digitalRead(soilMoisture) == LOW){
         pumpOff();     
         break;
    }
  } 
}

void pumpOff(){
  digitalWrite(pump, LOW);
  Firebase.setString("Pump/pumpState", "0");
}

void waterValue() {
   int waterLevel = analogRead(waterSensor);
   delay(1);
   int waterPercent = (waterLevel/100)*100;
   if(waterPercent == 0){
    Firebase.setString("Water/waterValue", "EMPTY");
   }else{
    Firebase.setString("Water/waterValue","FILLED");
   }
  
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
  pinMode(waterSensor, INPUT);
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
    if(monday.length() > 0){
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
      }else{
      pumpOff();
      }
    }
   }
    
}

void tuesdaySched(){
    FirebaseObject obj = Firebase.get("MainSchedule");
    String currentTime = timeClient.getFormattedTime();
    String curTime = currentTime.substring(0,5);
    String tuesday = obj.getString("tuesday");
    if(tuesday.length() > 0){
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
      }else{
       pumpOff();
      }
    }
   }
   
}

void wednesdaySched(){ 
    FirebaseObject obj = Firebase.get("MainSchedule");
    String currentTime = timeClient.getFormattedTime();
    String curTime = currentTime.substring(0,5);
    String wednesday = obj.getString("wednesday");
    if(wednesday.length() > 0){
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
    
}

void thursdaySched(){
     FirebaseObject obj = Firebase.get("MainSchedule");
    String currentTime = timeClient.getFormattedTime();
    String curTime = currentTime.substring(0,5);
    String thursday = obj.getString("thursday");
    if(thursday.length() > 0){
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
        Serial.println(th[i]);
      }else{
      pumpOff();
       }
     } 
   }
   
}

void fridaySched(){
     FirebaseObject obj = Firebase.get("MainSchedule");
    String currentTime = timeClient.getFormattedTime();
    String curTime = currentTime.substring(0,5);
    String friday = obj.getString("friday");
    if(friday.length() > 0){
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
      }else{
      pumpOff();
       }
     }
   }
    
}

void saturdaySched(){
     FirebaseObject obj = Firebase.get("MainSchedule");
    String currentTime = timeClient.getFormattedTime();
    String curTime = currentTime.substring(0,5);
    String saturday = obj.getString("saturday");
    if(saturday.length()){
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
        }else{
        pumpOff();
        }
      }
    }
    
}

void sundaySched(){
     FirebaseObject obj = Firebase.get("MainSchedule");
    String currentTime = timeClient.getFormattedTime();
    String curTime = currentTime.substring(0,5);
    String sunday = obj.getString("sunday");
    if(sunday.length() > 0){
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
        }else{
        pumpOff();
        }
      }
    }
   
}
