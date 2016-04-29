#include <SPI.h>


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
    // Set up SPI connection
    pinMode(CS_PIN, OUTPUT);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setFrequency(1000000);
    digitalWrite(CS_PIN, HIGH);

    Serial.begin(9600);
}

void loop() {
    // Read input
    float light = adcRead(0); // Photocell at pin 0
    float voltage = adcRead(1)*3300/1024.0+144; // Voltage from mcp3008 is about 144mV to low according to multimeter
    float temperature = (voltage / 10.0) - 50.0;

    // Debug prints
    Serial.print("Brightness is :");
    Serial.println(light);
    Serial.print("Temperature voltage :");
    Serial.println(voltage);
    Serial.print("Temperature is");
    Serial.println(temperature);

    delay(5000);
}
