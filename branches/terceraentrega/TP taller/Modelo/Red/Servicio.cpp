#include "Servicio.h"

int Servicio::enviarMensaje(SOCKET curSocket, char * mensaje, int mensajeTamano)
{
	//send es un metodo de winsock
    return send(curSocket, mensaje, mensajeTamano, 0);
}


int Servicio::recibirMensaje(SOCKET curSocket, char * buffer, int bufferTamano)
{
	//recv es un metodo de winsock
    return recv(curSocket, buffer, bufferTamano, 0);
}
