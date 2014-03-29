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

	//reservo espacio para mi matriz
	this->matrizTerreno = new bool* [anchoMatriz];
	for(int i = 0; i< anchoMatriz; i++){
		this->matrizTerreno[i] = new bool[altoMatriz];
	}
	
	if((!imagen)||(!IMG_isPNG(rwop))){ //si no existe el archivo o no es PNG
		
		if(!imagen) logError->escribir("Error 001: no se encontró el archivo de terreno '" + string(nombreArchivo) + "'.");
		if(imagen && !IMG_isPNG(rwop)) logError->escribir("Error 002: El archivo de terreno no es de formato PNG o está dañado.");
		logError->escribir("Se generará una imagen de terreno aleatoria.");
		generarTerrenoAleatorio(nombreArchivo);
		//al generar una nueva imagen , ya voy a tener la matriz de terreno cargada en memoria, asi que terminé (por eso el else)
	}else{
		//si no hay error tengo que cargar mi matriz de terreno
		//convierto RGBA a matriz y chequeo errores de terreno
		RGB_AMatrizBool();
	}

	//cuando ya tengo mi matriz valida, la adapto para que entre en el escenario
	escalarMatrizAEscenario();

	logError->guardarEstado();
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

	//si hubo algun tipo de error con la imagen, lo logueo y genero matriz de terreno aleatorio.
	if(cantErrores > 0){

		loguearErroresMatriz(pixelesInvalidos, columnasInvalidas);

		generarTerrenoAleatorio(mascaraTerrenoDEF);
	}
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

void LectorTerreno::guardarMatrizEnPNG(char* nombreArchivo, bool transparente){

	Uint32* vectorPixeles = new Uint32[altoMatriz*anchoMatriz];
	SDL_Surface* surNueva = IMG_Load(propiedadesPNG);

	Uint32 pCielo = transparente? SDL_MapRGBA(surNueva->format, 0xFF, 0xFF, 0xFF, 0x00) : SDL_MapRGB(surNueva->format, 0xFF, 0xFF, 0xFF);
	Uint32 pTierra = transparente? SDL_MapRGBA(surNueva->format, 0xFF, 0xFF, 0xFF, 0xFF) : SDL_MapRGB(surNueva->format, 0x00, 0x00, 0x00);

	for(unsigned y = 0; y < altoMatriz; y++)
		for(unsigned x = 0; x < anchoMatriz; x++){
			vectorPixeles[x + (y*anchoMatriz)] = (matrizTerreno[x][y])?  pTierra : pCielo; 
		}
	surNueva->h = altoMatriz;
	surNueva->w = anchoMatriz;
	surNueva->pitch = anchoMatriz*4;	//pitch: tamaño en bits de cada linea (ancho x bytes por pixel)
	surNueva->pixels = vectorPixeles;
	IMG_SavePNG(surNueva, nombreArchivo);

	delete[] vectorPixeles;
}

int LectorTerreno::getTamanoBorde(){

	int num = 0;
	for(int i=0; i<this->anchoMatriz; i++) if(matrizTerreno[i][altoMatriz]) num++;

	return num;
}


void LectorTerreno::escalarMatrizAEscenario(){

	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	int altoEscenario = e->altoPx;
	int anchoEscenario = e->anchoPx;
	double escalaX = (double)anchoEscenario/(double)this->anchoMatriz;
	double escalaY = (double)altoEscenario/(double)this->altoMatriz;	

	//reservo espacio para matriz escalada
	bool** matrizEscalada;
	matrizEscalada = new bool* [anchoEscenario];
	for(int i = 0; i < anchoEscenario; i++){
		matrizEscalada[i] = new bool[altoEscenario];
	}

	//mapeo matrices
	for(int i=0; i<altoEscenario; i++)
		for(int j=0; j<anchoEscenario; j++)
			matrizEscalada[j][i] = matrizTerreno[(int)(j/escalaX)][(int)(i/escalaY)];

	//borro matriz vieja
	for(int j=0 ; j< this->anchoMatriz; j++)
		delete[] matrizTerreno[j];

	delete[] this->matrizTerreno;

	//guardo matriz nueva
	this->anchoMatriz = anchoEscenario;
	this->altoMatriz = altoEscenario;
	this->matrizTerreno = matrizEscalada;

}

char* LectorTerreno::getRutaTexturaActualizada(){

	this->guardarMatrizEnPNG(texturaTerreno, true);
	return texturaTerreno;
}

void LectorTerreno::loguearErroresMatriz(vector<punto> pixeles, vector<int> columnas){

	if(pixeles.size() > 0){
		logError->escribir("Error 003: Mascara de terreno, se encontraron " + to_string((long long)pixeles.size()) + " pixeles de color invalido.");
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

void LectorTerreno::generarTerrenoAleatorio(char* nombreArchivo){

	generarMatrizAleatoria();
	guardarMatrizEnPNG(nombreArchivo, false);
}

bool LectorTerreno::esBlanco(pixel p){
	return((p.R == 0xFF)&&(p.G == 0xFF)&&(p.B == 0xFF))? true:false;
}

bool LectorTerreno::esNegro(pixel p){
	return((p.R == 0x00)&&(p.G == 0x00)&&(p.B == 0x00))? true:false;
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

LectorTerreno::~LectorTerreno(){
	for(int j=0 ; j< this->anchoMatriz; j++)
		delete[] matrizTerreno[j];

	delete[] this->matrizTerreno;
}
