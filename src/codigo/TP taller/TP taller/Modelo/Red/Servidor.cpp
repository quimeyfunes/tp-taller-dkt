#include "Servidor.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <process.h>
#include "../BuscadorArchivos.h"


unsigned int Servidor::cliente_id; 
string Servidor::dibujablesSerializados;
ServidorRed* Servidor::red;
cliente* Servidor::clientes;
EscenarioParseado* Servidor::escenario;
mensajeStru Servidor::mensaje;
explosion** Servidor::exp;
int Servidor::idJugando=0; //este es el id q va a arrancar a jugar.
int Servidor::tiempo=-1;

Servidor::Servidor(){

	

    // id's to assign clients for our table
    cliente_id = 0;
	clienteEnEspera = false;
	this->escenario = ParserYaml::getParser()->getEscenario();
	
	exp = new explosion*[escenario->maximosClientes +1];
	for(int i=0; i< escenario->maximosClientes+1; i++){
		exp[i] = new explosion[maxExplosionesPorTurno];
	}

	for(int i=0; i< escenario->maximosClientes +1; i++){
		for(int j=0; j < maxExplosionesPorTurno; j++){

			exp[i][j].radio = -1;
			exp[i][j].x = 0;
			exp[i][j].y = 0;
		}
	}


	this->mensaje.tiempoActivo=0;
	this->clientes = new cliente[escenario->maximosClientes +1];
	for(int i=0; i< escenario->maximosClientes +1; i++){
		this->clientes[i].activo=false;
		this->clientes[i].time=0;
		this->clientes[i].username= "";
		this->clientes[i].socket = INVALID_SOCKET;
		this->clientes[i].enviandoData = false;
		this->clientes[i].muerto = false;
		this->clientes[i].puedeJugar = false;
	}
    // set up the server network to listen 
    red = new ServidorRed(); 
	
	//creo un thread q se dedique a escuchar a los clientes entrantes
	_beginthread(Servidor::aceptarClientes, 0, (void*)escenario->maximosClientes);

}

Servidor::~Servidor(){
	delete this->clientes;

};

void Servidor::aceptarClientes(void* arg){

	while(true){
		if(red->aceptarNuevoCliente()){

			//seteo el socket en espera en el espacio N
			
			clientes[escenario->maximosClientes].socket = red->sockNuevo;
			clientes[escenario->maximosClientes].activo = true;
			_beginthread(Servidor::actualizar, 0, (void*)escenario->maximosClientes);
			//cliente_id++;
		}
	}
}

void Servidor::actualizar(void* clienteN) 
{
	int id= (int)clienteN;
	audioEnCola** colaDeSonidos;
	int tiempo = 0;

	while(clientes[id].activo){
			
		//recibe los mensajes que mandan otros clientes sin chocar en los threads
		if(time(NULL) - mensaje.tiempoActivo == 0){
			if(mensaje.emisor != clientes[id].socket)
				enviarPaquete(clientes[id].socket, paqueteMensajeInfo, mensaje.msj);
		}else{
			mensaje.tiempoActivo=0;
		}

		recibirDeCliente(&id);
		enviarCliente(&id, paqueteVista, dibujablesSerializados);
		
		for(int i=0; i< maxExplosionesPorTurno; i++){
			if(exp[id][i].radio >= 0){
				if(clientes[id].socket != INVALID_SOCKET){
					enviarExplosion(clientes[id].socket, exp[id][i]);
					exp[id][i].radio = -1;
				}
			}
		}
	
		//envio el tiempo del reloj a los clientes:5
		if(Servidor::tiempo != -1 && Servidor::tiempo <= tiempoTurno){ 
			enviarCliente(&id,paqueteTiempo, StringUtil::int2string(Servidor::tiempo));
		}

		colaDeSonidos = Reproductor::getReproductor()->getColaDeEspera();
		for(int i=0; i< numSonidos; i++){
			if(!colaDeSonidos[id][i].enviado){
				EnviarSonido(id, colaDeSonidos[id][i]);
				colaDeSonidos[id][i].enviado = true;
			}
		}
	}
}

void Servidor::enviarExplosion(SOCKET s, explosion e){

	string expSerializado;

	expSerializado = StringUtil::int2string((int)e.x);
	expSerializado += separadorCamposArreglo;
	expSerializado += StringUtil::int2string((int)e.y);
	expSerializado += separadorCamposArreglo;
	expSerializado += StringUtil::int2string((int)e.radio);

	enviarPaquete(s, paqueteExplosion, expSerializado);
}

