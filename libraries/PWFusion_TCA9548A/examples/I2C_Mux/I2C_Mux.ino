#include <Wire.h>
#include <PWFusion_TCA9548A.h>

TCA9548A i2cMux;

void setup() {
  // Initialize I2C and Serial
  Wire.begin();
  Serial.begin(9600);

  // Initialize I2C multiplexor
  i2cMux.begin();
}

void loop() {
  i2cMux.setChannel(CHAN0); 
  Serial.println("Channel 0 selected"); 
  delay(500);

  i2cMux.setChannel(CHAN0 | CHAN4); 
  Serial.println("Channels 0 and 4 selected"); 
  delay(500);

  i2cMux.setChannel(CHAN_NONE); 
  Serial.println("No channels selected"); 
  delay(500);
}
