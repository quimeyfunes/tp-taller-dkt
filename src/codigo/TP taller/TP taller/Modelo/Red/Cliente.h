#include "ClienteRed.h"
#include "../../Vista/Vista.h"
#include "../../Modelo/StringUtil.h"

class Cliente
{

public:

    Cliente();
    ~Cliente(void);

    ClienteRed* red; 
	void recibirDeServidor();
	void actualizar();

private:
	char network_data[MAX_PACKET_SIZE];
};
