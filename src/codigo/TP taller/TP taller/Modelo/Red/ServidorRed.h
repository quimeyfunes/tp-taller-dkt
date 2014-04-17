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

    // table to keep track of each client's socket
    std::map<unsigned int, SOCKET> sessions; 

	// accept new connections
    bool acceptarNuevoCliente(unsigned int & id);

	// receive incoming data
    int recibirData(unsigned int cliente_id, char * recvbuf);
};