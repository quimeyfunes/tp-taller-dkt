#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "Paquete.h"
#include "../../constantes.h"

class Servicio{

public:

static int enviarMensaje(SOCKET curSocket, char * mensaje, int mensajeTamano);
static int recibirMensaje(SOCKET curSocket, char * buffer, int bufferTamano);

};