#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

#define Addr 0x77 //i2C Address 
unsigned long Coff[6], Ti = 0, Offi = 0, sense = 0;
unsigned int data[3];
void setup()
{
 
  Wire.begin(); //Set i2C Communication as Master
  Serial.begin(9600); //Set serial boud rate 9600
  Serial.print("14CORE | MS5803 Test Code ");
  delay(4000);
  Serial.print("Reading Value from EEPROM");
  Serial.print(".........................");
  for(int i = 0; i < 6; i++) //Read values from EPROM 
  {
    Serial.print("Starting i2C Transmission");
    Wire.beginTransmission(Addr); //Start i2C Transmission
    Wire.write(0xA2 + (2 * i)); //Select data registers
    Wire.endTransmission(); //Stop Transmission
    Wire.requestFrom(Addr, 2); //Request 2 bytes
    if(Wire.available() == 2) //Read 2 bytes 
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }  
      
    Coff[i] = ((data[0] * 256) + data[1]); // Data convertion
  }
  delay(300);
}
 
void loop() 
{
  
  Wire.beginTransmission(Addr); // Start I2C Transmission
  Wire.write(0x1E);  // Send reset command 
  Wire.endTransmission();   // Stop I2C Transmission
  delay(500);
  Wire.beginTransmission(Addr);   // Start I2C Transmission
  Wire.write(0x40);   // Refresh pressure with the OSR = 256 
  Wire.endTransmission();   // Stop I2C Transmission
  delay(500);
  Wire.beginTransmission(Addr);   // Start I2C Transmission
  Wire.write(0x00);   // Select data register 
  Wire.endTransmission();   //Stop I2C Transmission
  
 
  Wire.requestFrom(Addr, 3);   //Set Request 3 bytes of data
  if(Wire.available() == 3)
  {
     data[0] = Wire.read();
     data[1] = Wire.read();
     data[2] = Wire.read();
  }   
  
 
  unsigned long ptemp = ((data[0] * 65536.0) + (data[1] * 256.0) + data[2]);   //Set to Convert the data 
  Wire.beginTransmission(Addr);   // Start I2C Transmission
  Wire.write(0x50);   // Refresh temperature with the OSR = 256 
  Wire.endTransmission();   // Stop I2C Transmission
  delay(500);
  Wire.beginTransmission(Addr);   // Start I2C Transmission
  Wire.write(0x00);   // Select data register
  Wire.endTransmission();   // Stop I2C Transmission
  Wire.requestFrom(Addr, 3);   // Request 3 bytes of data
  if(Wire.available() == 3)   // Read 3 bytes of data
  {
     data[0] = Wire.read();
     data[1] = Wire.read();
     data[2] = Wire.read();
  }
  
  unsigned long tempData = ((data[0] * 65536.0) + (data[1] * 256.0) + data[2]);   // Convert the data
  unsigned long dT = tempData- ((Coff[4] * 256));  // Difference between actual and reference temperature
  tempData = 2000 + (dT * (Coff[5] / pow(2, 23)));
  unsigned long long Off = Coff[1] * 65536 + (Coff[3] * dT) / 128;   // Offset and Sensitivity calculation
  unsigned long long sens = Coff[0] * 32768 + (Coff[2] * dT) / 256;
  if(tempData< 2000) //Second order temperature and pressure compensation
  {
    Ti = (dT * dT) / (pow(2,31));
    Offi = 5 * ((pow((tempData- 2000), 2))) / 2;
    sense = Offi / 2; 
    if(tempData< - 1500)
    {
       Offi = Offi + 7 * ((pow((tempData+ 1500), 2)));      
       sense = sense + 11 * ((pow((tempData+ 1500), 2))) / 2;
    }
  }
  else if(tempData>= 2000)
  {
     Ti = 0;
     Offi = 0;
     sense = 0;
  }
  
  tempData-= Ti;   // Adjust temp, Off, sens based on 2nd order compensation   
  Off -= Offi;
  sens -= sense;
 
 
  ptemp = (((ptemp * sens) / 2097152) - Off);   // Convert the final raw data
  ptemp /= 32768.0;
  float pressure = ptemp / 100.0;
  float ctemp = tempData/ 100.0;
  float fTemp = ctemp * 1.8 + 32.0;
  
/*************************** OUTPUT *****************************/
 
  Serial.print("Temperature in Celsius : ");
  Serial.print(ctemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit : ");
  Serial.print(fTemp);
  Serial.println(" F");
  Serial.print("Pressure > ");
  Serial.print(pressure);
  Serial.println(" mbar"); 
  delay(500); 
}