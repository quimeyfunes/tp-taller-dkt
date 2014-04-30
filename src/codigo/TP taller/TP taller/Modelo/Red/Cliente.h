#include "ClienteRed.h"

class Cliente
{

public:

    Cliente(string nombre, string ip);
    ~Cliente(void);
	string username;
    ClienteRed* red; 
	void recibirDeServidor();
	void enviarEstado();
	void enviarEvento(string eventoSerializado);
	void actualizar();
	string vistaSerializada;

private:
	bool activo;
	char network_data[MAX_PACKET_SIZE];
	void enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje);
};
