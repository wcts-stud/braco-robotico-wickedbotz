/*
 * 
 * Author: Igor Fachini com alterações;
 * 
 * Description: Movimentar servo motores utilizando bluetooth via app 
 *  (disponível em: https://github.com/WickedBotz/BrinquedoIOT_Free-A/blob/master/Apk/CAB-Controle%20Arduino%20Bluetooth_Free.apk)
 *  do protótipo do braço robótico feito de madeira;
 * 
 * Objetivo: Manipular o protótipo utilizando o módulo MPU 9250;
 * 
 */
 
#include <SoftwareSerial.h>
#include <Servo.h>
#include "mpu9250.h"

SoftwareSerial serial1(11, 10); // RX, TX

#define SERVO_A 4
#define SERVO_B 5
#define SERVO_C 6
#define SERVO_D 7

Servo servo_a, servo_b, servo_c, servo_d;

String data = "0";
int a = 15, b = 90, c = 90, d = 90, time_servo = 15;

// MAPEAMENTO DIRECOES BRACO
//A FRENTE tras - cervo esquerda  < pra tras > pra frente
//b garra < abre > fecha
//c cima baixo - cervo direita < pra frente > pra tras
//d direita esquerda - cervo baixo < pra esquerda > pra direita

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial1.begin(9600);
  Wire.begin();

  Serial.println("Starting arm...");


  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,26,0x06);
 
  
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_4_G);  
  

  servo_a.attach(SERVO_A);
  servo_b.attach(SERVO_B);
  servo_c.attach(SERVO_C);
  servo_d.attach(SERVO_D);

  servo_a.write(a);
  servo_b.write(b);
  servo_c.write(c);
  servo_d.write(d);
}

void loop()
{
  // put your main code here, to run repeatedly:
  app_arm_control();

  readGA();
}

// Para controle manual, recebe a letra do servo e a posicao
// Ex: a20
void manual_arm_control()
{
  if (Serial.available())
  {
    data = Serial.readString();
    char option = data.charAt(0);
    data.remove(0, 1);
    int var = data.toInt();

    switch (option)
    {
    case 'a':
      var = constrain(var, 10, 120);
      Serial.print("SERVO_A = ");
      Serial.println(var);
      while (a != var)
      {
        if (a > var)
        {
          a--;
        }
        if (a < var)
        {
          a++;
        }
        servo_a.write(a);
        delay(time_servo);
      }
      break;
    case 'b':
      var = constrain(var, 70, 110);
      Serial.print("SERVO_B = ");
      Serial.println(var);
      while (b != var)
      {
        if (b > var)
        {
          b--;
        }
        if (b < var)
        {
          b++;
        }
        servo_b.write(b);
        delay(time_servo);
      }
      break;
    case 'c':
      var = constrain(var, 0, 140);
      Serial.print("SERVO_C = ");
      Serial.println(var);
      while (c != var)
      {
        if (c > var)
        {
          c--;
        }
        if (c < var)
        {
          c++;
        }
        servo_c.write(c);
        delay(time_servo);
      }
      break;
    case 'd':
      var = constrain(var, 10, 170);
      Serial.print("SERVO_D = ");
      Serial.println(var);
      while (d != var)
      {
        if (d > var)
        {
          d--;
        }
        if (d < var)
        {
          d++;
        }
        servo_d.write(d);
        delay(time_servo);
      }
      break;
    }
  }
}

//Para ser controlodo pelo app
void app_arm_control()
{
  if (serial1.available() > 0)
  {
    data = serial1.readStringUntil(';');
  }
  char direction = data.charAt(0);
  switch (direction)
  {
  case '8':
    if (a < 120)
    {
      a++;
      servo_a.write(a);
      delay(time_servo);
    }

    break;
  case '4':
    if (d < 170)
    {
      d++;
      servo_d.write(d);
      delay(time_servo);
    }

    break;
  case '6':
    if (d > 10)
    {
      d--;
      servo_d.write(d);
      delay(time_servo);
    }

    break;
  case '2':
    if (a > 10)
    {
      a--;
      servo_a.write(a);
      delay(time_servo);
    }

    break;
  case 'x':
    if (c < 140)
    {
      c++;
      servo_c.write(c);
      delay(time_servo);
    }

    break;
  case 'a':
    if (c > 0)
    {
      c--;
      servo_c.write(c);
      delay(time_servo);
    }

    break;

  case 'y':
    if (b > 70)
    {
      b--;
      servo_b.write(b);
      delay(time_servo);
    }

    break;
  case 'z':
    if (b < 110)
    {
      b++;
      servo_b.write(b);
      delay(time_servo);
    }

    break;
  }
}
