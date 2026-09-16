#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// ESP32-S3
#define SDA_PIN 42
#define SCL_PIN 2

int sensorCh = 0;
int referenceCh = 1;

int samplecount = 8;

const float maxDisplacement = 600.00;

float last_distance = 0.0;

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

    // ADS1115 PGA = +/-6.144 V
    // Suitable for sensor/reference voltage around 5 V
    ads.setGain(GAIN_TWOTHIRDS);

    // 860 samples per second
    ads.setDataRate(RATE_ADS1115_860SPS);

    Serial.println("ADS1115 initialized");
}

void loop()
{
    long sensorSum = 0;
    long referenceSum = 0;

    // Take samples from both channels
    for (int i = 0; i < samplecount; i++)
    {
        sensorSum += ads.readADC_SingleEnded(sensorCh);
        referenceSum += ads.readADC_SingleEnded(referenceCh);
    }

    // Average ADC values
    float sensorADC = (float)sensorSum / samplecount;
    float referenceADC = (float)referenceSum / samplecount;

    // Convert ADC values to voltage
    float sensorVoltage = ads.computeVolts((int16_t)sensorADC);
    float referenceVoltage = ads.computeVolts((int16_t)referenceADC);

    // Calculate displacement using actual reference voltage
    float distance = (sensorVoltage / referenceVoltage) * maxDisplacement;

    if(last_distance == 0){
        last_distance = distance;
    }
    distance = distance * 10;
    distance = distance + last_distance;
    distance = distance / 11;
    last_distance = distance;

    Serial.print("Sensor ADC = ");
    Serial.print(sensorADC, 2);

    Serial.print(" : Sensor Voltage = ");
    Serial.print(sensorVoltage, 2);
    Serial.print(" V");

    Serial.print(" : Reference ADC = ");
    Serial.print(referenceADC, 2);

    Serial.print(" : Reference Voltage = ");
    Serial.print(referenceVoltage, 2);
    Serial.print(" V");

    Serial.print(" : Displacement = ");
    Serial.print(distance, 2);
    Serial.println(" mm");
     Serial.print(">var1:");
    Serial.println(distance, 1);


}