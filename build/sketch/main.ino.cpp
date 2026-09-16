#include <Arduino.h>
#line 1 "C:\\Users\\Shree\\Documents\\Arduino\\Test Codes\\LVDT\\main\\main.ino"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define SDA_PIN 42  
#define SCL_PIN 2

int ch = 0;
int samplecount = 8;
const float sensorMinVoltage = 0.00f;
const float sensorMaxVoltage = 5.11f;
const float maxDisplacement = 600.00f;

#line 15 "C:\\Users\\Shree\\Documents\\Arduino\\Test Codes\\LVDT\\main\\main.ino"
void setup();
#line 34 "C:\\Users\\Shree\\Documents\\Arduino\\Test Codes\\LVDT\\main\\main.ino"
void loop();
#line 15 "C:\\Users\\Shree\\Documents\\Arduino\\Test Codes\\LVDT\\main\\main.ino"
void setup()
{
    Serial.begin(115200);
    delay(1000);
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000); 

    if (!ads.begin(0x48))
    {
        Serial.println("ADS1115 not found!");
        while (1);
    }

    ads.setGain(GAIN_TWOTHIRDS);
    ads.setDataRate(RATE_ADS1115_860SPS);

    Serial.println("ADS1115 initialized");
}

void loop()
{
    // 1. Start execution timer
    uint32_t startTime = micros();

    long adcSum = 0;

    for (int i = 0; i < samplecount; i++)
    {
        adcSum += ads.readADC_SingleEnded(ch);
    }

    float adcAverage = (float)adcSum / samplecount;
    float voltage = adcAverage * (sensorMaxVoltage - sensorMinVoltage) / 32767.0f;
    float distance = (voltage - sensorMinVoltage) * maxDisplacement / (sensorMaxVoltage - sensorMinVoltage);

    // 2. Stop execution timer before serial printing
    uint32_t elapsedTimeUS = micros() - startTime;

    Serial.print("Execution Time: ");
    Serial.print(elapsedTimeUS);
    Serial.print(" us | Distance: ");
    Serial.print(distance, 2);
    Serial.println(" mm");
}
