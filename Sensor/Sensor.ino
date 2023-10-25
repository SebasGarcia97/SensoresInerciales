#include "variables.h"
#include "timestamp.h" 
#include "esp32_now_linux.h"
#include "mpu6050.h"

#define TASKER_MAX_TASKS 4
#include "Tasker.h"
Tasker tasker;

bool ejecutarTareaNTP = true; // Variable para controlar si se ejecuta tareaNTP

// Función para la tarea 1
void tareaNTP() {
  //Serial.println("Tarea 1 ejecutada.");
  /*if (ejecutarTareaNTP) {
    initTimestamp();
    valueTimestamp();
    ejecutarTareaNTP = false; 
    delay(1000);
  }*/
  
 tasker.setInterval(tareaNOW_LINUX, 100);
}

// Función para la tarea 2
void tareaNOW_LINUX() {
//  Serial.println("Tarea 2 ejecutada.");
  setupESP();
  initMPU();
  while (true) {
   // Serial.println("Tarea 3 ejecutada.");
    memcpy(slave.peer_addr, dest_mac, sizeof(dest_mac));
    slave.channel = CHANNEL; // Elige un canal
    slave.encrypt = 0; // Sin encriptación
    if (slave.channel == CHANNEL) {
      bool isPaired = manageSlave();
      if (isPaired) {
        sensorMPU();
        sendData();
        delay(10);
      } else {
        Serial.println("¡Falló la conexión con el dispositivo esclavo!");
      }
    }
  }
 delay(3000);
}

void setup() {
  Serial.begin(115200);
  tasker.setInterval(tareaNTP, 1000);
}

void loop() {
  tasker.loop();
}
