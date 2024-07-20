#include <Wire.h>
#include <PWFusion_TCA9548A.h>
#include <SparkFun_MS5803_I2C.h>

#define MS5803_ADDRESS_HIGH 0x76
#define MS5803_ADDRESS_LOW  0x77

TCA9548A i2cMux;
MS5803 sensorHigh(MS5803_ADDRESS_HIGH);
MS5803 sensorLow(MS5803_ADDRESS_LOW);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  Serial.println("Initializing I2C multiplexer and sensors...");
  i2cMux.begin();
  Serial.println("Initialization complete.");
}

void scanForMS5803(uint8_t channel) {
  i2cMux.setChannel(channel);
  delay(1000);

  Serial.print("Scanning channel ");
  Serial.println(channel);

  Wire.beginTransmission(MS5803_ADDRESS_HIGH);
  if (Wire.endTransmission() == 0) {
    Serial.print("MS5803 found at address 0x76 on channel ");
    Serial.println(channel);
    sensorHigh.reset();
    sensorHigh.begin();
  } else {
    Serial.print("MS5803 not found at address 0x76 on channel ");
    Serial.println(channel);
  }

  Wire.beginTransmission(MS5803_ADDRESS_LOW);
  if (Wire.endTransmission() == 0) {
    Serial.print("MS5803 found at address 0x77 on channel ");
    Serial.println(channel);
    sensorLow.reset();
    sensorLow.begin();
  } else {
    Serial.print("MS5803 not found at address 0x77 on channel ");
    Serial.println(channel);
  }

  i2cMux.setChannel(CHAN_NONE);
}

void loop() {
  for (uint8_t channel = 0; channel < 8; channel++) {
    scanForMS5803(channel);
    delay(1000); 

    i2cMux.setChannel(channel);
    delay(1000);

    Wire.beginTransmission(MS5803_ADDRESS_HIGH);
    if (Wire.endTransmission() == 0) {
      float temperature = sensorHigh.getTemperature(CELSIUS, ADC_512);
      double pressure = sensorHigh.getPressure(ADC_4096);
      Serial.print("Sensor at 0x76 on channel ");
      Serial.print(channel);
      Serial.print(": Temperature: ");
      Serial.print(temperature);
      Serial.print(" C, Pressure: ");
      Serial.print(pressure);
      Serial.println(" mbar");
    }

    Wire.beginTransmission(MS5803_ADDRESS_LOW);
    if (Wire.endTransmission() == 0) {
      float temperature = sensorLow.getTemperature(CELSIUS, ADC_512);
      double pressure = sensorLow.getPressure(ADC_4096);
      Serial.print("Sensor at 0x77 on channel ");
      Serial.print(channel);
      Serial.print(": Temperature: ");
      Serial.print(temperature);
      Serial.print(" C, Pressure: ");
      Serial.print(pressure);
      Serial.println(" mbar");
    }
  }

  delay(5000); 
}



