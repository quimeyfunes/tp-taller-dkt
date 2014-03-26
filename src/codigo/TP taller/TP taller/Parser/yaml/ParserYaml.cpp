#include "ParserYaml.h"

ParserYaml* ParserYaml::parserInstancia = NULL;

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
			string mensaje = "El archivo config no se encuentra o es incorrecto.";
			Logger::getLogger()->escribir(mensaje);
			return;
		}
		YAML::Parser parser(archivo);
		YAML::Node doc;

		parser.GetNextDocument(this->documento);
		
		archivo.close();
	} catch(YAML::ParserException& e) {
		//std::cout << e.what() << "\n";
		Logger::getLogger()->escribir(e.what());
	}
}

Escenario* ParserYaml::getEscenario(){
	const YAML::Node *nodoEscenario = this->documento.FindValue("escenario");
	if(nodoEscenario) {
		//Tengo que instanciar el escenario
		string imagen_tierra,imagen_cielo;
		int altopx,anchopx,altoun,anchou,nivel_agua;
		altopx = this->getValorEscalar(*nodoEscenario,"altopx",altoPxDEF);
		anchopx = this->getValorEscalar(*nodoEscenario,"anchopx",altoPxDEF);
		altoun = this->getValorEscalar(*nodoEscenario,"altoun",altoUDEF);
		anchou = this->getValorEscalar(*nodoEscenario,"anchoun",altoUDEF);
		nivel_agua = this->getValorEscalar(*nodoEscenario,"nivel_agua",nivelAguaDEF);
		imagen_tierra = this->getValorCadena(*nodoEscenario,"imagen_tierra",imagenTerrenoDEF);
		imagen_cielo = this->getValorCadena(*nodoEscenario,"imagen_cielo",imagenCieloDEF);
		//this->validarSecuencia(*nodoEscenario,"objetos");
		return new Escenario(altopx,anchopx,altoun,anchou,nivel_agua,imagen_tierra,imagen_cielo);
	} else {
		Logger::getLogger()->escribir("Error en parseo del yaml - No se encuentra el nodo del escenario. Se utiliza un escenario default.");
		//HAY QUE CARGAR UN ESCENARIO DEFAULT
	}
}

int ParserYaml::getValorEscalar(const YAML::Node & nodo, string clave, const int valorPorDefecto){
	int valor;
	if(this->validarEscalar(nodo,clave,valor)){
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

bool ParserYaml::validarEscalar(const YAML::Node & nodo, string clave, int &valor){
	const YAML::Node *nodo_aux;
	std::string str;
	if(nodo_aux = nodo.FindValue(clave)) {
		if ((*nodo_aux).Type() == YAML::NodeType::Scalar ){
			*nodo_aux >> str;
			if (esNumero(str)){
				int value_aux;
				*nodo_aux >> value_aux;
				if(value_aux < 0){
					//value_aux = value_aux * (-1);
					std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": se esperaba un valor de la clave" + clave + " no es positivo.";
					Logger::getLogger()->escribir(message);
					return false;
				}
				valor = value_aux;
				return true;
			}
			else{
				std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es numerico.";
				Logger::getLogger()->escribir(message);
			}
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es Scalar.";
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
	nodo.GetMark();
	return "TODO:falta obtener la informacion del mark del nodo";
}

bool ParserYaml::esNumero(const std::string& s){
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

//para validar los elementos de las secuencias lo deberia hacer el metodo invocante, luego de validar q sea una secuencia.
bool ParserYaml::validarSecuencia(const YAML::Node &nodo, string clave){
	const YAML::Node *nodo_aux;
	if(nodo_aux = nodo.FindValue(clave)){
		if((*nodo_aux).Type() == YAML::NodeType::Sequence){
			return true;
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es una secuencia.";
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
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es correcto.";
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