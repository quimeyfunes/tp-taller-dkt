#include "ServidorRed.h"
#include "../Figuras/Gusano.h"
#include "../../Parser/yaml/ParserYaml.h"
#include "../../constantes.h"
#include "../../constantesArmas.h"
#include "../StringUtil.h"
#include "../Reproductor.h"

typedef struct structCliente{
	string username;
	int time;
	SOCKET socket;
	bool enviandoData;
	bool activo;
	bool muerto;
	list<Gusano*> figuras;
	string ultimoEventoSerializado;
	bool puedeJugar;
	//Sesion* sesion;
}cliente;

typedef struct{
	SOCKET emisor;
	string msj;
	int tiempoActivo;
}mensajeStru;

class Servidor
{

public:

    Servidor();
    ~Servidor();
	static int tiempo;
	static mensajeStru mensaje;
	static EscenarioParseado* escenario;
    static void actualizar(void* clienteN);
	static void recibirDeCliente(int* clienteN);
	static void enviarCliente(int* clienteN, int tipoPaquete, string mensaje);
	static string dibujablesSerializados;
	static explosion** exp;
	static void enviarImagen(SOCKET sock, string direccion, int tipoPaquete);
	static ServidorRed* red;
	static void aceptarClientes(void* arg);
	static void enviarEscenario(int num_cliente, bool reconectado);
	static void enviarImagenes(SOCKET sock);
	static void enviarExplosion(SOCKET sock, explosion e);
	static cliente* clientes;
	static void setTerrenoModificado(explosion e);
	static string siguienteJugador();
	static unsigned int getCantidadDeClientes();
	static void darArranque();
	static int buscarCliente(string nombre);
	bool tieneGusanosVivos(int idCliente);

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int cliente_id;

   static void EnviarSonido(int id, audioEnCola aMandar);
   static void enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje);
   bool clienteEnEspera;
   static int idJugando;

   // The ServerNetwork object 

   
    
};