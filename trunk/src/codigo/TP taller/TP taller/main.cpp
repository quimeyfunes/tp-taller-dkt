#include "Modelo/Juego/JuegoCliente.h"
#include "Modelo/Juego/Turno.h"
#include "Vista\Menu\Menu.h"
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

void inicializarSDL(SDL_Window** window,SDL_Renderer** renderer) {
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	SDL_Init( SDL_INIT_EVERYTHING );
	*window = SDL_CreateWindow("Worms!", 25, 25, e->anchoPx, e->altoPx,  SDL_WINDOW_SHOWN);
	
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetWindowIcon(*window, IMG_Load(rutaIcono));
	TTF_Init();
}

int main(int argc, char* argv[]){
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	ParserYaml::setConfigPath("config/config.yaml");
	try{
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		inicializarSDL(&window,&renderer);
		Menu* menu = new Menu(window,renderer);
		int accion = 0;
		SDL_StartTextInput();
		string ip = obtenerAnteriorIP();
		menu->setIP(ip);
		while (accion == 0) {
			menu->dibujar();
			accion = menu->leerEvento();
		}
		SDL_StopTextInput();
		/*printf("Ingrese 1 para ser cliente, 2 para ser servidor.\n");
		std::string argumento;
		std::cin >> argumento;*/
		//SDL_HideWindow(window);
		if (accion == nameMenu::CLIENTE) { 
			//(argumento == "1"){
			bool listo = false;
			/*printf("Soy Cliente.\n");
			printf("Ingrese su nombre de usuario: ");
			string nombre;
			//cin.ignore();
			getline(cin, nombre);*/
			string nombre = menu->getNombre();
			string ip2 = menu->getIP();
			/*string ip = obtenerAnteriorIP();
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
			}*/
			JuegoCliente* juego = new JuegoCliente(nombre, ip2, window, renderer,menu);
			juego->ejecutar(); 
			delete juego;
		}else if (accion == nameMenu::SERVIDOR) {
			//if(argumento == "2"){
			//Servidor
			system("cls");
			printf("Soy Servidor!\n");
			char* ip = obtenerIPMaquina();
			printf("Mi direccion IP es: %s\n", ip);
			menu->agregarMensaje(string("Mi direccion IP es: " + string(ip)),30,255,0,0);
			Juego* juego = new Juego("a",window,renderer,menu);
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