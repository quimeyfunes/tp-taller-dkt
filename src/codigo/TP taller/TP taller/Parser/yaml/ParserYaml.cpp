#include "ParserYaml.h"

ParserYaml* ParserYaml::parserInstancia = NULL;
EscenarioParseado* ParserYaml::escenario = NULL;
vector<ObjetoParseado>* ParserYaml::objetos = NULL;



ParserYaml::ParserYaml(string nombreArchivo){
	this->nombreArchivo = nombreArchivo;
}

ParserYaml::ParserYaml(){
}


ParserYaml* ParserYaml::getParser(){
	if(parserInstancia == NULL) {
		parserInstancia = new ParserYaml(configPath);
		parserInstancia->parsear();
	}
	return parserInstancia;
}

void ParserYaml::parsear()
{
	try{
		std::ifstream archivo(this->nombreArchivo,ios::out | ios::in);
		if(!archivo.is_open()){
			string mensaje = "El archivo config no se encuentra o es incorrecto. Se crea un config default.";
			Logger::getLogger()->escribir(mensaje);
			this->nombreArchivo = this->crearConfigDefault();
			this->parsear();
			return;
		}
		
		YAML::Parser parser(archivo);
		

		parser.GetNextDocument(this->documento);
		
		archivo.close();
	} catch(YAML::ParserException& e) {
		
		Logger::getLogger()->escribir(e.what());
		this->nombreArchivo = this->crearConfigDefault();
		this->parsear();
		
	}
	
	Logger::getLogger()->guardarEstado();
}

int ParserYaml::validarMayorA(int valor, int limite){
	
	if(valor<limite){
		Logger::getLogger()->escribir("Error en parseo del yaml - No se cumple una resolucion minima, se establecera resolucion por defecto.");
		return limite;
	}

	return valor;
}

EscenarioParseado* ParserYaml::parsearEscenario(){
	const YAML::Node *nodoEscenario = this->documento.FindValue("escenario");
	EscenarioParseado* esc = new EscenarioParseado();
	if(nodoEscenario) {
		esc->altoPx = this->getValorEscalar(*nodoEscenario,"altopx",altoPxDEF);
		esc->altoPx = validarMayorA(esc->altoPx, altoPXMIN);
		esc->anchoPx = this->getValorEscalar(*nodoEscenario,"anchopx",altoPxDEF);
		esc->anchoPx = validarMayorA(esc->anchoPx, anchoPXMIN);
		esc->altoU = this->getValorEscalar(*nodoEscenario,"altoun",altoUDEF);
		esc->altoU = validarMayorA(esc->altoU, altoUMIN);
		esc->anchoU = this->getValorEscalar(*nodoEscenario,"anchoun",altoUDEF);
		esc->anchoU = validarMayorA(esc->anchoU, anchoUMIN);
		esc->nivelAgua = esc->altoU - this->getValorEscalar(*nodoEscenario,"nivel_agua",nivelAguaDEF);
		esc->imagenTierra = this->getValorCadena(*nodoEscenario,"imagen_tierra",mascaraTerrenoDEF);
		esc->imagenCielo = this->getValorCadena(*nodoEscenario,"imagen_cielo",texturaCieloDEF);
		//this->validarSecuencia(*nodoEscenario,"objetos");
		if(esc->altoU > esc->altoPx){
			Logger::getLogger()->escribir("Error en parseo del yaml - el alto en unidades no puede ser mayor al alto en pixeles, se setearán con el mismo valor.");
			esc->altoU = esc->altoPx;
		}
		if(esc->anchoU > esc->anchoPx){
			Logger::getLogger()->escribir("Error en parseo del yaml - el ancho en unidades no puede ser mayor al ancho en pixeles, se setearán con el mismo valor.");
			esc->anchoU = esc->anchoPx;
		}

		return esc;
	} else {
		Logger::getLogger()->escribir("Error en parseo del yaml - No se encuentra el nodo del escenario. Se utiliza un escenario default.");
		return this->getEscenarioDefault();
	}
	Logger::getLogger()->guardarEstado();
}

