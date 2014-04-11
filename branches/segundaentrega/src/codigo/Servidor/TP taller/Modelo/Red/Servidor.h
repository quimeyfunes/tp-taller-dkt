#include "ServidorRed.h"

class Servidor
{

public:

    Servidor(void);
    ~Servidor(void);

    void actualizar();

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int cliente_id;

   // The ServerNetwork object 
    ServidorRed* red;
};