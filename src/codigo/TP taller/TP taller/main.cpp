#include "Modelo/Juego/JuegoCliente.h"
#include "Modelo/Juego/Turno.h"
#include <process.h>
#include <winsock2.h>
#include <stdlib.h>
#include <crtdbg.h>

char* obtenerIPMaquina(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	hostent* host = gethostbyname("");
	in_addr* address = (in_addr*)host->h_addr;
	char* ip = new char[20];
	ip = inet_ntoa(*address);
return ip;
}

string obtenerAnteriorIP(){
	ifstream archivoIP;
	archivoIP.open(rutaArchivoIP);
	if(archivoIP){
		string ip;
		archivoIP>>ip;
		return ip;
	}

	return "";
}

int main(int argc, char* argv[]){
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	ParserYaml::setConfigPath("config/config.yaml");
	try{
		printf("Ingrese 1 para ser cliente, 2 para ser servidor.\n");
		std::string argumento;
		std::cin >> argumento;
		if(argumento == "1"){
			bool listo = false;
			printf("Soy Cliente.\n");
			printf("Ingrese su nombre de usuario: ");
			string nombre;
			cin.ignore();
			getline(cin, nombre);
			string ip = obtenerAnteriorIP();
			while(!listo){
				if(ip == ""){
					ofstream archivoIP;
					archivoIP.open(rutaArchivoIP, ios::trunc);
					printf("Ingrese la direccion IP del servidor: ");
					cin>>ip;
					archivoIP<<ip;
					archivoIP.close();
					listo=true;
				}else{
					cout<<"La direccion IP establecida es: "<<ip<<endl;
					string respuesta="";
					while(respuesta != "s" && respuesta != "n"){
						printf("Correcto? (s/n) ");
						cin>>respuesta;
					}
					if(respuesta == "s") listo= true;
					else ip = "";
				}
			}

			JuegoCliente* juego = new JuegoCliente(nombre, ip);
			juego->ejecutar();
			delete juego;
		}else if(argumento == "2"){
			//Servidor
			system("cls");
			printf("Soy Servidor!\n");
			char* ip = obtenerIPMaquina();
			printf("Mi direccion IP es: %s\n", ip);
			Juego* juego = new Juego("a");
			juego->ejecutar();
			delete juego;
		}
	}catch(exception &e){
		Logger::getLogger()->escribir(e.what());
		Logger::getLogger()->guardarEstado();
	}
	/*Turno *turno = new Turno();
	turno->comenzar();
	while(turno->estaTerminado() == false){
		cout << turno->getTiempoActual() << endl;
		if (turno->getTiempoActual() == 4) turno->esperarDisparo();
		turno->actualizar();
	}

	
	int b =2;
	if(b == 1) cout << "if " << endl;
	else cout << "else " << endl;

	char a;
	cin >> a;*/

	return 0;    
}