EscenarioParseado* ParserYaml::getEscenario(){
	//Si ya lo parsee no vuelvo a parsearlo
	if(escenario == NULL){
		escenario = ParserYaml::getParser()->parsearEscenario();
	}
	return escenario;
}

int ParserYaml::getValorEscalar(const YAML::Node & nodo, string clave, const int valorPorDefecto){
	int valor;
	if(this->validarEscalar(nodo,clave,valor)){
		return valor;
	}
	return valorPorDefecto;
}

float ParserYaml::getValorFloat(const YAML::Node & nodo, string clave, const float valorPorDefecto){
	float valor;
	if(this->validarFloat(nodo,clave,valor)){
		return valor;
	}
	return valorPorDefecto;
}

string ParserYaml::getValorCadena(const YAML::Node & nodo, string clave, string valorPorDefecto){
	string valor;
	if(this->validarCadena(nodo,clave,valor)){
		return valor;
	}
	return valorPorDefecto;
}

bool ParserYaml::getValorBool(const YAML::Node & nodo, string clave, bool valorPorDefecto){
	string valor;
	//El bool viene como un string: si o no
	if(this->validarCadena(nodo,clave,valor)){
		if(valor == "si"){
			return true;
		}else if(valor == "no"){
			return false;
		}else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": se esperaba un valor booleano(si o no). Se toma valor por defecto.";
			Logger::getLogger()->escribir(message);
		}
	}
	return valorPorDefecto;
}

string ParserYaml::getValorColor(const YAML::Node & nodo, string clave, string valorPorDefecto){
	string valor;
	if(this->validarCadena(nodo,clave,valor)){
		//Primero tengo que validar que el color sea un string. Si lo es, valido que sean caracteres 0-1 y A-F
		if( !(valor.length() > 6)){
			for(int i=1; i<valor.length();i++){
				if(!isxdigit(valor[i])){
					//Si alguno de los caracteres no pertenece a los hexa devuelvo el valor por defecto
					std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": se esperaba un valor Hexa(valores 0-9 y a-f). Se toma valor por defecto.";
					Logger::getLogger()->escribir(message);
					return valorPorDefecto;
				}
			}
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": se esperaba un valor Hexa, el largo no puede ser mayor a 7. Se toma valor por defecto.";
			Logger::getLogger()->escribir(message);
			return valorPorDefecto;
		}
		
	}else{
		return valorPorDefecto;
	}
	return valor;
}

int ParserYaml::getValorTipoObjeto(const YAML::Node & nodo, string clave, int valorPorDefecto){
	string valor;
	//Tengo que validar que el tipo sea un string
	if(this->validarCadena(nodo,clave,valor)){
		if(valor ==  circuloString){
			return circuloTipo;
		}
		else if(valor ==  rectanguloString){
			return rectanguloTipo;
		}
		else if(valor ==  trianguloString){
			return trianguloTipo;
		}
		else if(valor ==  cuadradoString){
			return cuadradoTipo;
		}
		else if(valor ==  pentagonoString){
			return pentagonoTipo;
		}
		else if(valor ==  hexagonoString){
			return hexagonoTipo;
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": el tipo no es correcto. Se toma valor por defecto.";
			Logger::getLogger()->escribir(message);
		}
	}
	return valorPorDefecto;
}

vector<ObjetoParseado>* ParserYaml::getValorSecuencia(const YAML::Node & nodo, string clave){
	vector<ObjetoParseado>* valor = new vector<ObjetoParseado>();
	const YAML::Node *nodo_aux;
	//Obtengo el nodo que tiene los objectos
	nodo_aux = nodo.FindValue(clave);
	//Tengo que recorrer la secuencia
	for(unsigned int i = 0; i < nodo_aux->size(); i++){
		//Cada elemento es un nodo. Lo parseo y lo agrego al vector
		ObjetoParseado objeto = this->parsearObjeto((*nodo_aux)[i]);
		valor->push_back(objeto);
	}
	return valor;
}

