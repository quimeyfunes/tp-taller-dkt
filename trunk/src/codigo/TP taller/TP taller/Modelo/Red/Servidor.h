#include "ServidorRed.h"
#include "../Figuras/Gusano.h"
#include "../../Parser/yaml/ParserYaml.h"

typedef struct structCliente{
	string username;
	int time;
	SOCKET socket;
	bool activo;
	list<Gusano*> figuras;
	string ultimoEventoSerializado;
	//Sesion* sesion;
}cliente;

class Servidor
{

public:

    Servidor(void);
    ~Servidor(void);
	EscenarioParseado* escenario;
    void actualizar(string dibujablesSerializados);
	void recibirDeClientes();
	void enviarAeClientes(char* mensaje);
	void enviarImagen(string direccion, int tipoPaquete);
	void enviarTodosLosClientes(int tipoPaquete, string mensaje);
	static ServidorRed* red;
	static void aceptarClientes(void* arg);
	void enviarEscenario(SOCKET sock);
	void enviarImagenes(SOCKET sock);
	static cliente* clientes;
private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int cliente_id;
	// data buffer
   char network_data[MAX_PACKET_SIZE];
   int buscarCliente(string nombre);
   void enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje);
   void enviarPaquete(SOCKET sock, int tipoPaquete, char* mensaje);
   bool clienteEnEspera;
   
   // The ServerNetwork object 

   
    
};