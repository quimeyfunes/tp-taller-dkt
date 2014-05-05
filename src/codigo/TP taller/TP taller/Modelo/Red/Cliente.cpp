#include "Cliente.h" 
#include <iostream>
#include <fstream>

Cliente::Cliente(string nombre, string ip){
    red = new ClienteRed(ip);
	this->username=nombre;
	this->escenario = new EscenarioParseado();
	this->activo = false;
	enviarPaquete(red->socketCliente, paqueteInicial, this->username);
}

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
	
	int offset;
	Paquete* paquete = new Paquete();
    // get data from server
    int data_length = red->recibirData(network_data);
	fstream archTerreno;
		
	int tipoPaquete;

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

		if(tipoPaquete != paqueteInicial){
			paquete->deserializar(&(network_data[i]));
			i += paquete->getPesoPaquete();	//devuelve sizeof(tipoPaquete) + sizeof(tamanioMensaje) + strlen(mensaje)
		}
		else{
			i+= (2*sizeof(int)) + (4*sizeof(double));	//2 ints: TIPO, NIVELAGUA		
		}												//4 doubles: ALTOPX, ANCHOPX, ALTOU, ANCHOU	
			

		switch (tipoPaquete) {

            case paqueteInicial:

				//recibo [ TIPO | ALTOPX | ANCHOPX | ALTOU | ANCHOU | NIVELAGUA ]

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
				
				break;

			case paqueteImagen:
				//recibo imagenTierra e Imagen Cielo
				offset = 2*sizeof(int); // TIPO_PAQUETE+TAMANIO
				archTerreno.open(texturaTerreno, std::ofstream::binary);
				archTerreno.seekp(0, ios::beg);
				archTerreno.write(network_data+offset, paquete->getTamanio());
				archTerreno.close();
				this->escenario->imagenTierra = texturaTerreno;
				//this->escenario->imagenCielo = texturaCielo;
				break;

			case paqueteDescargaLista:

				cout<<paquete->getMensaje();
				this->activo = true;
				delete paquete;
				return false;
				break;

            case paqueteEvento:

				printf("El cliente recibio un paquete evento del servidor.\n");
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
    }
	delete paquete;
		
return true;
}


void Cliente::actualizar() 
{
		recibirDeServidor();
		if(activo)enviarEstado();
}
