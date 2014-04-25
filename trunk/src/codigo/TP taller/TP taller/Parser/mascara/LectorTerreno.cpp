#include "LectorTerreno.h"
#include "generador\GeneradorFunciones.h"
#include <string>

LectorTerreno::LectorTerreno(string nombreArchivo){

	logError = Logger::getLogger();
	imagen = IMG_Load(nombreArchivo.c_str());
	//asigno valores de alto y ancho
	this->anchoMatriz = (!imagen)? anchoPxDEF : imagen->w;
	this->altoMatriz =  (!imagen)? altoPxDEF  : imagen->h;

	//reservo espacio para mi matriz
	this->matrizTerreno = new pixel* [anchoMatriz];
	for(int i = 0; i< anchoMatriz; i++){
		this->matrizTerreno[i] = new pixel[altoMatriz];
	}
	
	if(!imagen){ //si no existe el archivo o no es PNG
		
		if(!imagen) logError->escribir("Error en el lector de mascara - No se encontró el archivo de mascara '" + nombreArchivo + "'.");
		logError->escribir("Se generará una imagen de terreno aleatoria.");
		generarTerrenoAleatorio(nombreArchivo);
		//al generar una nueva imagen , ya voy a tener la matriz de terreno cargada en memoria, asi que terminé (por eso el else)
	}else{
		crearMatrizRGBA();
	}

	//cuando ya tengo mi matriz valida, la adapto para que entre en el escenario
	escalarMatrizAEscenario();

	logError->guardarEstado();
}

void LectorTerreno::crearMatrizRGBA(){

	pixel pixActual;
	vector<int> columnasInvalidas;
	int cantErrores = 0;

	Uint32* vectorPixeles = (Uint32*)imagen->pixels;

	for(int i=0; i<altoMatriz; i++){
		for(int j=0 ; j<anchoMatriz; j++){
			
			SDL_GetRGBA(vectorPixeles[j + (i*imagen->w)], imagen->format, &pixActual.R, &pixActual.G, &pixActual.B, &pixActual.A);
			matrizTerreno[j][i] = pixActual;
		}
	}
	//hago una pasada columna por columna para chequear TCT
	//de haber errores, devuelvo las columnas
	columnasInvalidas = chequearTCT(cantErrores);

	//si hubo algun tipo de error con la imagen, lo logueo y genero matriz de terreno aleatorio.
	if(cantErrores > 0){

		loguearErroresMatriz(columnasInvalidas);
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

	pixel tierra;
	pixel cielo;
	tierra.R=0x80;
	tierra.G=0x40;
	tierra.B=0x00;
	tierra.A=0xFF;
	cielo.R=0x00;
	cielo.G=0x00;
	cielo.B=0x00;
	cielo.A=0x00;

	for(int i=0; i< this->altoMatriz; i++){
		for(int j=0; j<this->anchoMatriz; j++){

			matrizTerreno[j][i] = (f[j] < i)? tierra : cielo;
		}
	}
}

bool LectorTerreno::sonIgualesAlpha(pixel p1, pixel p2){
	return (p1.A == p2.A);
}

vector<int> LectorTerreno::chequearTCT(int &numErrores){

	int numeroCambios;
	vector<int> columnas;
	pixel pixelActual;
	pixel pixelAnterior;

	for(int i=0; i<this->anchoMatriz; i++){
		pixelActual = matrizTerreno[i][0];
		numeroCambios = 0;
		//por cada columna, si detecto mas de un cambio de pixeles se informa del error
		for(int j=1; j<this->altoMatriz; j++){
			pixelAnterior = pixelActual;
			pixelActual = matrizTerreno[i][j];
			if(!sonIgualesAlpha(pixelActual, pixelAnterior)) numeroCambios++;
		}

		if(numeroCambios > 1){
			numErrores++;
			columnas.push_back(i);
		}
	}
	return columnas;
}

Uint32 LectorTerreno::pixelToUint32(pixel p, SDL_PixelFormat* format){
	return (SDL_MapRGBA(format, p.R, p.G, p.B, p.A));
}

void LectorTerreno::guardarMatrizEnPNG(string nombreArchivo){

	Uint32* vectorPixeles = new Uint32[altoMatriz*anchoMatriz];
	SDL_Surface* surNueva = IMG_Load(propiedadesPNG);

	for(unsigned y = 0; y < altoMatriz; y++)
		for(unsigned x = 0; x < anchoMatriz; x++){
			vectorPixeles[x + (y*anchoMatriz)] = pixelToUint32(matrizTerreno[x][y], surNueva->format);
		}

	surNueva->h = altoMatriz;
	surNueva->w = anchoMatriz;
	surNueva->pitch = anchoMatriz*4;	//pitch: tamaño en bits de cada linea (ancho x bytes por pixel)
	surNueva->pixels = vectorPixeles;
	IMG_SavePNG(surNueva, nombreArchivo.c_str());

	delete[] vectorPixeles;
}

int LectorTerreno::getTamanoBorde(){

	int num = 0;
	for(int i=0; i<this->anchoMatriz; i++) if(matrizTerreno[i][altoMatriz].A == 0xFF) num++;

	return num;
}


void LectorTerreno::escalarMatrizAEscenario(){

	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	int altoEscenario = e->altoPx;
	int anchoEscenario = e->anchoPx;
	double escalaX = (double)anchoEscenario/(double)this->anchoMatriz;
	double escalaY = (double)altoEscenario/(double)this->altoMatriz;	

	//reservo espacio para matriz escalada
	pixel** matrizEscalada;
	matrizEscalada = new pixel* [anchoEscenario];
	for(int i = 0; i < anchoEscenario; i++){
		matrizEscalada[i] = new pixel[altoEscenario];
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

	this->guardarMatrizEnPNG(texturaTerreno);
	return texturaTerreno;
}

void LectorTerreno::loguearErroresMatriz(vector<int> columnas){

	if(columnas.size() > 0){
		logError->escribir("Error en el lector de mascara - Se encontraron " + to_string((long long)columnas.size()) + " columnas invalidas.");
		if(columnas.size() <= maxColumnasInvalidasMascara){
			//loguear posicion de cada columna invalida
			for(int i=0; i< columnas.size(); i++)
				logError->escribir("		columna: " + to_string((long long)columnas.at(i)) + ".");
		}
	}

	logError->escribir("Se generará un terreno aleatorio.");
}

void LectorTerreno::generarTerrenoAleatorio(string nombreArchivo){

	generarMatrizAleatoria();
	guardarMatrizEnPNG(nombreArchivo);
}

bool LectorTerreno::esTierra(pixel p){
	return(p.A == 0xFF)? true:false;
}

pixel** LectorTerreno::getMatrizTerreno(){
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
