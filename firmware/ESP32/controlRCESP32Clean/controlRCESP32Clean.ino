#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>

const char* ssid = ""; //SSID

WiFiUDP udp;
const uint16_t UDP_PORT = 50000;

unsigned long receivedTime = 0;
const unsigned long timeout = 500; // 0.5 sec timeout

static const int servoPin = 36; 
Servo servo1;

int throttle = 0;
int direction = 0;
int steering = 90; 

IPAddress local_IP(0,0,0,0);// Set Static IP address

IPAddress gateway(0,0,0,0);// Set Gateway IP address
IPAddress subnet(0,0,0,0); //must change all 0s to actual ip

void setup() {
  Serial.begin(115200);
  initWiFi();
  udp.begin(UDP_PORT);

  pinMode(14, OUTPUT);
  ledcAttach(11, 20000, 8);  // pin, frequency, 8 bits 0 - 255
  ledcAttach(12, 20000, 8);  
  ledcAttach(10, 20000, 8);
  ledcAttach(13, 20000, 8); 

  servo1.attach(servoPin,500,2500);

  ledcWrite(10, 0);
  ledcWrite(11, 0);
  ledcWrite(12, 0);
  ledcWrite(13, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(14, HIGH);
  int packetSize = udp.parsePacket();

  if (packetSize == 3) {
    uint8_t data[3];
    int len = udp.read(data, 3);

    if (len == 3) {
      steering = data[0];
      direction = data[1]; // 0 = forward, 1 = backward
      throttle = data[2];
      receivedTime = millis();
    }
  }

  if (millis() - receivedTime > timeout){
    steering = 90;
    throttle = 0;
  }

  servo1.write(steering);    
  if (direction == 0) {
    ledcWrite(10, 0);
    ledcWrite(13, 0);
    ledcWrite(11, throttle);
    ledcWrite(12, throttle);
  }
  else if (direction == 1) {
    ledcWrite(11, 0);
    ledcWrite(12, 0);
    ledcWrite(10, throttle);
    ledcWrite(13, throttle);
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  while (!WiFi.config(local_IP, gateway, subnet) ){ //set up ip, if failed retry
    delay(1000);
  }
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) { //check connection, if not connected retry
    delay(1000);
  }
}