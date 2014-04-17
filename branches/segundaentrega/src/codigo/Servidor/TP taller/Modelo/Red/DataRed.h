#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum TipoPaquete {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

}; 


struct Paquete {

    unsigned int tipoPaquete;

    void serializar(char * data) {
        memcpy(data, this, sizeof(Paquete));
    }

    void deserializar(char * data) {
        memcpy(this, data, sizeof(Paquete));
    }
};