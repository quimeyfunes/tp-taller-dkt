#include "Modelo/Juego/JuegoCliente.h"
#include <process.h>
#include <winsock2.h>

char* obtenerIPMaquina(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	hostent* host = gethostbyname("");
	in_addr* address = (in_addr*)host->h_addr;
	char* ip = new char[20];
	ip = inet_ntoa(*address);
	return ip;
}

int main(int argc, char* argv[]){

	ParserYaml::setConfigPath("config/config.yaml");
	try{
		printf("Ingrese 1 para ser cliente, 2 para ser servidor.\n");
		std::string argumento;
		std::cin >> argumento;
		if(argumento == "1"){
			
			printf("Soy Cliente.\n");
			printf("Ingrese su nombre de usuario: ");
			string nombre;
			cin.ignore();
			getline(cin, nombre);
			printf("Ingrese la dirección IP del servidor: ");
			string ip;
			cin>>ip;
			JuegoCliente* juego = new JuegoCliente(nombre, ip);
			juego->ejecutar();
			delete juego;
		}else if(argumento == "2"){
			//Servidor
			system("cls");
			printf("Soy Servidor!\n");
			char* ip = obtenerIPMaquina();
			printf("Mi dirección IP es: %s\n", ip);
			Juego* juego = new Juego("a");
			juego->ejecutar();
			delete juego;
		}
	}catch(exception &e){
		Logger::getLogger()->escribir(e.what());
		Logger::getLogger()->guardarEstado();
	}
	return 0;    
}