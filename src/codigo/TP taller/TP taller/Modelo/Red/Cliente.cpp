#include "Cliente.h" 
#include <iostream>
#include <fstream>

Cliente::Cliente(string nombre, string ip, string &msjError){

	TIEMPO_MAX_ESPERA = 2;
	
	this->exp = new explosion[maxExplosionesPorTurno];
	for(int i=0; i< maxExplosionesPorTurno; i++){
		exp[i].radio = -1;
		exp[i].x=0;
		exp[i].y=0;
	}

	this->arrancarJuego = false;
    red = new ClienteRed(ip, msjError);
	this->username=nombre;
	this->escenario = new EscenarioParseado();
	this->activo = false;
	if ( msjError == "") enviarPaquete(red->socketCliente, paqueteInicial, this->username);
	this->mensajeInfo = "";
	this->nuevoMensaje = false;
	this->timeServidor = time(NULL);
	this->partidaTerminada = false;
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

bool Cliente::recibirDeServidor(string &msjError){
	
	bool retorno=true;
	int offset;
	int tamanoRutaMascara = 0;
	Paquete* paquete = new Paquete();
    // get data from server
    int data_length = red->recibirData(network_data);
	ofstream archTerreno;
		
	int tipoPaquete;
	unsigned int tamanioImagen;
		
    int i = 0;
	while (i < data_length) 
    {
		//obtengo el tipo del paquete
		//si tipo = 1 es un arreglo de ints del Escenario
		memcpy(&tipoPaquete, &network_data[i], sizeof(int));
		switch (tipoPaquete) {

			case paqueteInicial:
				memcpy(&tamanoRutaMascara, &network_data[i] + sizeof(int), sizeof(int));
				 i+= (5*sizeof(int)) + (4*sizeof(double)) + tamanoRutaMascara;
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
				{
				//recibo [ TIPO | LARGO RUTA MASCARA | ALTOPX | ANCHOPX | ALTOU | ANCHOU | NIVELAGUA | ID_CLIENTE | MAX_CLIENTES | STRING RUTA MASCARA ]
				
				offset = sizeof(tipoPaquete) + sizeof(tamanoRutaMascara);
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
				offset += sizeof(escenario->maximosClientes);
				char* buff = new char[tamanoRutaMascara];
				strcpy(buff, network_data+offset);
				string s(buff);
				escenario->imagenTierra = s;
				delete buff;
				break;
				}
			case paqueteTextura:
				//RECIBE TODAS LAS TEXTURAS
				{
					char* dir = new char[200];
					memcpy(&tamanioImagen, &network_data[i]+sizeof(int), sizeof(int));
					offset = 2*sizeof(int); // TIPO_PAQUETE+TAMANIO

					strcpy(dir,&network_data[i]+offset);
					offset += strlen(dir)+1;

					archTerreno.open(dir, std::ofstream::binary);
					archTerreno.seekp(0, ios::beg);
					archTerreno.write(&network_data[i]+offset, tamanioImagen);
					archTerreno.close();
					i+= offset+tamanioImagen;

					enviarEstado();
					delete dir;
					break;
				}

			case paqueteDescargaLista:
				//ya termino de recibir las texturas
				//this->escenario->imagenTierra = texturaTerreno;
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
				this->activo=false;
				msjError = paquete->getMensaje();
				retorno=false;
				break;

			case paqueteTiempo:
				 this->setTiempoActualDeJuego(StringUtil::str2int(paquete->getMensaje()));
				break;

			case paqueteArranque:
				this->arrancarJuego = true;
				break;

			case paquetePartidaTerminada:
				this->arrancarJuego = false;
				this->partidaTerminada = true;
				break;

			case paqueteSonido:
				{
					
					vector<string> deserializado = StringUtil::split(paquete->getMensaje(), separadorCamposArreglo);
					bool reproducir = (deserializado.at(0) == "1")? true:false;
					int s = StringUtil::str2int(deserializado.at(1));
					if(reproducir) Reproductor::getReproductor()->reproducirSonido((sonido)s);
					else Reproductor::getReproductor()->detenerSonido((sonido)s);
					break;

				}

			case paqueteExplosion:
				{
					vector<string> deserializado = StringUtil::split(paquete->getMensaje(), separadorCamposArreglo);
					for(int i=0; i< maxExplosionesPorTurno; i++){
						if(exp[i].radio == -1){
							exp[i].x = StringUtil::str2int(deserializado.at(0));
							exp[i].y = StringUtil::str2int(deserializado.at(1));
							exp[i].radio = StringUtil::str2int(deserializado.at(2));
							break;
						}
					}
					//cout<<"me llego explosion"<<endl;
					break;
				}
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
	string mensajeError="";
		recibirDeServidor(mensajeError);
		if(activo)enviarEstado();
}

string Cliente::getUserName(){
	return this->username;
}

int Cliente::getId(){
	return this->cliente_id;
}

int Cliente::getTiempoActualDeJuego(){
	if( this->tiempoActualDeJuego > 0 && this->tiempoActualDeJuego <= tiempoTurno) return this->tiempoActualDeJuego;
	else return tiempoTurno;
	
}

void Cliente::setTiempoActualDeJuego(int tiempo){
	this->tiempoActualDeJuego = tiempo;
}


void Cliente::enviarQuieroJugar(){
	enviarPaquete(red->socketCliente,paqueteQuieroJugar,"quiero jugar :D");
}