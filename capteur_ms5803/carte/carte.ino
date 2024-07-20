#include <Wire.h>
#define Addr 0x77  // I2C Address for MS5803
#define MUX_ADDR 0x70  // I2C Address for TCA9548A Multiplexer

unsigned long Coff[6], Ti = 0, Offi = 0, sense = 0;
unsigned int data[3];

void selectMuxChannel(uint8_t channel) {
    Wire.beginTransmission(MUX_ADDR);
    Wire.write(1 << channel);
    Wire.endTransmission();
}

void setup() {
    Wire.begin(); // Start I2C Communication as Master
    Serial.begin(9600); // Set serial baud rate to 9600
    Serial.println("14CORE | MS5803 Test Code ");
    delay(4000);
    Serial.println("Reading Value from EEPROM");
    Serial.println(".........................");
    
    // Initialize all sensors
    for (int channel = 0; channel < 8; channel++) {
        selectMuxChannel(channel); // Select multiplexer channel
        for (int i = 0; i < 6; i++) { // Read values from EPROM of each sensor
            Serial.println("Starting i2C Transmission");
            Wire.beginTransmission(Addr);
            Wire.write(0xA2 + (2 * i)); // Select data registers
            Wire.endTransmission();
            Wire.requestFrom(Addr, 2); // Request 2 bytes
            if (Wire.available() == 2) {
                data[0] = Wire.read();
                data[1] = Wire.read();
            }
            Coff[i] = ((data[0] * 256) + data[1]); // Data conversion
        }
    }
    delay(300);
}

void loop() {
    for (int channel = 0; channel < 8; channel++) {
        selectMuxChannel(channel); // Select each sensor sequentially

        // Send reset command
        Wire.beginTransmission(Addr);
        Wire.write(0x1E);
        Wire.endTransmission();
        delay(500);

        // Refresh pressure with the OSR = 256
        Wire.beginTransmission(Addr);
        Wire.write(0x40);
        Wire.endTransmission();
        delay(500);

        // Select and read data register for pressure
        Wire.beginTransmission(Addr);
        Wire.write(0x00);
        Wire.endTransmission();
        Wire.requestFrom(Addr, 3);
        if (Wire.available() == 3) {
            data[0] = Wire.read();
            data[1] = Wire.read();
            data[2] = Wire.read();
        }
        unsigned long ptemp = ((data[0] * 65536.0) + (data[1] * 256.0) + data[2]);

        // Refresh temperature with the OSR = 256
        Wire.beginTransmission(Addr);
        Wire.write(0x50);
        Wire.endTransmission();
        delay(500);

        // Select and read data register for temperature
        Wire.beginTransmission(Addr);
        Wire.write(0x00);
        Wire.endTransmission();
        Wire.requestFrom(Addr, 3);
        if (Wire.available() == 3) {
            data[0] = Wire.read();
            data[1] = Wire.read();
            data[2] = Wire.read();
        }
        unsigned long tempData = ((data[0] * 65536.0) + (data[1] * 256.0) + data[2]);
        unsigned long dT = tempData - (Coff[4] * 256);
        tempData = 2000 + (dT * (Coff[5] / pow(2, 23)));

        // Calculate offset and sensitivity
        unsigned long long Off = Coff[1] * 65536 + (Coff[3] * dT) / 128;
        unsigned long long sens = Coff[0] * 32768 + (Coff[2] * dT) / 256;

        // Second order temperature and pressure compensation
        if (tempData < 2000) {
            Ti = (dT * dT) / (pow(2,31));
            Offi = 5 * ((pow((tempData - 2000), 2))) / 2;
            sense = Offi / 2;
            if (tempData < -1500) {
                Offi += 7 * ((pow((tempData + 1500), 2)));
                sense += 11 * ((pow((tempData + 1500), 2))) / 2;
            }
        } else {
            Ti = 0;
            Offi = 0;
            sense = 0;
        }

        tempData -= Ti; // Adjust temperature based on 2nd order compensation
        Off -= Offi;
        sens -= sense;

        // Convert the final raw data to pressure
        ptemp = (((ptemp * sens) / 2097152) - Off) / 32768.0;
        float pressure = ptemp / 100.0;
        float ctemp = tempData / 100.0;

        // Print results for each sensor
        Serial.print("Sensor ");
        Serial.print(channel);
        Serial.print(" - Temperature in Celsius: ");
        Serial.print(ctemp);
        Serial.println(" C");
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" mbar");
        delay(500); 
    }
}
