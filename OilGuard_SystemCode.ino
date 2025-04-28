// Including libraries needed in project
#include <Stepper.h>
#include <Arduino.h>
#include <WiFi.h>               
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <OneWire.h>
#include <DallasTemperature.h>  
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Defining WiFi Info
#define WIFI_SSID "wifi_ssid"
#define WIFI_PASSWORD "wifi_pass"

// Needed info to connect to the Firebase RTDB
#define API_KEY ""
#define DATABASE_URL "" 

const char serverName[] PROGMEM = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000); // Update every 60 seconds

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Defining Pins connections
#define TRIG_PIN  14
#define ECHO_PIN  12 
#define OneWireBus 32
#define HallSensor1 34
#define HallSensor2 35
// Creating Variables and Constants included in project
bool signupOK = false;
const int stepsPerRevolution = 2048;
float Temperature = 0.0;
float Viscosity = 0.0;
float Oil_Level = 0.0;
uint8_t State = 0;
float Viscosity_Calculation_Constant = 0.890 / 11712164.00; 
int prevMillis = 0;
int Viscosity_delay = 1000*60*60; // 1 hour 

// Creating a Stepper Object through Stepper.h library to allow easy control of Motor
Stepper myStepper(stepsPerRevolution, 2, 4, 0, 16);

OneWire oneWire(OneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Initialize the serial communication:
  Serial.begin(115200);
  myStepper.setSpeed(5);

  SetUp_WiFi_Connection();
  
  timeClient.begin();
  timeClient.update();
  Restart_Google_Sheet();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(HallSensor1, INPUT);
  pinMode(HallSensor2, INPUT);
  // Ensure the Trigger pin is LOW initially
  digitalWrite(TRIG_PIN, LOW);
  delay(50);
}

void loop() {
  
  sensors.requestTemperatures(); 
  
  Temperature = sensors.getTempCByIndex(0);
  Oil_Level = Oil_Level_calculator();
  
  // debugging line 
  if(Serial.available() > 0){
    if(Serial.read() == '1'){
      Viscosity = Viscosity_Calculator();
      Serial.println(Viscosity);
      }
  }

  int currentMillis = millis();
  if((currentMillis - prevMillis) > Viscosity_delay && 22 < Temperature < 26){
    Viscosity = Viscosity_Calculator();
    prevMillis = currentMillis;
    }

  if(Firebase.RTDB.getString(&fbdo, "Sensors/Control")){
    State = atoi(fbdo.to<const char *>());
    if (State == 1){
      Viscosity = Viscosity_Calculator();
      prevMillis = currentMillis;
      } 
   }

  if (Firebase.ready() && signupOK){  
    Firebase.RTDB.setFloat(&fbdo, "Sensors/Temperature", Temperature);
    Firebase.RTDB.setFloat(&fbdo, "Sensors/Oil_Level", Oil_Level);
    Firebase.RTDB.setFloat(&fbdo, "Sensors/Viscosity", Viscosity);
  }
  Send_Data_to_Google_sheet(Temperature, Oil_Level, Viscosity);

  //delay(1000);
}

void SetUp_WiFi_Connection(){

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void Send_Data_to_Google_sheet(float Temperature,float Oil_Level, float Viscosity ){
   
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  // Fetch time from NTP client
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();

  // Prepare JSON payload with NTP time, sensor data, and button state
  String jsonData = "{\"method\":\"append\", \"Temperature\":" + String(Temperature) +
                    ", \"Oil_Level\":" + String(Oil_Level) +
                    ", \"timestamp\":\"" + formattedTime + "\"" +
                    ", \"Viscosity\":" + String(Viscosity) + "}";

  // Send HTTP POST request
  int httpResponseCode = http.POST(jsonData);

  http.end(); // Close connection

}

void Restart_Google_Sheet(){
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  // Fetch time from NTP client
  timeClient.update();
  // Prepare JSON payload with NTP time, sensor data, and button state
  String jsonData = "{\"method\":\"replace\"}";
  http.POST(jsonData);
  http.end(); // Close connection
}

float Viscosity_Calculator(){
  float Duration = micros();
  
  while(digitalRead(HallSensor1) == 1){myStepper.step(-100);}
  
  Duration = millis() - Duration;

  while(digitalRead(HallSensor2) == 1){myStepper.step(100);}
  
  Viscosity = Duration * Viscosity_Calculation_Constant;

  if (Firebase.ready() && signupOK){Firebase.RTDB.setString(&fbdo, "Sensors/Control", "0");}

  return Viscosity;
}

float Oil_Level_calculator(){
  
  long duration;
  float height;


  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure the duration of the pulse received at Echo pin:
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance (distance = duration / 2 * speed of sound)
  // The speed of sound is approximately 343 meters per second (or 0.0343 cm/microsecond)
  height = duration * 0.0343 / 2; 
  if (height > 11 || height <0){
    return 0;
  }
  
  else{
    Oil_Level = (11 - height);
    return Oil_Level;
  }
}
