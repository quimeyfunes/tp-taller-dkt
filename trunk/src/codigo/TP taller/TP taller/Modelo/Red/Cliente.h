#include "ClienteRed.h"

class Cliente
{

public:

    Cliente(string nombre);
    ~Cliente(void);
	string username;
    ClienteRed* red; 
	void recibirDeServidor();
	void enviarEstado();
	void actualizar();

private:
	bool activo;
	char network_data[MAX_PACKET_SIZE];
	void enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje);
};
