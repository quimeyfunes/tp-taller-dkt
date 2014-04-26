#include "ServidorRed.h"

class Servidor
{

public:

    Servidor(void);
    ~Servidor(void);

    void actualizar();
	void recibirDeClientes();
	void enviarAeClientes(char* mensaje);
	ServidorRed* red;
private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int cliente_id;
	// data buffer
   char network_data[MAX_PACKET_SIZE];
   int buscarCliente(string nombre);
   void enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje);
   bool clienteEnEspera;
   int MAX_CLIENTES;
   // The ServerNetwork object 

   
	typedef struct structCliente{
	   string username;
	   int time;
	   SOCKET socket;
	   bool activo;
	   //Sesion* sesion;
   }cliente;

   cliente* clientes;
    
};