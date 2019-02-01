/*
 * 
 * Author: Lulu's blog (www.lucidar.me). Adaptado por 
 * 
 * Description:
 * 
 */


#include <Wire.h>


#define    MPU9250_ADDRESS            0x68

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18


const int iLED = 13;



  /* :: Funcoes basicas :: */

// This function read Nbytes bytes from I2C device at address Address. 
// Put read bytes starting at register Register in the Data array. 
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
  
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}


// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data) {
  
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}





// ____________________________________
// :::  accelerometer and gyroscope ::: 


// Read accelerometer and gyroscope
void readGA() {

  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,A4,Buf);
  
  // Create 16 bits values from 8 bits data

  /*  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];
  */

  // Gyroscope
  int16_t gx = -(Buf[8]<<8 | Buf[9]);
  int16_t gy = -(Buf[10]<<8 | Buf[11]);
  int16_t gz = Buf[12]<<8 | Buf[13];

  
  
    // Display values
  /*
  // Accelerometer
  Serial.print (ax,DEC); 
  Serial.print ("Ax\t");
  Serial.print (ay,DEC);
  Serial.print ("Ay\t");
  Serial.print (az,DEC);  
  Serial.print ("Az\t");
  */
  
  // Gyroscope
  Serial.print (gx,DEC); 
  Serial.print ("Gx\t");
  Serial.print (gy,DEC);
  Serial.print ("Gy\t");
  Serial.print (gz,DEC);  
  Serial.print ("Gz\t");
  
  // End of line
  Serial.println("");
  delay(100);
  
}

