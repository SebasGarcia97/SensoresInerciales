/*
Etienne Arlaud
*/

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

#include <thread>

#include "ESPNOW_manager.h"
#include "ESPNOW_types.h"

using namespace std;

//static uint8_t my_mac[6] = {0xF8, 0x1A, 0x67, 0xb7, 0xEB, 0x0B};
//static uint8_t dest_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//static uint8_t ESP_mac[6] = {0xB4,0xE6,0x2D,0xB5,0x9F,0x85};

static uint8_t my_mac[6] = {0x74, 0xDA, 0x38, 0x42, 0x48, 0x2A};
static uint8_t dest_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static uint8_t ESP_mac[6] = {0x78,0xE3, 0x6D,0x19,0xE5,0x28}; //nodo4
//static uint8_t ESP_mac[6] = {0x7C,0x9E, 0xBD,0x06,0x8B,0x5C}; //nodotest


ESPNOW_manager *handler;

uint8_t payload[127];


void callback(uint8_t src_mac[6], uint8_t *data, int len) {
	handler->mypacket.wlan.actionframe.content.length = 127 + 5;
	memcpy(handler->mypacket.wlan.actionframe.content.payload, data, 6);
	//handler->set_dst_mac(dest_mac);
	handler->send();
	printf(" MAC: ");
	FILE *archivo = fopen("datos.csv","a");
	for (int i = 0; i < 6; i++)
    {
        printf("%02x-", src_mac[i]);
    }
	printf(" DATA: ");
	for (int i = 0; i < len; i+=4)
    {
		int32_t data32 = data[i+3] <<24 |data[i+2] << 16 |data[i+1] << 8 | data[i];
        printf("%d ", data32);
		fprintf(archivo, "%d,", data32);
	}
	printf("\n");
	fprintf(archivo, "\n");
	fclose(archivo);
}

int main(int argc, char **argv) {

	assert(argc > 1);

	nice(-20);

	handler = new ESPNOW_manager(argv[1], DATARATE_54Mbps, CHANNEL_freq_9, dest_mac, my_mac, false);

	handler->set_filter(ESP_mac, dest_mac);

	handler->set_recv_callback(&callback);

	handler->start();

	while(1) {
		std::this_thread::yield();
	}
	

	handler->end();
	
}
