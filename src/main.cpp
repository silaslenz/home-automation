#include <SPI.h>
#include <Arduino.h>
#include "main.h"
#include <ESP8266WiFi.h>
const char* ssid     = "internetavting";
const char* password = "iatmaskiner";


const char* host = "data.sparkfun.com";
const int httpPort = 80;

WiFiClient client;

#define CS_PIN 15 //Chip select

int adcRead(int channel) { // Read from analog to digital converter
  if ((channel > 7) || (channel < 0)) {
    return -1;
  }

  digitalWrite(CS_PIN, LOW);

  SPI.transfer(1);
  uint8_t r1 = SPI.transfer((channel + 8) << 4);
  uint8_t r2 = SPI.transfer(0);

  digitalWrite(CS_PIN, HIGH);

  return ((r1 & 3) << 8) + r2;
}

void setup() {
    pinMode(A0, INPUT);


    // Set up SPI connection
    pinMode(CS_PIN, OUTPUT);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setFrequency(1000000);
    digitalWrite(CS_PIN, HIGH);

    Serial.begin(230400);
    delay(100);

    // Start by connecting to a WiFi network

    Serial.println();
    Serial.println();

    // Network setup
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(100);

    Serial.print("connecting to ");
    Serial.println(host);
}

void loop() {
    // Making sure wifi connection is working.
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    // Read input
    float light = adcRead(0); // Photocell at pin 0
    float voltage = analogRead(A0);

    voltage /= 1024.0;

    float temperature = (voltage) * 100*3.3;

    // Debug prints
    Serial.print("Brightness is :");
    Serial.println(light);
    Serial.print("Temperature voltage :");
    Serial.println(voltage);
    Serial.print("Temperature is");
    Serial.println(temperature);

    String url = "/input/xRxr3EvbJ2u6JWn5jzMO?private_key=ZaN8EbznYpfv47Me59A2&data=";

    Serial.print("Requesting URL: ");
    url=url+String(temperature);
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);
    Serial.println(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    // Read all the lines of the reply from server and print them to Serial
    Serial.println("Respond:");
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

    client.stop();

    delay(60000);
}
