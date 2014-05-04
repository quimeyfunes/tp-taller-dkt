#include "BuscadorArchivos.h"

BuscadorArchivos::BuscadorArchivos(string ruta, LPCSTR nombreBuscado){

	this->ruta = ruta;
	this->busqueda = nombreBuscado;
	this->encontrados = new vector<archivo*>;

	this->nombreConRuta = new char[1000];
	this->aBuscar = new char[1000];
	strcpy(this->aBuscar, this->ruta.c_str());
	strcpy(this->aBuscar+this->ruta.size(), this->busqueda);
}

vector<archivo*>* BuscadorArchivos::buscarTodos(){

	if(this->encontrados->size() == 0){ //para buscar una sola vez

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile(aBuscar, &FindFileData);
	
		if (hFind == INVALID_HANDLE_VALUE){
			archivo* invalido = new archivo;
			invalido->rutaCompleta= "invalida";
			invalido->pesoBytes= -1;
			encontrados->push_back(invalido);

		}else{
			do{
				strcpy(nombreConRuta, ruta.c_str());
				strcpy(nombreConRuta+ruta.size(), FindFileData.cFileName);
				archivo* archivoEncontrado = new archivo;
				archivoEncontrado->rutaCompleta = nombreConRuta;
				archivoEncontrado->pesoBytes = FindFileData.nFileSizeLow;
				this->encontrados->push_back(archivoEncontrado);

			}while (FindNextFile(hFind, &FindFileData));   
		}
		 FindClose(hFind);
	}

	return this->encontrados;
	
}


BuscadorArchivos::~BuscadorArchivos(void){

	delete[] this->nombreConRuta;
	delete[] this->aBuscar;

	for(int i=0; i< this->encontrados->size(); i++)
		delete this->encontrados->at(i);

	delete this->encontrados;
}
