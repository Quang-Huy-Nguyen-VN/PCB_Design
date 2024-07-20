#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

// Initialize the MS5803 sensor
MS5803 sensor(ADDRESS_HIGH); // Assuming all sensors are set to the same address

// Function to select the multiplexer channel
void selectMuxChannel(byte channel) {
  Wire.beginTransmission(0x70); // Address of the TCA9548A
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize all sensors (assuming they all can be initialized in the same way)
  for (int i = 0; i < 8; i++) {
    selectMuxChannel(i);
    sensor.init();
  }
}

void loop() {
  for (int i = 0; i < 8; i++) {
    selectMuxChannel(i); // Select the current channel
    sensor.readSensor(); // Read the sensor values

    // Print the pressure and temperature readings
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": Pressure = ");
    Serial.print(sensor.pressure(), 4); // Print pressure with 4 decimal places
    Serial.print(" mbar, Temperature = ");
    Serial.print(sensor.temperature(), 2); // Print temperature with 2 decimal places
    Serial.println(" C");
  }

  delay(1000); // Delay between readings
}