void Servidor::EnviarSonido(int id, audioEnCola aMandar){

	string sonidoSerializado;
	if(aMandar.reproducir) 
		sonidoSerializado = "1";
	else
		sonidoSerializado = "0";

	sonidoSerializado += separadorCamposArreglo;
	sonidoSerializado += StringUtil::int2string((int)aMandar.s);
	enviarPaquete(clientes[id].socket, paqueteSonido, sonidoSerializado);
}

void Servidor::enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje){

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

void Servidor::enviarCliente(int* clienteN, int tipoPaquete, string mensaje){

		if(clientes[*clienteN].socket != INVALID_SOCKET) enviarPaquete(clientes[*clienteN].socket, tipoPaquete, mensaje);

}

void Servidor::setTerrenoModificado(explosion e){
	
	for(int i=0; i< escenario->maximosClientes; i++){
		for(int j=0; j < maxExplosionesPorTurno; j++){
			if(exp[i][j].radio == -1){
				exp[i][j].radio = e.radio;
				exp[i][j].x = e.x;
				exp[i][j].y = e.y;
				break;
			}
		}
	}
}

int Servidor::buscarCliente(string nombre){

	for(int i=0; i< escenario->maximosClientes; i++){
		if(clientes[i].username == nombre) return i;
	}
	return -1;
}

void Servidor::enviarEscenario(int num_cliente, bool reconectado){

		//envio [ TIPO | LARGO RUTA MASCARA | ALTOPX | ANCHOPX | ALTOU | ANCHOU | NIVELAGUA | ID_CLIENTE | MAX_CLIENTES | STRING RUTA MASCARA ]
	int tipoPaquete = 1;
	string rutaMascara;

	if(reconectado) rutaMascara = texturaTerreno + StringUtil::int2string(99) + ".png";	//99 es el numero que usa el servidor para su terrenoActual
	else rutaMascara = escenario->imagenTierra;

	int tamanoRutaMascara = rutaMascara.size()+1;

	int peso = ((5*sizeof(int)) + (4*sizeof(double)) + tamanoRutaMascara);
	char *data = new char[peso];
	//cout<<peso<<endl;
	int offset = 0;
	memcpy(data+offset, &tipoPaquete, sizeof(tipoPaquete)); //TIPO
	offset = sizeof(tipoPaquete);
	memcpy(data+offset, &tamanoRutaMascara, sizeof(tamanoRutaMascara));
	offset += sizeof(tamanoRutaMascara);
	memcpy(data+offset, &escenario->altoPx, sizeof(escenario->altoPx));	//altopx
	offset += sizeof(escenario->altoPx);
	memcpy(data+offset, &escenario->anchoPx, sizeof(escenario->anchoPx)); //anchopx
	offset += sizeof(escenario->anchoPx);
	memcpy(data+offset, &escenario->altoU, sizeof(escenario->altoU));	//altoU
	offset += sizeof(escenario->altoU);
	memcpy(data+offset, &escenario->anchoU, sizeof(escenario->anchoU));	//anchoU
	offset += sizeof(escenario->anchoU);
	memcpy(data+offset, &escenario->nivelAgua, sizeof(escenario->nivelAgua)); //nivelAgua
	offset += sizeof(escenario->nivelAgua);
	memcpy(data+offset, &num_cliente, sizeof(num_cliente));			//ID_CLIENTE
	offset += sizeof(num_cliente);
	memcpy(data+offset, &escenario->maximosClientes, sizeof(escenario->maximosClientes)); //MAXIMOS_CLIENTES
	offset += sizeof(escenario->maximosClientes);
	strcpy(data+offset, rutaMascara.c_str());
							
	if(clientes[num_cliente].socket != INVALID_SOCKET) Servicio::enviarMensaje(clientes[num_cliente].socket, data, peso);
	Sleep(10);
	delete data;
}

void Servidor::enviarImagenes(SOCKET sock){
	
	BuscadorArchivos *mascaras = new BuscadorArchivos("imagenes/mascaras/","*.png");
	vector<archivo*>* mascarasImgs = mascaras->buscarTodos();
	for(int i=0;i<mascarasImgs->size(); i++){
		enviarImagen(sock, mascarasImgs->at(i)->rutaCompleta, paqueteTextura );
		Sleep(40);
	}

	BuscadorArchivos *texturas = new BuscadorArchivos("imagenes/texturas/","*.png");
	vector<archivo*>* texImgs = texturas->buscarTodos();
	for(int i=0;i<texImgs->size(); i++){
		enviarImagen(sock, texImgs->at(i)->rutaCompleta, paqueteTextura );
		Sleep(40);
	}

	//envio el .ICO
	BuscadorArchivos* buscadorICO = new BuscadorArchivos("imagenes/texturas/", "*.ICO");
	vector<archivo*>* icono = buscadorICO->buscarTodos();
	for(int i=0;i < icono->size(); i++){
		enviarImagen(sock, icono->at(i)->rutaCompleta, paqueteTextura );
		Sleep(40);
	}

	delete mascaras;
	delete texturas;
	delete buscadorICO;
}

void Servidor::recibirDeCliente(int* clienteN)
{
    Paquete* paquete = new Paquete();
	// data buffer
   char network_data[MAX_PACKET_SIZE];

    // get data for that client
    int data_length = 0;
	int id;
	data_length = red->recibirData(clientes[*clienteN].socket, network_data);
    int cantData = 0;
    while (cantData < data_length) 
    {
        paquete->deserializar(&(network_data[cantData]));
		cantData+= paquete->getPesoPaquete();
		switch (paquete->getTipo()) {

            case paqueteInicial:	//en el getMensaje tengo el username
				id = buscarCliente(paquete->getMensaje());
				if(id != -1){										//si existe el username
					if(!clientes[id].activo){						//si ese username esta congelado

											//descongelo y doy bienvenida
						clientes[id].activo=true;
						clientes[id].time=time(NULL);
						clientes[id].socket = clientes[*clienteN].socket;
						*clienteN = id;

						//cliente_id--;

						//le vuelvo a enviar todas las cosas, por si se reconecta en otra pc

						enviarImagenes(clientes[*clienteN].socket);
						enviarEscenario(*clienteN, true);
						enviarPaquete(clientes[*clienteN].socket, paqueteDescargaLista, "");
						
						cout<<clientes[*clienteN].username<<" se ha reconectado."<<endl;

						//hago que el bienvenido le aparezca en la vista al cliente
						enviarPaquete(clientes[*clienteN].socket, paqueteMensajeInfo, "Bienvenido de nuevo, "+clientes[*clienteN].username + ".");

						//hago que el resto sepa que se reconectó el cliente
						//for(int cont=0; cont < escenario->maximosClientes; cont++){
						//	if((clientes[cont].username != clientes[*clienteN].username)&&(clientes[cont].activo)){
						//		if(clientes[cont].socket != INVALID_SOCKET) enviarPaquete(clientes[cont].socket, paqueteMensajeInfo, clientes[*clienteN].username +" se ha reconectado.");
						//	}
						//}
						mensaje.emisor=clientes[*clienteN].socket;
						mensaje.msj = clientes[*clienteN].username +" se ha reconectado.";
						mensaje.tiempoActivo=time(NULL);
						enviarPaquete(clientes[*clienteN].socket,paqueteArranque,"dale q va");

						for (std::list<Gusano*>::const_iterator it = clientes[*clienteN].figuras.begin(); it != clientes[*clienteN].figuras.end(); it++) {
							(*it)->setCongelado(false);
						}
						Sleep(100);
						

					}else{										//si no esta congelado, es xq ya existe un usuario con ese nombre
						enviarPaquete(clientes[*clienteN].socket, paqueteFinal, "Ya existe otro usuario con su nombre.");
						//cliente_id--;
						clientes[*clienteN].activo=false;
					}
				}else{											//si no existe username, tengo que ver si hay lugar para uno nuevo
					if(cliente_id < escenario->maximosClientes){				//si hay lugar 
						
						for(int i=0; i<escenario->maximosClientes; i++){
							if(clientes[i].username==""){
								*clienteN = i;
								break;
							}
						}

						clientes[*clienteN].username = paquete->getMensaje();
						clientes[*clienteN].socket = clientes[escenario->maximosClientes].socket;
						
						//le asigno un espacio y doy la bienvenida
						
						enviarImagenes(clientes[*clienteN].socket);
						enviarEscenario(*clienteN, false);
						clientes[*clienteN].time = time(NULL);
						enviarPaquete(clientes[*clienteN].socket, paqueteDescargaLista, "");
						cout<<clientes[*clienteN].username<<" se ha conectado."<<endl;
						mensaje.tiempoActivo=time(NULL);

						mensaje.emisor=clientes[*clienteN].socket;
						mensaje.msj = clientes[*clienteN].username +" se ha conectado.";
						mensaje.tiempoActivo=time(NULL);

						enviarPaquete(clientes[*clienteN].socket, paqueteMensajeInfo, "Bienvenido, "+clientes[*clienteN].username + ".");

						clientes[*clienteN].activo=true;
						clientes[*clienteN].puedeJugar = true;
						cliente_id++;

					}else{
																//si no hay lugar, lo saco
						enviarPaquete(clientes[*clienteN].socket, paqueteFinal, "Ya se ha alcanzado la cantidad maxima de clientes.");
						//cliente_id--;
						clientes[*clienteN].activo=false;
					}
				}
                break;

            case paqueteEvento:
				if(clientes[*clienteN].username == clientes[Servidor::idJugando].username){
					clientes[*clienteN].ultimoEventoSerializado = paquete->getMensaje();
				}
				else{
					//cout << "espere su turno, ahora esta jugando " << clientes[Servidor::idJugando].username <<endl;
					//clientes[*clienteN].ultimoEventoSerializado = paquete->getMensaje();
				}
                break;

			case paqueteEstado:
					clientes[*clienteN].time = time(NULL);
				break;

            default: break;
        }

    }
    

	if(clientes[*clienteN].activo){
		if(time(NULL) - clientes[*clienteN].time > 3){	//3 segundos de espera
			
			clientes[*clienteN].socket = INVALID_SOCKET;
			cout<<clientes[*clienteN].username<<" se ha desconectado."<<endl;

			mensaje.emisor=clientes[*clienteN].socket;
			mensaje.msj = clientes[*clienteN].username +" se ha desconectado.";
			mensaje.tiempoActivo=time(NULL);

			for (std::list<Gusano*>::const_iterator it = clientes[*clienteN].figuras.begin(); it != clientes[*clienteN].figuras.end(); it++) {
				(*it)->setCongelado(true);
			}
			clientes[*clienteN].activo=false;
		}
	}
	delete paquete;
}

void Servidor::enviarImagen(SOCKET sock, string direccion, int tipoPaquete){

	char *newfilename;
	unsigned int size;     //file size
	ifstream infile(direccion, ios::in|ios::binary);
	infile.seekg (0, ios::end);
	size = infile.tellg();
	infile.seekg (0, ios::beg);
	newfilename = new char[size];  
	infile.read (newfilename, size);
	infile.close();
	//el Tamanio del paquete es: TIPO_PAQUETE + TAMANIO_IMAGEN + DIRECCION + IMAGEN 
	int tamanioPaqueteImagen = ( 2*sizeof(int) ) + strlen(direccion.c_str())+1 + size;
	char *dataImagen = new char[tamanioPaqueteImagen];

	int offset = 0;
	memcpy(dataImagen+offset, &tipoPaquete, sizeof(int)); //TIPO
	offset += sizeof(int);
	memcpy(dataImagen+offset, &size, sizeof(int));	//TAMANIO DE LA IMAGEN
	offset += sizeof(int);
	strcpy(dataImagen+offset, direccion.c_str());	//DIRECCION DE LA IMAGEN
	offset += strlen(direccion.c_str())+1;
	memcpy(dataImagen+offset, newfilename, size); //IMAGEN

	Servicio::enviarMensaje(sock, dataImagen, tamanioPaqueteImagen);
	delete dataImagen;
	delete newfilename;
}

string Servidor::siguienteJugador(){
	bool b = false;
	//entra hasta el siguiente jugador activo.
	while(b == false){

	if(Servidor::idJugando+1 == Servidor::cliente_id)		Servidor::idJugando = 0;
	else													Servidor::idJugando++;

	if(clientes[Servidor::idJugando].activo == true && !clientes[Servidor::idJugando].muerto ) b = true;	
	
	}

	return clientes[Servidor::idJugando].username;
	
	
}

unsigned int Servidor::getCantidadDeClientes(){
	return Servidor::cliente_id;
}


void Servidor::darArranque(){
	
	for(int id=0; id < Servidor::cliente_id; id++ ){

			if( (clientes[id].activo) && (clientes[id].socket != INVALID_SOCKET) ){	
				clientes[id].puedeJugar = true;
				Sleep(100);
				enviarPaquete(clientes[id].socket,paqueteArranque,"dale q va");
			}
	}
}



bool Servidor::tieneGusanosVivos(int idCliente){

	list<Gusano*> gusanos = clientes[idCliente].figuras;
	list<Gusano*> gusanosVivos;
	list<Gusano*>::iterator it;
	for(it = gusanos.begin();it != gusanos.end();it++){
		if(!(*it)->estaMuerto()) gusanosVivos.push_back(*it);
	}
	if(gusanosVivos.size() > 0 ) return true;
	else return false;

}