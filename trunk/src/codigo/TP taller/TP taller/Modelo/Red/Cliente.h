#include "ClienteRed.h"
#include "../../Vista/Vista.h"
#include "../../Modelo/StringUtil.h"

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
	char network_data[MAX_PACKET_SIZE];
};
