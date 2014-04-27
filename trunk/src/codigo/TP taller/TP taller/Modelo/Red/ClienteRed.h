#include <winsock2.h>
#include <Windows.h>
#include "Servicio.h"
#include <ws2tcpip.h>
#include <stdio.h> 

// size of our buffer
#define DEFAULT_BUFLEN 512
// port to connect sockets through 
#define DEFAULT_PORT "6881"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClienteRed
{

public:

    // para chequear errores en las llamadas a funcioes de winsock
    int iResult;

    // socket para el cliente para conectar con el servidor
    SOCKET socketCliente;

    // ctor/dtor
    ClienteRed(string ip);
    ~ClienteRed(void);
	int recibirData( char * recvbuf);
};