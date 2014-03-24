#include "lectorTerreno.h"
#include "generador\GeneradorFunciones.h"

lectorTerreno::lectorTerreno(char* nombreArchivo){

	logError = Logger::getLogger();

	vector<unsigned char> imagen; //vector de pixelesRGBA del terreno
	unsigned altoPx, anchoPx;

	unsigned error = lodepng::decode(imagen, anchoPx, altoPx, nombreArchivo);

	//asigno valores de alto y ancho
	this->anchoMatriz = error? anchoPxDEF : anchoPx;
	this->altoMatriz =  error? altoPxDEF  : altoPx;

	//reservo espacio para mi matriz
	this->matrizTerreno = new bool* [anchoMatriz];
	for(int i = 0; i< anchoMatriz; i++){
		this->matrizTerreno[i] = new bool[altoMatriz];
	}
	
	if(error){ //si no existe el archivo, o no es un PNG
		//error 48: no se encontro el archivo
		//error 28: el archivo no es un PNG o esta corrupto
		if(error == 48) logError->escribir("Error 001: no se encontró el archivo de terreno " + string(nombreArchivo) + ".");
		if(error == 28) logError->escribir("Error 002: el archivo de terreno no es de formato PNG o está corrupto.");

		logError->escribir("Se generará una imagen de terreno aleatoria.");

		//crea un PNG de terreno aleatorio
		generarTerrenoAleatorio(nombreArchivo, anchoMatriz, altoMatriz);
		//al generar una nueva imagen , ya voy a tener la matriz de terreno cargada en memoria, asi que terminé (por eso el else)
		
	}else{
		//si no hay error tengo que cargar mi matriz de terreno
		//convierto RGBA a matriz y chequeo errores de terreno
		RGBA_AMatrizBool(&imagen, this->matrizTerreno);
	}
}

void lectorTerreno::RGBA_AMatrizBool(vector<unsigned char>* imagen, bool** matriz){


	pixel pixActual;
	bool errorBN = false;	//chequea q todos los pixeles sean blancos o negros
	bool errorTCT = false;	//chequea q no haya una columna con tierra-cielo-tierra

	for(int i=0; ((i<altoMatriz)&&(!errorBN)); i++){
		for(int j=0 ; (j<anchoMatriz*4)&&(!errorBN); j+=4){
			pixActual = RGBAaPixel(	(int)imagen->at(i* anchoMatriz*4 + j +0), 
									(int)imagen->at(i* anchoMatriz*4 + j +1), 
									(int)imagen->at(i* anchoMatriz*4 + j +2), 
									(int)imagen->at(i* anchoMatriz*4 + j +3));

			if(esBlanco(pixActual) || esNegro(pixActual)){ //chequea que todos los pixeles sean blancos o negros
				matriz[(int)(j/4)][i] = esNegro(pixActual);
			}else{
				errorBN = true;
				logError->escribir("Error 003: Imagen de terreno. Pixel de color invalido en columna: " + (to_string((long long)((int)(j/4)))) + " y fila: " + (to_string((long long)i)));
				logError->escribir("Se generará un terreno aleatorio.");
			}
		}
	}
	//hago una nueva pasada columna por columna para chequear TCT
	//de haber errores, devuelvo la columna
	int columnaError ;
	if(!errorBN) columnaError = chequearTCT(matrizTerreno, errorTCT);
	if(errorTCT){
			//escribo en el log la columna invalida
			logError->escribir("Error 004: Imagen de terreno. La columna " + (to_string((long long)columnaError)) + " contiene un patron invalido de cielo y tierra.");
			logError->escribir("Se generará un terreno aleatorio.");
	}

	//si hubo algun tipo de error con la imagen, genero matriz de terreno aleatorio.
	if(errorBN || errorTCT){
		generarMatrizAleatoria(this->matrizTerreno);
	}
}

void lectorTerreno::generarMatrizAleatoria(bool** matriz){

	//genero una funcion aleatoria
	double* f = new double[anchoMatriz];
	GeneradorFunciones gen;
	gen.generarFuncionFourier(f, anchoMatriz, fMinDEFAULT, fMaxDEFAULT);
	//convierto la funcion a una matriz
	convertirFuncionAMatriz(f, matriz);
	//ya tengo guardada la matriz aleatoria, borro la funcion
	delete[] f;
}

void lectorTerreno::convertirFuncionAMatriz(double* f, bool** matriz){

	for(int i=0; i< this->altoMatriz; i++){
		for(int j=0; j<this->anchoMatriz; j++){

			matriz[j][i] = (f[j] < i)? true:false;
		}
	}
}

int lectorTerreno::chequearTCT(bool** matriz, bool &error){

	int numeroCambios;
	int columnaError = -1;
	bool pixelActual;
	bool pixelAnterior;

	for(int i=0; (i<this->anchoMatriz)&&(!error); i++){
		pixelActual = matriz[i][0];
		numeroCambios = 0;
		//por cada columna, si detecto mas de un cambio de pixeles se informa del error
		for(int j=1; (j<this->altoMatriz)&&(!error); j++){
			pixelAnterior = pixelActual;
			pixelActual = matriz[i][j];
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

pixel lectorTerreno::RGBAaPixel(int r, int g, int b, int a){
	pixel p; 
	p.R = r;
	p.G = g;
	p.B = b;
	p.A = a;	
	return p;
}

bool lectorTerreno::esBlanco(pixel p){
	return((p.R == 255)&&(p.G == 255)&&(p.B == 255))? true:false;
}

bool lectorTerreno::esNegro(pixel p){
	return((p.R == 0)&&(p.G == 0)&&(p.B == 0))? true:false;
}

void lectorTerreno::generarTerrenoAleatorio(char* nombreArchivo, int ancho, int alto){

}

lectorTerreno::~lectorTerreno(){
	for(int j=0 ; j< this->anchoMatriz; j++)
		delete[] matrizTerreno[j];

	delete[] this->matrizTerreno;

}
