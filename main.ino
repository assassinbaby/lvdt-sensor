#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;
// esp32 s3
#define SDA_PIN 42  
#define SCL_PIN 2



int ch = 0;
int samplecount = 8;
const float sensorMinVoltage = 0.00f;
const float sensorMaxVoltage = 5.11f;
const float maxDisplacement = 600.00f;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000); // set I2C clock to 

    if (!ads.begin(0x48))
    {
        Serial.println("ADS1115 not found!");
        while (1);
    }

    // ADS1115 PGA = +/-4.096 V
    ads.setGain(GAIN_TWOTHIRDS);

    // 128 samples per second
    ads.setDataRate(RATE_ADS1115_860SPS);

    Serial.println("ADS1115 initialized");
}

void loop()
{
    long adcSum = 0;

    // Take samples
    for (int i = 0; i < samplecount; i++)
    {
        adcSum += ads.readADC_SingleEnded(ch);
    }

    // Average ADC value
    float adcAverage = (float)adcSum / samplecount;
    float voltage = adcAverage * (sensorMaxVoltage - sensorMinVoltage) / 32767.0f;
    float distance = (voltage - sensorMinVoltage) * maxDisplacement / (sensorMaxVoltage - sensorMinVoltage);

    // // Convert ADC count to voltage
    // float voltage = ads.computeVolts((int16_t)adcAverage);

    // Convert voltage to displacement
    // float displacement =
    //     (voltage - sensorMinVoltage) *
    //     maxDisplacement /
    //     (sensorMaxVoltage - sensorMinVoltage);
        

    // // Limit displacement
    // if (displacement < 0)
    //     displacement = 0;

    // if (displacement > maxDisplacement)
    //     displacement = maxDisplacement;

    Serial.print("ADC Avg = ");
    Serial.print(adcAverage, 2);

    Serial.print(" : Voltage = ");
    Serial.print(voltage, 2);
    Serial.print(" V");

    Serial.print(" : Displacement = ");
    Serial.print(distance, 2);
    Serial.println(" mm");
}