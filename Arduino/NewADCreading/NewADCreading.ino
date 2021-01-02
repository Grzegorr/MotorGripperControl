#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads1015;

void setup()
{
  ads1015.begin();
  Serial.begin(115200);
  Serial.println("Hello!");
  
  //Serial.println("Getting single-ended readings from 1083 ADC shield.");
  //Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
}

void loop()
{
  //Serial.println("Loop");
  int16_t adc0;
  //Serial.println("LoopB");
  adc0 = ads1015.readADC_SingleEnded(0);
  //Serial.println("LoopC");
  int sensorSum = 0;
  //Serial.println("LoopA");
  int x = 0;
  for(x = 0; x <128; x++){
    adc0 = ads1015.readADC_SingleEnded(0);
    sensorSum = sensorSum + adc0;
  }
  Serial.println(sensorSum/128.);

  delay(1);
}
