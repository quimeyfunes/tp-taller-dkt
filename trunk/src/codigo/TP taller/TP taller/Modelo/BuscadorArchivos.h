#pragma once
#include<windows.h>
#include<iostream>
#include <vector>
#include<string>

using namespace std;

typedef struct{
	string rutaCompleta;
	int pesoBytes;
}archivo;

class BuscadorArchivos{

public:

	BuscadorArchivos(string ruta, LPCSTR nombreBuscado);
	~BuscadorArchivos();
	vector<archivo*>* buscarTodos();

private:

	vector<archivo*>* encontrados;
	string ruta;
	LPCSTR busqueda;
	char* nombreConRuta;
	char* aBuscar;

};

/*EJEMPLO DE USO:

	BuscadorArchivos* buscador = new BuscadorArchivos("imagenes/texturas/", "*.png");
	vector<archivo*>* encontrados = buscador->buscarTodos();

	for(int i=0; i< encontrados->size(); i++){
	
		cout<<encontrados->at(i)->rutaCompleta<<": "<<encontrados->at(i)->pesoBytes<<" bytes."<<endl;
	}

	delete buscador;

*/