#include "Servidor.h"

Servidor::Servidor(void)
{

    // create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
	this->socketEscuchador = INVALID_SOCKET;
    this->socketClientes = INVALID_SOCKET;



    // address info for the server to listen to
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup fallo con error: %d\n", iResult);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

	  // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if ( iResult != 0 ) {
        printf("getaddrinfo fallo con error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    this->socketEscuchador = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (this->socketEscuchador == INVALID_SOCKET) {
        printf("socket fallo con error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    iResult = ioctlsocket(this->socketEscuchador, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("ioctlsocket fallo con error: %d\n", WSAGetLastError());
        closesocket(this->socketEscuchador);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind( this->socketEscuchador, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("bind fallo con error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(this->socketEscuchador);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(this->socketEscuchador, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen fallo con error: %d\n", WSAGetLastError());
        closesocket(this->socketEscuchador);
        WSACleanup();
        exit(1);
    }
}


bool Servidor::acceptarNuevoCliente(unsigned int & id)
{
    // if client waiting, accept the connection and save the socket
    this->socketClientes = accept(this->socketEscuchador,NULL,NULL);

    if (this->socketClientes != INVALID_SOCKET) 
    {
        //disable nagle on the client's socket
        char value = 1;
        setsockopt( this->socketClientes, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

        // insert new client into session id table
        sessions.insert( pair<unsigned int, SOCKET>(id, this->socketClientes) );

        return true;
    }

    return false;
}