bool ParserYaml::validarEscalar(const YAML::Node & nodo, string clave, int &valor){
	const YAML::Node *nodo_aux;
	std::string str;
	if(nodo_aux = nodo.FindValue(clave)) {
		if ((*nodo_aux).Type() == YAML::NodeType::Scalar ){
			*nodo_aux >> str;
			if (esEntero(str)){
				int value_aux;
				*nodo_aux >> value_aux;
				if(value_aux < 0){
					//value_aux = value_aux * (-1);
					std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": se esperaba un valor de la clave " + clave + " no es positivo.";
					Logger::getLogger()->escribir(message);
					return false;
				}
				valor = value_aux;
				return true;
			}
			else{
				std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave " + clave + " no es numerico.";
				Logger::getLogger()->escribir(message);
			}
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave " + clave + " no es Scalar.";
			Logger::getLogger()->escribir(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		Logger::getLogger()->escribir(message);
	}
	return false;
}


bool ParserYaml::validarFloat(const YAML::Node & nodo, string clave, float &valor){
	const YAML::Node *nodo_aux;
	std::string str;
	if(nodo_aux = nodo.FindValue(clave)) {
		if ((*nodo_aux).Type() == YAML::NodeType::Scalar ){
			*nodo_aux >> str;
			if (esFloat(str)){
				float value_aux;
				*nodo_aux >> value_aux;
				if(value_aux < 0){
					//value_aux = value_aux * (-1);
					std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": se esperaba un valor de la clave " + clave + " no es positivo.";
					Logger::getLogger()->escribir(message);
					return false;
				}
				valor = value_aux;
				return true;
			}
			else{
				std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave " + clave + " no es un float correcto.";
				Logger::getLogger()->escribir(message);
			}
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave " + clave + " no es Scalar.";
			Logger::getLogger()->escribir(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		Logger::getLogger()->escribir(message);
	}
	return false;
}

string ParserYaml::getNodoInfo(const YAML::Node & nodo){
	YAML::Mark mark = nodo.GetMark();
	std::stringstream info;
	info << "Linea " << mark.line + 1 << ", Columna " << mark.pos + 1;
	return info.str();
}

bool ParserYaml::esEntero(const std::string& s){
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

bool ParserYaml::esFloat(const std::string &number) {
    std::istringstream iss(number);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail(); 
}

//para validar los elementos de las secuencias lo deberia hacer el metodo invocante, luego de validar q sea una secuencia.
bool ParserYaml::validarSecuencia(const YAML::Node &nodo, string clave){
	const YAML::Node *nodo_aux;
	if(nodo_aux = nodo.FindValue(clave)){
		if((*nodo_aux).Type() == YAML::NodeType::Sequence){
			return true;
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave " + clave + " no es una secuencia.";
			Logger::getLogger()->escribir(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		Logger::getLogger()->escribir(message);
	}
return false;
}


//valida que la cadena almacenada sea la correcta.
bool ParserYaml::validarCadena(const YAML::Node &nodo, string clave, string &cadenaValida){
	const YAML::Node *nodo_aux;
	if(nodo_aux = nodo.FindValue(clave)){
		string cadena_aux;
		*nodo_aux >> cadena_aux;
		if(cadena_aux != ""){
			*nodo_aux >> cadenaValida;
			return true;
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave " + clave + " no es correcto.";
			Logger::getLogger()->escribir(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		Logger::getLogger()->escribir(message);
	}
return false;
}


bool ParserYaml::validarImagen(string path){
	std::ifstream archivo(path,ios::out | ios::in);
	return archivo.is_open();
}

EscenarioParseado* ParserYaml::getEscenarioDefault(){
	EscenarioParseado* esc = new EscenarioParseado();
	esc->altoPx = altoPxDEF;
	esc->anchoPx = altoPxDEF;
	esc->altoU = altoUDEF;
	esc->anchoU = altoUDEF;
	esc->nivelAgua = altoUDEF - nivelAguaDEF;
	esc->imagenTierra = mascaraTerrenoDEF;
	esc->imagenCielo = texturaCieloDEF;
	return esc;
}


vector<ObjetoParseado>* ParserYaml::parsearObjetos(){
	const YAML::Node *nodoEscenario = this->documento.FindValue("escenario");
	if(nodoEscenario){
		if(this->validarSecuencia(*nodoEscenario,"objetos")){
			//Si es una secuencia, obtengo los objetos
			return this->getValorSecuencia(*nodoEscenario,"objetos");
		}
		//Si no hay nodo objetis tengo que devolver un vector de objetos default
	}else{
		//Si no hay nodo escenario tengo que devolver un vector de objetos default
		Logger::getLogger()->escribir("Error en parseo del yaml - No se encuentra el nodo del escenario. Se utilizan figuras default.");
	}
	Logger::getLogger()->guardarEstado();
}

vector<ObjetoParseado>* ParserYaml::getObjetos(){

	if(objetos == NULL){
		objetos = ParserYaml::getParser()->parsearObjetos();
	}
	return objetos;
}

vector<ObjetoParseado>* ParserYaml::getObjetosDefault(){
	//Devuelvo un vector de objetos default
	vector<ObjetoParseado>* resultado = new vector<ObjetoParseado>();
	resultado->push_back(this->getObjetoDefault());
	return resultado;
}

ObjetoParseado ParserYaml::getObjetoDefault(){
	ObjetoParseado obj;
	obj.tipo = tipoObjDEF;
	obj.x = getPosRandom(10,90,'x');
	obj.y = this->escenario->altoU - getPosRandom(70,95,'y');
	obj.ancho = anchoObjDEF;
	obj.alto = altoObjDEF;
	obj.escala = escalaDEF;
	obj.rotacion = rotacionDEF;
	obj.masa = masaDEF;
	obj.color = colorDEF;
	obj.estatico = estaticoDEF;
	return obj;
}

ObjetoParseado ParserYaml::parsearObjeto(const YAML::Node &nodo){
	ObjetoParseado obj;
	obj.tipo = this->getValorTipoObjeto(nodo,"tipo",tipoObjDEF);
	obj.x = this->getValorEscalar(nodo,"x",this->getPosRandom(10,90,'x'));
	obj.y = this->escenario->altoU - this->getValorEscalar(nodo,"y",this->getPosRandom(70,95,'y'));
	obj.ancho = this->getValorEscalar(nodo,"ancho",anchoObjDEF);
	obj.alto = this->getValorEscalar(nodo,"alto",altoObjDEF);
	obj.rotacion = this->getValorEscalar(nodo,"rot",rotacionDEF);
	obj.masa = this->getValorEscalar(nodo,"masa",masaDEF);
	obj.color = this->getValorColor(nodo,"color",colorDEF);
	obj.estatico = this->getValorBool(nodo,"estatico",estaticoDEF);
	if(obj.tipo != rectanguloTipo){
		obj.escala = this->getValorFloat(nodo,"escala",escalaDEF);
	}
	obj.linea = nodo.GetMark().line + 1;
	//Chequeo si el objeto esta dentro del escenario
	EscenarioParseado* es = this->getEscenario();
	if(obj.x < 0 || obj.x > es->anchoU || obj.y < 0 || obj.y > es->altoU){
		std::stringstream info;
		info << obj.linea;
		Logger::getLogger()->escribir("Error en parseo del yaml - El objeto de la linea "+info.str()+" no se encuentra dentro del escenario. Se utiliza una figura default.");
		return this->getObjetoDefault();
	}
	return obj;
}

string ParserYaml::crearConfigDefault(){
	YAML::Emitter out;
	EscenarioParseado* es = this->getEscenarioDefault();
	vector<ObjetoParseado>* objetos = this->getObjetosDefault();
	out << YAML::BeginMap;
	out << YAML::Key << "escenario";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "altopx";
		out << YAML::Value << es->altoPx;
		out << YAML::Key << "anchopx";
		out << YAML::Value << es->anchoPx;
		out << YAML::Key << "altou";
		out << YAML::Value << es->altoU;
		out << YAML::Key << "anchou";
		out << YAML::Value << es->anchoU;
		out << YAML::Key << "nivel_agua";
		out << YAML::Value << es->nivelAgua;
		out << YAML::Key << "imagen_tierra";
		out << YAML::Value << es->imagenTierra;
		out << YAML::Key << "imagen_cielo";
		out << YAML::Value << es->imagenCielo;
		out << YAML::Key << "objetos";
		out << YAML::Value << YAML::BeginSeq;
		for(std::vector<ObjetoParseado>::iterator it = objetos->begin(); it != objetos->end(); ++it) {
			ObjetoParseado obj = *it;
			out << YAML::BeginMap;
				out << YAML::Key << "tipo";
				out << YAML::Value << this->getTipoStringByTipo(obj.tipo);
				out << YAML::Key << "x";
				out << YAML::Value << obj.x;
				out << YAML::Key << "y";
				out << YAML::Value << obj.y;
				out << YAML::Key << "alto";
				out << YAML::Value << obj.alto;
				out << YAML::Key << "ancho";
				out << YAML::Value << obj.ancho;
				out << YAML::Key << "color";
				out << YAML::Value << obj.color;
				if(obj.tipo != rectanguloTipo){
					out << YAML::Key << "escala";
					out << YAML::Value << obj.escala;
				}
				out << YAML::Key << "rot";
				out << YAML::Value << obj.rotacion;
				out << YAML::Key << "masa";
				out << YAML::Value << obj.masa;
				out << YAML::Key << "estatico";
				if(obj.estatico){
					out << YAML::Value << "si";
				}else{
					out << YAML::Value << "no";
				}
				
			out << YAML::EndMap;
		}

		out << YAML::Value << YAML::EndSeq;
			
	out << YAML::EndMap;

	out << YAML::EndMap;
	ofstream myfile("config/configDefault.yaml"); 
	myfile << out.c_str();
	myfile.close();

	return "config/configDefault.yaml";
}


string ParserYaml::getTipoStringByTipo(int tipo){
	if(tipo ==  circuloTipo){
		return circuloString;
	}
	else if(tipo ==  rectanguloTipo){
		return rectanguloString;
	}
	else if(tipo ==  trianguloTipo){
		return trianguloString;
	}
	else if(tipo ==  cuadradoTipo){
		return cuadradoString;
	}
	else if(tipo ==  pentagonoTipo){
		return pentagonoString;
	}
	else if(tipo ==  hexagonoTipo){
		return hexagonoString;
	}
	return rectanguloString;
}


//los porcenjates son para calcular el rango dentro del escenario.
int ParserYaml::getPosRandom(int porcentaje_min, int porcentaje_max, char eje){
	int posMin, posMax, resultado;
	float min = (porcentaje_min+ 0.00) / 100;
	float max = (porcentaje_max+ 0.00) / 100;
	srand(time(NULL));
	Sleep(150+rand()%250);
	EscenarioParseado* es = this->getEscenario();

	if(eje == 'x'){
		posMin = (es->anchoU)*min;
		posMax = (es->anchoU)*max;
		resultado = posMin+rand()%(posMax+1-posMin);
		return resultado;
	}
	else if(eje == 'y'){
		posMin = (es->altoU)*min;
		posMax = (es->altoU)*max;
		resultado = posMin+rand()%(posMax+1-posMin);
		return resultado;
	}
	else{
		Logger::getLogger()->escribir("Error en la funcion ParserYaml::getPosRandom, el eje debe ser 'x' o 'y'...");
		return 50;
	}
}