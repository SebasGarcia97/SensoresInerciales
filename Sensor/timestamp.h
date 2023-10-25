#ifndef timestamp_h
#define timestamp_h

#include <WiFi.h> 
#include "time.h"

const char* ssid       = "Consultorios_Garcia";
const char* password   = "acapulco";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;       // Offset horario en segundos (0 para GMT)
const int daylightOffset_sec = 3600; // Offset horario de horario de verano en segundos


void initWiFi() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red Wi-Fi...");
  }
  Serial.println("Conexión exitosa a la red Wi-Fi");
  bandera = 1;
}

void initTimestamp(){
  initWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Configuración de hora exitosa");
  delay(1000);
}


unsigned long getEpochTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora local");
    return 0;
  }
  time_t epochTime = mktime(&timeinfo);
  return static_cast<unsigned long>(epochTime);
}


void valueTimestamp() {
  unsigned long epochTime= getEpochTime();
  if (epochTime > 0) {
    gyroData.timeStamp = epochTime;
    valor = 1;
  }
}
#endif
