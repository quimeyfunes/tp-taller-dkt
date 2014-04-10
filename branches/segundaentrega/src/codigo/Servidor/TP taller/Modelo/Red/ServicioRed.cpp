#include "ServicioRed.h"

int ServicioRed::enviarMensaje(SOCKET curSocket, char * mensaje, int mensajeTamano)
{
	//send es un metodo de winsock
    return send(curSocket, mensaje, mensajeTamano, 0);
}


int ServicioRed::recibirMensaje(SOCKET curSocket, char * buffer, int bufferTamano)
{
	//recv es un metodo de winsock
    return recv(curSocket, buffer, bufferTamano, 0);
}
