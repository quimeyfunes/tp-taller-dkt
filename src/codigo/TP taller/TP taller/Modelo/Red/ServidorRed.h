#include <winsock2.h>
#include <Windows.h>
#include "Servicio.h"
#include <ws2tcpip.h>
#include <map>

using namespace std; 
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881" 

class ServidorRed
{

public:

    ServidorRed(void);
    ~ServidorRed(void);

    // Socket to listen for new connections
    SOCKET socketEscuchador;

    // Socket to give to the clients
    SOCKET socketClientes;

    // for error checking return values
    int iResult;

    SOCKET sockNuevo; 

	// accept new connections
    bool aceptarNuevoCliente();

	// receive incoming data
    int recibirData(SOCKET socket, char * recvbuf);
};