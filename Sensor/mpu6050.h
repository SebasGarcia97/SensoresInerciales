#ifndef mpu6050_h
#define mpu6050_h


#include <Wire.h>
// Dirección del MPU6050
const int MPU6050_ADDRESS = 0x68;

// Registros del MPU6050
const int PWR_MGMT_1 = 0x6B;
const int GYRO_ZOUT_H = 0x47;

// Factores de conversión para el giroscopio
const float GYRO_SCALE_FACTOR = 131.0;

// Variables de compensación
int gyro_offset_z = 0;

// Variables para el cálculo de la orientación
float gyro_z_angle = 0.0;

// Tiempo de muestreo (en segundos)
const float dt = 0.1;  // Ajusta esto según tu frecuencia de muestreo


void initMPU(){
  Wire.begin();
  // Inicializar el MPU6050
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.println("MPU6050 Configurado");
  
}


// Función para leer valores de 16 bits a partir de dos registros consecutivos
int readWord(int reg) {
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 2, true);
  int high = Wire.read();
  int low = Wire.read();
  int value = (high << 8) | low;
  if (value >= 0x8000) {
    return -((65535 - value) + 1);
  } else {
    return value;
  }
}


void sensorMPU(){
  float gyro_z = (readWord(GYRO_ZOUT_H) - gyro_offset_z) / GYRO_SCALE_FACTOR;
  gyro_z_angle += gyro_z * dt;
  // Enviar los datos a JETSON NANO
  gyroData.gyroZ = gyro_z_angle;
  delay(100);
}

#endif
