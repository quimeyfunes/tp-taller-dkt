#include "LectorTerreno.h"
#include "generador\GeneradorFunciones.h"
#include <string>

LectorTerreno::LectorTerreno(char* nombreArchivo){

	logError = Logger::getLogger();
	imagen = IMG_Load(nombreArchivo);
	SDL_RWops *rwop = SDL_RWFromFile(nombreArchivo, "rb"); //para chequear si es PNG
	
	//asigno valores de alto y ancho
	this->anchoMatriz = (!imagen)? anchoPxDEF : imagen->w;
	this->altoMatriz =  (!imagen)? altoPxDEF  : imagen->h;
	this->rutaMascaraUsada = nombreArchivo;

	//reservo espacio para mi matriz
	this->matrizTerreno = new bool* [anchoMatriz];
	for(int i = 0; i< anchoMatriz; i++){
		this->matrizTerreno[i] = new bool[altoMatriz];
	}
	
	if((!imagen)||(!IMG_isPNG(rwop))){ //si no existe el archivo o no es PNG

		loguearErroresPNG(imagen, IMG_isPNG(rwop));
		generarTerrenoAleatorio(nombreArchivo);
		//al generar una nueva imagen , ya voy a tener la matriz de terreno cargada en memoria, asi que terminé (por eso el else)
	}else{
		//si no hay error tengo que cargar mi matriz de terreno
		//convierto RGBA a matriz y chequeo errores de terreno
		RGB_AMatrizBool();
	}
}

void LectorTerreno::RGB_AMatrizBool(){

	pixel pixActual;
	punto posPixel;
	vector<punto> pixelesInvalidos;
	vector<int> columnasInvalidas;
	int cantErrores = 0;

	Uint32* vectorPixeles = (Uint32*)imagen->pixels;

	for(int i=0; i<altoMatriz; i++){
		for(int j=0 ; j<anchoMatriz; j++){
			
			SDL_GetRGB(vectorPixeles[j + (i*imagen->w)], imagen->format, &pixActual.R, &pixActual.G, &pixActual.B);

			if(esBlanco(pixActual) || esNegro(pixActual)){ //chequea que todos los pixeles sean blancos o negros
				matrizTerreno[j][i] = esNegro(pixActual);
			}else{
				cantErrores++;
				posPixel.x = j;
				posPixel.y = i;
				pixelesInvalidos.push_back(posPixel);
			}
		}
	}
	//hago una nueva pasada columna por columna para chequear TCT
	//de haber errores, devuelvo las columnas
	columnasInvalidas = chequearTCT(cantErrores);

	//si hubo algun tipo de error con la imagen, genero matriz de terreno aleatorio.
	if(cantErrores > 0){

		loguearErroresMatriz(pixelesInvalidos, columnasInvalidas);

		generarTerrenoAleatorio(imagenTerrenoDEF);
		this->rutaMascaraUsada = imagenTerrenoDEF;
	}
}

void LectorTerreno::loguearErroresMatriz(vector<punto> pixeles, vector<int> columnas){

	if(pixeles.size() > 0){
		logError->escribir("Error 003: Mascara de terreno, se encontraron " + to_string((long long)pixeles.size()) + "pixeles de color invalido.");
		if(pixeles.size() <= maxPixelesInvalidosMascara){
			//loguear posicion de cada pixel invalido
			for(int i=0; i< pixeles.size(); i++)
				logError->escribir("		columna: " + to_string((long long)pixeles.at(i).x) + ", fila: " + to_string((long long)pixeles.at(i).y) + ".");
		}
	}

	if(columnas.size() > 0){
		logError->escribir("Error 004: Mascara de terreno, se encontraron " + to_string((long long)columnas.size()) + " columnas invalidas.");
		if(columnas.size() <= maxColumnasInvalidasMascara){
			//loguear posicion de cada columna invalida
			for(int i=0; i< columnas.size(); i++)
				logError->escribir("		columna: " + to_string((long long)columnas.at(i)) + ".");
		}
	}

	logError->escribir("Se generará un terreno aleatorio.");
	logError->guardarEstado();
}

