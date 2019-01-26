/*
 * 
 * Author: Lulu's blog (www.lucidar.me). Adaptado por 
 * 
 * Description:
 * 
 */

#include <Wire.h>
#include <TimerOne.h>

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

int16_t GAxes[6] = {0, 0, 0, 0, 0, 0};

void calibrar();


// This function read Nbytes bytes from I2C device at address Address. 
// Put read bytes starting at register Register in the Data array. 
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
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
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}



// Initial time
long int ti;
volatile bool intFlag=false;

// Initializations
void setup()
{
  // Arduino initializations
  Wire.begin();
  Serial.begin(9600);
  
  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,26,0x06);
 
  
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_4_G);

  
  pinMode(13, OUTPUT);
  Timer1.initialize(10000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
  
  
  // Store initial time
  ti=millis();
}





// Counter
long int cpt=0;

void callback()
{ 
  intFlag=true;
  digitalWrite(13, digitalRead(13) ^ 1);
}

// Main loop, read and display data
void loop()
{
  while (!intFlag);
  intFlag=false;

  /*
  // Display time
  Serial.print (millis()-ti,DEC);
  Serial.print ("\t");
  */
  
  // _______________
  // ::: Counter :::
  
  // Display data counter
//  Serial.print (cpt++,DEC);
//  Serial.print ("\t");
  
 
 
  // ____________________________________
  // :::  accelerometer and gyroscope ::: 

  // Read accelerometer and gyroscope
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,A4,Buf);
  
  // Create 16 bits values from 8 bits data

  /*  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];


  // Gyroscope
  int16_t gx = -(Buf[8]<<8 | Buf[9]);
  int16_t gy = -(Buf[10]<<8 | Buf[11]);
  int16_t gz = Buf[12]<<8 | Buf[13];
  */
  
  int16_t gx;
  int16_t gy;
  int16_t gz;


  if (GAxes[0] == 0) {
    
    gx = -(Buf[8]<<8 | Buf[9]);
    gy = -(Buf[10]<<8 | Buf[11]);
    gz = Buf[12]<<8 | Buf[13];

    // TODO: Mostra valor da leitura;
    Serial.print(gx);
    Serial.print(", ");
    Serial.print(gy);
    Serial.print(", ");
    Serial.print(gz);
    Serial.println("** ");

  } else {
    
    GAxes[3] = -(Buf[8]<<8 | Buf[9]);
    //gx = (-(Buf[8]<<8 | Buf[9])) - GAxes[0]; // ou gx -= GAxes[0];
    gx = GAxes[3] - GAxes[0]; 
    GAxes[4] = -(Buf[10]<<8 | Buf[11]);
    //gy = (-(Buf[10]<<8 | Buf[11])) - GAxes[1];  // ou gy -= GAxes[1];
    gy = GAxes[4] - GAxes[1];
    GAxes[5] = Buf[12]<<8 | Buf[13];
    //gz = (Buf[12]<<8 | Buf[13]) - GAxes[2]; // ou gz -= GAxes[2];
    gz = GAxes[5] - GAxes[2];

    // TODO: Mostra valor da leitura ( ** ) e calibrado ( cc )...
    Serial.print(GAxes[3]);
    Serial.print(", ");
    Serial.print(GAxes[4]);
    Serial.print(", ");
    Serial.print(GAxes[5]);
    Serial.print("** \t \t \t");
    Serial.print(gx);
    Serial.print(", ");
    Serial.print(gy);
    Serial.print(", ");
    Serial.print(gz);
    Serial.println(" cc ");
    
  }




  
    // Display values
  /*
  // Accelerometer
  Serial.print (ax,DEC); 
  Serial.print ("Ax\t");
  Serial.print (ay,DEC);
  Serial.print ("Ay\t");
  Serial.print (az,DEC);  
  Serial.print ("Az\t");

  // Gyroscope
  Serial.print (gx,DEC); 
  Serial.print ("Gx\t");
  Serial.print (gy,DEC);
  Serial.print ("Gy\t");
  Serial.print (gz,DEC);  
  Serial.print ("Gz\t");
  */



  // Point 0 of the axes;
  if ( Serial.available() ) {

    switch (Serial.read() ) {
      case '0': 
        digitalWrite(iLED, !digitalRead(iLED));

        Serial.println("Calibrando...");

       GAxes[0] = -(Buf[8]<<8 | Buf[9]);
       GAxes[1] = -(Buf[10]<<8 | Buf[11]);
       GAxes[2] = (Buf[12]<<8 | Buf[13]);

        /*
        // Trata de valores negativos;
        for (int i=0; i < 3; i++) {
          if ( GAxes[i] < 0 ) {
            //int16_t temp;
            //temp = - GAxes[i];
            GAxes[i] = - GAxes[i];
            //Serial.print("TEMP = ");
            //Serial.println(GAxes[i]);
          }
        }
        */
        
        Serial.print("\nGX: ");
        Serial.print(GAxes[0]);
        Serial.print("\t\t GY: ");
        Serial.print(GAxes[1]);
        Serial.print("\t\t GZ: ");
        Serial.print(GAxes[2]);
        Serial.println(" - Valor de calibragem");  
        

        // TODO: LED não altera valor      
        digitalWrite(iLED, !digitalRead(iLED));
        break;  
        
    }
  }

/*
// TODO: Confirmar alteração do valor fora do escopo;
Serial.print("GY = ");
Serial.println(GAxes[1]);
*/
  
  
  
  // End of line
  Serial.println("");
  delay(100);    
}


