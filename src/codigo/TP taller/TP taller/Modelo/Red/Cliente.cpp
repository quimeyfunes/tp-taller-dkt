#include "Cliente.h" 
#include <iostream>
#include <fstream>

Cliente::Cliente(string nombre, string ip){

	TIEMPO_MAX_ESPERA = 2;

    red = new ClienteRed(ip);
	this->username=nombre;
	this->escenario = new EscenarioParseado();
	this->activo = false;
	enviarPaquete(red->socketCliente, paqueteInicial, this->username);
	this->mensajeInfo = "";
	this->nuevoMensaje = false;
	this->timeServidor = time(NULL);
}

 Cliente::~Cliente(){
	delete this->escenario;
 };

void Cliente::enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje){

	int largoMensaje = mensaje.size()+1;
	int paquete_tamano = sizeof(int) + sizeof(int) + largoMensaje;
    char* paquete_data = new char[paquete_tamano];

    Paquete* paquete = new Paquete();
    paquete->setTipo(tipoPaquete);
	paquete->setMensaje(mensaje);
	paquete->setTamanio(largoMensaje);
    paquete->serializar(paquete_data);
	Servicio::enviarMensaje(sock, paquete_data, paquete_tamano);
	delete paquete;
	delete paquete_data;
}

void Cliente::enviarEstado(){

	enviarPaquete(red->socketCliente, paqueteEstado, this->username);
}

void Cliente::enviarEvento(string eventoSerializado){

	enviarPaquete(red->socketCliente, paqueteEvento, eventoSerializado);
}

EscenarioParseado* Cliente::getEscenarioActual(){
	return this->escenario;
}

bool Cliente::recibirDeServidor(){
	
	bool retorno=true;
	int offset;
	Paquete* paquete = new Paquete();
    // get data from server
    int data_length = red->recibirData(network_data);
	ofstream archTerreno;
		
	int tipoPaquete;
	unsigned int tamanioImagen;
	char* dir = new char[200];
	

    if (data_length <= 0) 
    {
        //no data recieved
		//ver q hacerrrrrrrrrrrrr
    }
		
    int i = 0;
	while (i < data_length) 
    {
		//obtengo el tipo del paquete
		//si tipo = 1 es un arreglo de ints del Escenario
		memcpy(&tipoPaquete, &network_data[i], sizeof(int));
		switch (tipoPaquete) {

			case paqueteInicial:
				 i+= (4*sizeof(int)) + (4*sizeof(double));
				 break;

			case paqueteTextura:
				//aumenta el i despues.
				break;

			default:
				paquete->deserializar(&(network_data[i]));
				i += paquete->getPesoPaquete();	//devuelve sizeof(tipoPaquete) + sizeof(tamanioMensaje) + strlen(mensaje)
				break;
		}

		/*
		if(tipoPaquete != paqueteInicial && tipoPaquete != paqueteImagen ){
			paquete->deserializar(&(network_data[i]));
			i += paquete->getPesoPaquete();	//devuelve sizeof(tipoPaquete) + sizeof(tamanioMensaje) + strlen(mensaje)
		}
		else{
			if(tipoPaquete == paqueteInicial) i+= (2*sizeof(int)) + (4*sizeof(double));	//2 ints: TIPO, NIVELAGUA; 4 doubles: ALTOPX, ANCHOPX, ALTOU, ANCHOU	
		}
			
		*/

		switch (tipoPaquete) {

            case paqueteInicial:

				//recibo [ TIPO | ALTOPX | ANCHOPX | ALTOU | ANCHOU | NIVELAGUA | ID_CLIENTE | MAX_CLIENTES]

				offset = sizeof(tipoPaquete);
				memcpy(&escenario->altoPx, network_data+offset, sizeof(escenario->altoPx));	//altopx
				offset += sizeof(escenario->altoPx);
				memcpy(&escenario->anchoPx, network_data+offset, sizeof(escenario->anchoPx)); //anchopx
				offset += sizeof(escenario->anchoPx);
				memcpy(&escenario->altoU, network_data+offset, sizeof(escenario->altoU));	//altoU
				offset += sizeof(escenario->altoU);
				memcpy(&escenario->anchoU, network_data+offset, sizeof(escenario->anchoU));	//anchoU
				offset += sizeof(escenario->anchoU);
				memcpy(&escenario->nivelAgua, network_data+offset, sizeof(escenario->nivelAgua)); //nivelAgua
				offset += sizeof(escenario->nivelAgua);
				memcpy(&cliente_id, network_data+offset, sizeof(cliente_id));	//cliente_id
				offset += sizeof(cliente_id);
				memcpy(&escenario->maximosClientes, network_data+offset, sizeof(escenario->maximosClientes)); //MAX_clientes
				break;

			case paqueteTextura:
				//RECIBE TODAS LAS TEXTURAS
				memcpy(&tamanioImagen, &network_data[i]+sizeof(int), sizeof(int));
				offset = 2*sizeof(int); // TIPO_PAQUETE+TAMANIO

				strcpy(dir,&network_data[i]+offset);
				offset += strlen(dir)+1;

				archTerreno.open(dir, std::ofstream::binary);
				archTerreno.seekp(0, ios::beg);
				archTerreno.write(&network_data[i]+offset, tamanioImagen);
				archTerreno.close();
				i+= offset+tamanioImagen;
				break;


			case paqueteDescargaLista:
				//ya termino de recibir las texturas
				this->escenario->imagenTierra = texturaTerreno;
				this->escenario->imagenCielo = texturaCielo;
				this->activo = true;
				retorno=false;
				break;

			case paqueteMensajeInfo:

				this->mensajeInfo = paquete->getMensaje();
				this->nuevoMensaje = true;
				break;

            case paqueteEvento:

				//printf("El cliente recibio un paquete evento del servidor.\n");
				break;

			case paqueteVista:
				//printf("El cliente recibio un paquete vista del servidor.\n");
				this->vistaSerializada = paquete->getMensaje();
				break;

			case paqueteFinal:
				cout<<paquete->getMensaje();
				this->activo=false;
				break;

            default:

                //printf("Error en el tipo de paquete.Tipo es %d\n",paquete->getTipo());

                break;
        }

		//si estoy aca es xq el servidor me envio un paquete, actualizo el timeServidor
		this->timeServidor = time(NULL);
    }
	delete paquete;

	//verifico q el servidor no esté caido
	if(time(NULL) - this->timeServidor > TIEMPO_MAX_ESPERA){
		this->mensajeInfo = "El servidor se encuentra caido.";
		this->nuevoMensaje = true;
	}
		
return retorno;
}


void Cliente::actualizar() 
{
		recibirDeServidor();
		if(activo)enviarEstado();
}

string Cliente::getUserName(){
	return this->username;
}

int Cliente::getId(){
	return this->cliente_id;
}