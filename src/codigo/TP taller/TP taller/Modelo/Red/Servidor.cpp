#include "Servidor.h"
#include <iostream>
#include <time.h>
#include <fstream>

unsigned int Servidor::cliente_id; 

Servidor::Servidor(void){
    // id's to assign clients for our table
    cliente_id = 0;
	MAX_CLIENTES = 4;
	clienteEnEspera = false;
	this->clientes = new cliente[MAX_CLIENTES +1];
	for(int i=0; i<MAX_CLIENTES +1; i++){
		this->clientes[i].activo=false;
		this->clientes[i].time=0;
		this->clientes[i].username= "";
		this->clientes[i].socket = INVALID_SOCKET;
	}
    // set up the server network to listen 
    red = new ServidorRed(); 

	this->escenario = ParserYaml::getParser()->getEscenario();
}

void Servidor::actualizar() 
{
	if(red->aceptarNuevoCliente()){
		this->clientes[MAX_CLIENTES].socket = red->sessions.at(0); //es el cliente en espera
	}

	recibirDeClientes();
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

void Servidor::enviarPaquete(SOCKET sock, int tipoPaquete, char* mensaje){

	int largoMensaje = strlen(mensaje)+1;
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

void Servidor::enviarTodosLosClientes(int tipoPaquete, string mensaje){
	for(int i=0; i < MAX_CLIENTES +1; i++){
		enviarPaquete(clientes[i].socket, tipoPaquete, mensaje);
	}
}

int Servidor::buscarCliente(string nombre){

	for(int i=0; i< MAX_CLIENTES; i++){
		if(clientes[i].username == nombre) return i;
	}
	return -1;
}

void Servidor::recibirDeClientes()
{
    Paquete* paquete = new Paquete();
    // go through all clients
    //std::map<unsigned int, SOCKET>::iterator iter;

    //for(iter = red->sessions.begin(); iter != red->sessions.end(); iter++)
	for(int i=0; i < MAX_CLIENTES +1; i++)
    {
        // get data for that client
        int data_length = 0;
		int id;
		if(clientes[i].socket != INVALID_SOCKET) data_length = red->recibirData(clientes[i].socket, network_data);
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
							clientes[id].activo=true;					//descongelo y doy bienvenida
							clientes[id].time=time(NULL);
							clientes[id].socket = red->sessions.at(0);
							enviarPaquete(clientes[id].socket, paqueteInicial, "Bienvenido de nuevo, "+clientes[id].username+ ".");
							cout<<clientes[id].username<<" se ha reconectado."<<endl;
						}else{										//si no esta congelado, es xq ya existe un usuario con ese nombre
							enviarPaquete(clientes[i].socket, paqueteFinal, "Ya existe otro usuario con su nombre.");
						}
					}else{											//si no existe username, tengo que ver si hay lugar para uno nuevo
						if(cliente_id < MAX_CLIENTES){				//si hay lugar 
							
							this->clientes[cliente_id].activo=true;			//le asigno un espacio y doy la bienvenida
							this->clientes[cliente_id].username = paquete->getMensaje();
							this->clientes[cliente_id].time = time(NULL);
							this->clientes[cliente_id].socket = red->sessions.at(0);
							
						
						//	unsigned long iFileSize = 0;
						//	long size;
						//	ifstream infile(texturaTerreno, ios::in|ios::binary);
						//	infile.seekg (0, ios::end);
						//	size = infile.tellg();
						////	cout << size << endl;
						//	infile.seekg (0, ios::beg);
						//	char *terreno = new char[size];  
						//	infile.read (terreno, size);
						//	infile.close();

							//envio [ TIPO | ALTOPX | ANCHOPX | ALTOU | ANCHOU | NIVELAGUA ]
							int tipoPaquete = 1;
							int peso = ((2*sizeof(int)) + (4*sizeof(double)));
							char *data = new char[peso];
							//cout<<peso<<endl;
							int offset = 0;
							memcpy(data+offset, &tipoPaquete, sizeof(tipoPaquete)); //TIPO
							offset = sizeof(tipoPaquete);
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
							
							Servicio::enviarMensaje(clientes[cliente_id].socket, data, peso);

							//AHORA ENVIO LAS IMAGENES DEL TERRENO Y EL CIELITO LINDO:
							char *newfilename;
							unsigned long iFileSize = 0;
							unsigned int size;     //file size
							//mascara1.png tiene q estar en la altura de
							ifstream infile(texturaTerreno, ios::in|ios::binary);
							infile.seekg (0, ios::end);
							size = infile.tellg();
							infile.seekg (0, ios::beg);
							newfilename = new char[size];  
							infile.read (newfilename, size);
							infile.close();
							int tamanioPaqueteImagen = 2*sizeof(int)+size;
							char *Paqueteimagen = new char[tamanioPaqueteImagen];

							offset = 0;
							memcpy(data+offset, &paqueteImagen, sizeof(int)); //TIPO
							offset = sizeof(int);
							memcpy(data+offset, &size, sizeof(size));	//TAMANIO DE LA IMAGEN
							offset += sizeof(size);
							memcpy(data+offset, newfilename, size); //TERRENO

							Servicio::enviarMensaje(clientes[cliente_id].socket, Paqueteimagen, tamanioPaqueteImagen);
							
							//----------------------------------------------------------------------------------------------------------------------------
							enviarPaquete(clientes[cliente_id].socket, paqueteDescargaLista, "Bienvenido, "+clientes[cliente_id].username+".");
							cout<<clientes[cliente_id].username<<" se ha conectado."<<endl;
							cliente_id++;
							//delete terreno;
							delete data;

						}else{
																	//si no hay lugar, lo saco
							enviarPaquete(clientes[i].socket, paqueteFinal, "Ya se ha alcanzado la cantidad maxima de clientes.");
						}
					}
                    break;

                case paqueteEvento:

					printf("El servidor recibio un paquete evento del cliente %i.\n", i);
					clientes[i].ultimoEventoSerializado = paquete->getMensaje();
                    break;

				case paqueteEstado:

					this->clientes[i].time = time(NULL);
					break;

                default:

                    printf("Error en el tipo de paquete.\n");
                    break;
            }
        }
    }

	for(int i=0; i< MAX_CLIENTES; i++){
		if(clientes[i].activo){
			if(time(NULL) - clientes[i].time > 1){	//1 segundo de espera
				clientes[i].activo=false;
				clientes[i].socket = INVALID_SOCKET;
				cout<<clientes[i].username<<" se ha desconectado."<<endl;
			}
		}
	}


	delete paquete;
}