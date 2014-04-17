#include "ServidorRed.h"

class Servidor
{

public:

    Servidor(void);
    ~Servidor(void);

    void actualizar();
	void recibirDeClientes();
private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int cliente_id;
	// data buffer
   char network_data[MAX_PACKET_SIZE];

   // The ServerNetwork object 
    ServidorRed* red;
};