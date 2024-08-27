#include <WiFiNINA.h>
#include "secret.h"
#define LIGHT_SENSOR 2
//please enter your sensitive data in the Secret tab
char SSID[] = SECRET_SSID;
char PASS[] = SECRET_PASSWORD;

WiFiClient client;
//https://maker.ifttt.com/trigger/enter/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ

char   HOST_NAME[] = "maker.ifttt.com";

// Data variables for IFTTT
String state = "Off";
int value = 0;
String light = "Off";

bool connectToWiFi(char SSID[], char PASS[]) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(SSID, PASS);
      Serial.print(".");
      delay(5000);
      if (i > 6) {
        Serial.print("Wifi Connection has Failed!");
        return false;
        break;
      }
      i++;
    }
    Serial.println("\nConnected.");
    return true;
  }
  return true; 
}
int readLightSensor() {
  value = digitalRead(LIGHT_SENSOR);
  return value;
}


void sendWebhook(String state) {
  if (!client.connect(HOST_NAME, 80)) {
    Serial.println("Connection failed!");
    return;
  }


    Serial.println("Connected to server");
    client.println("GET /trigger/Light_Changed/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ HTTP/1.1");
    client.println("Host: maker.ifttt.com"  );
    client.println("Connection: close");
    client.println();

  
  client.stop();
  Serial.println("Sent notification to IFTTT");
}

void setup() {

  Serial.begin(115200);
  connectToWiFi(SSID,PASS);
  while (!Serial);


  pinMode(LIGHT_SENSOR,INPUT);
}



void loop() {

  int current_light = readLightSensor();

  if (current_light > 0) {
    state = "On";
  } else {
    state = "Off";
  }

  // Send notification only if state changes or 'c' is pressed
  if (state != light || Serial.read() == 'c') {
    sendWebhook(state);
    light = state;
  }
  delay(1000); // Adjust delay based on desired update frequency

}