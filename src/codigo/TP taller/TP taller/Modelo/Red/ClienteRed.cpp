#include "ClienteRed.h"

ClienteRed::ClienteRed(string ip) {

    // create WSADATA object
    WSADATA wsaData;

    // socket
    this->socketCliente = INVALID_SOCKET;

    // holds address info for socket to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup fallo con error: %d\n", iResult);
        exit(1);
    }



    // set address info
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!


	//resolve server address and port 
	char* _ip = new char[ip.size()+1];
	strcpy(_ip, ip.c_str());
    iResult = getaddrinfo(_ip, DEFAULT_PORT, &hints, &result);
	delete _ip;

    if( iResult != 0 ) 
    {
        printf("getaddrinfo fallo con error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        this->socketCliente = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);

        if (this->socketCliente == INVALID_SOCKET) {
            printf("socket fallo con error: %ld\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        iResult = connect( this->socketCliente, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            closesocket(this->socketCliente);
            this->socketCliente = INVALID_SOCKET;
            printf ("El servidor esta caido. No se pudo conectar.");
        }
    }



    // no longer need address info for server
    freeaddrinfo(result);



    // if connection failed
    if (this->socketCliente == INVALID_SOCKET) 
    {
        printf("No se pudo conectar con el servidor!\n");
        WSACleanup();
        exit(1);
    }

	// Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    iResult = ioctlsocket(this->socketCliente, FIONBIO, &iMode);
    if (iResult == SOCKET_ERROR)
    {
        printf("ioctlsocket fallo con error: %d\n", WSAGetLastError());
        closesocket(this->socketCliente);
        WSACleanup();
        exit(1);        
    }

	//disable nagle
    char value = 1;
    setsockopt( this->socketCliente, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

}

int ClienteRed::recibirData( char * recvbuf)
{
    
        SOCKET currentSocket = this->socketCliente;
        iResult = Servicio::recibirMensaje(currentSocket, recvbuf, MAX_PACKET_SIZE);
        if (iResult == 0)
        {
            printf("Coneccion cerrada\n");
            closesocket(currentSocket);
        }
        return iResult;
    
    return 0;
} 