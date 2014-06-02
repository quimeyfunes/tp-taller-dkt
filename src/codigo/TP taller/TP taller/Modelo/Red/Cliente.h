#include "ClienteRed.h"
#include "../../Parser/yaml/ParserYaml.h"
#include "../StringUtil.h"

class Cliente
{

public:
    Cliente(string nombre, string ip);
    ~Cliente();
	string username;
    ClienteRed* red; 
	bool recibirDeServidor();
	void enviarEstado();
	void enviarEvento(string eventoSerializado);
	void actualizar();
	EscenarioParseado* getEscenarioActual();
	string vistaSerializada;
	string getUserName();
	int getId();
	int getTiempoActualDeJuego();
	void setTiempoActualDeJuego(int tiempo);
	//cambiar esto por una lista de mensajes (talvez)
	string mensajeInfo;
	bool nuevoMensaje;
	//

	int timeServidor;

private:
	int tiempoActualDeJuego;
	int TIEMPO_MAX_ESPERA;
	int cliente_id;
	EscenarioParseado* escenario;
	bool activo;
	char network_data[MAX_PACKET_SIZE];
	void enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje);
	
};