void LectorTerreno::loguearErroresPNG(bool existePNG, bool esPNG){


}

void LectorTerreno::generarMatrizAleatoria(){

	//genero una funcion aleatoria
	double* f = new double[anchoMatriz];
	GeneradorFunciones gen;
	gen.generarFuncionFourier(f, anchoMatriz, fMinDEFAULT, fMaxDEFAULT);
	//convierto la funcion a una matriz
	cargarFuncionEnMatriz(f);
	//ya tengo guardada la matriz aleatoria, borro la funcion
	delete[] f;
}

void LectorTerreno::cargarFuncionEnMatriz(double* f){

	for(int i=0; i< this->altoMatriz; i++){
		for(int j=0; j<this->anchoMatriz; j++){

			matrizTerreno[j][i] = (f[j] < i)? true:false;
		}
	}
}

vector<int> LectorTerreno::chequearTCT(int &numErrores){

	int numeroCambios;
	vector<int> columnas;
	bool pixelActual;
	bool pixelAnterior;

	for(int i=0; i<this->anchoMatriz; i++){
		pixelActual = matrizTerreno[i][0];
		numeroCambios = 0;
		//por cada columna, si detecto mas de un cambio de pixeles se informa del error
		for(int j=1; j<this->altoMatriz; j++){
			pixelAnterior = pixelActual;
			pixelActual = matrizTerreno[i][j];
			if(pixelAnterior != pixelActual) numeroCambios++;
		}

		if(numeroCambios > 1){
			numErrores++;
			columnas.push_back(i);
		}
	}
	return columnas;
}

bool** LectorTerreno::getMatrizTerreno(){
	return this->matrizTerreno;
}

int LectorTerreno::getAnchoMatriz(){
	return this->anchoMatriz;
}

int LectorTerreno::getAltoMatriz(){
	return this->altoMatriz;
}

char* LectorTerreno::getRutaMascaraUsada(){
	return this->rutaMascaraUsada;
}

pixel LectorTerreno::boolAPixel(bool valor){
	pixel p;

	if(valor){	//valor == true , pixel negro

		p.R = 0x00;
		p.G = 0x00;
		p.B = 0x00;

	}else{ //valor == false, pixel blanco
		p.R = 0xFF;
		p.G = 0xFF;
		p.B = 0XFF;
	}

	return p;
}

bool LectorTerreno::esBlanco(pixel p){
	return((p.R == 0xFF)&&(p.G == 0xFF)&&(p.B == 0xFF))? true:false;
}

bool LectorTerreno::esNegro(pixel p){
	return((p.R == 0x00)&&(p.G == 0x00)&&(p.B == 0x00))? true:false;
}

void LectorTerreno::generarTerrenoAleatorio(char* nombreArchivo){

	generarMatrizAleatoria();
	guardarMatrizEnPNG(nombreArchivo);
}

void LectorTerreno::guardarMatrizEnPNG(char* nombreArchivo){

	Uint32* vectorPixeles = new Uint32[altoMatriz*anchoMatriz];
	SDL_Surface* surNueva = IMG_Load(propiedadesPNG);

	for(unsigned y = 0; y < altoMatriz; y++)
		for(unsigned x = 0; x < anchoMatriz; x++){
			vectorPixeles[x + (y*anchoMatriz)] = (matrizTerreno[x][y])? SDL_MapRGB(surNueva->format, 0x00, 0x00, 0x00) : SDL_MapRGB(surNueva->format, 0xFF, 0xFF, 0xFF);
		}
	surNueva->h = altoMatriz;
	surNueva->w = anchoMatriz;
	surNueva->pitch = anchoMatriz*4;	//pitch: tamaño en bits de cada linea (ancho x bytes por pixel)
	surNueva->pixels = vectorPixeles;
	IMG_SavePNG(surNueva, nombreArchivo);

	delete[] vectorPixeles;
}

LectorTerreno::~LectorTerreno(){
	for(int j=0 ; j< this->anchoMatriz; j++)
		delete[] matrizTerreno[j];

	delete[] this->matrizTerreno;
}
