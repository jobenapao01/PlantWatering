#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>       //I2C library
#include <RtcDS3231.h>  //RTC library
#include <ArduinoJson.h>
using namespace std;

//RtcDS3231 rtcObject;              //Uncomment for version 1.0.1 of the rtc library
RtcDS3231<TwoWire> rtcObject(Wire); //Uncomment for version 2.0.0 of the rtc library

#define WIFI_SSID "NUEGINAGAWAMUE" //Wifi SSID
#define WIFI_PASSWORD "SUMMERTIMESAGA" //Wifi password

#define FIREBASE_HOST "pwsystem-6e02c.firebaseio.com"
#define FIREBASE_AUTH "75rzCPuqnO0hiRjwAIcTf6Numsdc1Kn7ghIXftfn"

#define pump 14 //d5
#define soilMoisture 13 //D7
#define waterSensor 15 // D8

WiFiClient client;

int soilOutput = 0;
//int soilOutputReal = 0;
//int pump = 5;
//int soilMoisture = 6;
//int waterSensor = 7;
void setup() {
  Serial.begin(115200);
  rtcObject.Begin(); //Starts I2C
  RtcDateTime currentTime = RtcDateTime(19, 02, 22, 3, 24, 0); //define date and time object
  rtcObject.SetDateTime(currentTime); //configure the RTC with object
  initializeWifi();
  initializePins();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  blinkLed();
  //Firebase object = Firebase.pushString("Logs","sad");
  addData();
  getSchedule();
}

void pumpPower() {
  //TODO: save logs when pump is on
  int pumpStatus = Firebase.getString("Pump/pumpState").toInt();
  Serial.println(pumpStatus);
  if (pumpStatus == 1) {
      digitalWrite(pump,HIGH);
      Serial.println("Relay is open");
      if(digitalRead(soilMoisture) != HIGH){
        Firebase.setInt("Pump/pumpStatus",0);
      }
    }else if (pumpStatus == 0) {
      digitalWrite(pump,LOW);
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
 digitalWrite(2,HIGH);
 delay(1000);
 digitalWrite(2,LOW);
 delay(1000);
}

void sendSoilData(){
  if(digitalRead(soilMoisture) == HIGH){
    Serial.println("DRY");
    Firebase.setString("Soil/soilValue", "DRY");
  }
  //Firebase.setInt();
  else if (digitalRead(soilMoisture) == LOW){
    Serial.println("WET");
    Firebase.setString("Soil/soilValue", "WET");
  }
  else
    Serial.println("Error");
 }

void initializeWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);  
  }

  Serial.println();
  Serial.print("Connected");
  Serial.println(WiFi.localIP());
 }

void initializePins(){
  pinMode(pump, OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(soilMoisture, INPUT);
 }

void timeDisplay(){
  RtcDateTime currentTime = rtcObject.GetDateTime();    //get the time from the RTC
 
  char str[20];   //declare a string as an array of chars
 
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
          currentTime.Year(),   //get year method
          currentTime.Month(),  //get month method
          currentTime.Day(),    //get day method
          currentTime.Hour(),   //get hour method
          currentTime.Minute(), //get minute method
          currentTime.Second()  //get second method
         );
 
  Serial.println(str); //print the string to the serial port
}

String fireGet(String s){
  String x = Firebase.getString(s);
  yield();
  delay(100);
  return x;
  delay(100);
}

 void getSchedule(){
  //String sched = Firebase.get("/");
  FirebaseObject obj = Firebase.get("MainSchedule");
  String Monday = obj.getString("Monday");
  String Tuesday = obj.getString("Tuesday");
  String Wednesday = obj.getString("Wednesday");
  String Thursday = obj.getString("Thursday");
  String Friday = obj.getString("Friday");
  String Saturday = obj.getString("Saturday");
  String Sunday = obj.getString("Sunday");

  Serial.println("Monday");
  Serial.println(Monday);
  Serial.println(Tuesday);
  Serial.println(Wednesday);
  Serial.println(Thursday);
  Serial.println(Friday);
  Serial.println(Saturday);
  Serial.println(Sunday);
 }

void addData(){
    //FirebaseObject obj = Firebase.get("logs");
    String key = Firebase.push("Logs","Test");

//    Serial.println(key);

  //string key = Firebase.setInt(1);
 // obj.Child("logs").Child(key).SetValue("test");

 
}
