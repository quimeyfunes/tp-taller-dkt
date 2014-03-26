#include "lectorTerreno.h"
#include "generador\GeneradorFunciones.h"
#include <string>

lectorTerreno::lectorTerreno(char* nombreArchivo){

	logError = Logger::getLogger();
	imagen = IMG_Load(nombreArchivo);
	SDL_RWops *rwop = SDL_RWFromFile(nombreArchivo, "rb"); //para chequear si es PNG
	
	//asigno valores de alto y ancho
	this->anchoMatriz = (!imagen)? anchoPxDEF : imagen->w;
	this->altoMatriz =  (!imagen)? altoPxDEF  : imagen->h;

	//reservo espacio para mi matriz
	this->matrizTerreno = new bool* [anchoMatriz];
	for(int i = 0; i< anchoMatriz; i++){
		this->matrizTerreno[i] = new bool[altoMatriz];
	}
	
	if((!imagen)||(!IMG_isPNG(rwop))){ //si no existe el archivo o no es PNG

		if(!imagen) logError->escribir("Error 001: no se encontró el archivo de terreno '" + string(nombreArchivo) + "'.");
		if(!IMG_isPNG(rwop)) logError->escribir("Error 002: El archivo de terreno no es de formato PNG o está dañado.");
		logError->escribir("Se generará una imagen de terreno aleatoria.");
		generarTerrenoAleatorio(nombreArchivo);
		//al generar una nueva imagen , ya voy a tener la matriz de terreno cargada en memoria, asi que terminé (por eso el else)
	}else{
		//si no hay error tengo que cargar mi matriz de terreno
		//convierto RGBA a matriz y chequeo errores de terreno
		RGB_AMatrizBool();
	}
}

void lectorTerreno::RGB_AMatrizBool(){

	pixel pixActual;
	bool errorBN = false;	//chequea q todos los pixeles sean blancos o negros
	bool errorTCT = false;	//chequea q no haya una columna con tierra-cielo-tierra
	Uint32* vectorPixeles = (Uint32*)imagen->pixels;

	for(int i=0; (i<altoMatriz)&&(!errorBN); i++){
		for(int j=0 ; (j<anchoMatriz)&&(!errorBN); j++){
			
			SDL_GetRGB(vectorPixeles[j + (i*imagen->w)], imagen->format, &pixActual.R, &pixActual.G, &pixActual.B);
			if(esBlanco(pixActual) || esNegro(pixActual)){ //chequea que todos los pixeles sean blancos o negros
				matrizTerreno[j][i] = esNegro(pixActual);
			}else{
				errorBN = true;
				logError->escribir("Error 003: Imagen de terreno. Pixel de color invalido en columna: " +  to_string((long long)j) + " y fila: " + to_string((long long)i));
				logError->escribir("Se generará un terreno aleatorio.");
			}
		}
	}
	//hago una nueva pasada columna por columna para chequear TCT
	//de haber errores, devuelvo la columna
	int columnaError ;
	if(!errorBN) columnaError = chequearTCT(errorTCT);
	if(errorTCT){
			//escribo en el log la columna invalida
			logError->escribir("Error 004: Imagen de terreno. La columna " + (to_string((long long)columnaError)) + " contiene un patron invalido de cielo y tierra.");
			logError->escribir("Se generará un terreno aleatorio.");
	}

	//si hubo algun tipo de error con la imagen, genero matriz de terreno aleatorio.
	if(errorBN || errorTCT){
		generarMatrizAleatoria();
	}
}

void lectorTerreno::generarMatrizAleatoria(){

	//genero una funcion aleatoria
	double* f = new double[anchoMatriz];
	GeneradorFunciones gen;
	gen.generarFuncionFourier(f, anchoMatriz, fMinDEFAULT, fMaxDEFAULT);
	//convierto la funcion a una matriz
	cargarFuncionEnMatriz(f);
	//ya tengo guardada la matriz aleatoria, borro la funcion
	delete[] f;
}

void lectorTerreno::cargarFuncionEnMatriz(double* f){

	for(int i=0; i< this->altoMatriz; i++){
		for(int j=0; j<this->anchoMatriz; j++){

			matrizTerreno[j][i] = (f[j] < i)? true:false;
		}
	}
}

int lectorTerreno::chequearTCT(bool &error){

	int numeroCambios;
	int columnaError = -1;
	bool pixelActual;
	bool pixelAnterior;

	for(int i=0; (i<this->anchoMatriz)&&(!error); i++){
		pixelActual = matrizTerreno[i][0];
		numeroCambios = 0;
		//por cada columna, si detecto mas de un cambio de pixeles se informa del error
		for(int j=1; (j<this->altoMatriz)&&(!error); j++){
			pixelAnterior = pixelActual;
			pixelActual = matrizTerreno[i][j];
			if(pixelAnterior != pixelActual) numeroCambios++;
		}

		if(numeroCambios > 1){
			error = true;
			columnaError = i;
		}
	}
	return columnaError;
}

bool** lectorTerreno::getMatrizTerreno(){
	return this->matrizTerreno;
}

int lectorTerreno::getAnchoMatriz(){
	return this->anchoMatriz;
}

int lectorTerreno::getAltoMatriz(){
	return this->altoMatriz;
}

pixel lectorTerreno::boolAPixel(bool valor){
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

bool lectorTerreno::esBlanco(pixel p){
	return((p.R == 0xFF)&&(p.G == 0xFF)&&(p.B == 0xFF))? true:false;
}

bool lectorTerreno::esNegro(pixel p){
	return((p.R == 0x00)&&(p.G == 0x00)&&(p.B == 0x00))? true:false;
}

void lectorTerreno::generarTerrenoAleatorio(char* nombreArchivo){

	generarMatrizAleatoria();
	guardarMatrizEnPNG(nombreArchivo);
}

void lectorTerreno::guardarMatrizEnPNG(char* nombreArchivo){

	Uint32* vectorPixeles = new Uint32[altoMatriz*anchoMatriz];
	SDL_Surface* surNueva = IMG_Load(imagenTerrenoDEF);

	for(unsigned y = 0; y < altoMatriz; y++)
		for(unsigned x = 0; x < anchoMatriz; x++)
			vectorPixeles[x + (y*anchoMatriz)] = (matrizTerreno[x][y])? 0xFF000000 : 0xFFFFFFFF;
				
	surNueva->h = altoMatriz;
	surNueva->w = anchoMatriz;
	surNueva->pitch = anchoMatriz*4;	//pitch: tamaño en bits de cada linea (ancho x bytes por pixel)
	surNueva->pixels = vectorPixeles;
	IMG_SavePNG(surNueva, nombreArchivo);

	delete[] vectorPixeles;
}

lectorTerreno::~lectorTerreno(){
	for(int j=0 ; j< this->anchoMatriz; j++)
		delete[] matrizTerreno[j];

	delete[] this->matrizTerreno;
}
