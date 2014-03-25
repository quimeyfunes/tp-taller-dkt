#include "ParserYaml.h"

ParserYaml::ParserYaml(string nombreArchivo){
	this->nombreArchivo = nombreArchivo;
}


void ParserYaml::parsear()
{
	try{
		std::ifstream archivo(this->nombreArchivo,ios::out | ios::in);
		if(!archivo.is_open()){
			string mensaje = "Loguear: El archivo no se encuentra o es incorrecto";
			return;
		}
		YAML::Parser parser(archivo);
		YAML::Node doc;

		parser.GetNextDocument(this->documento);
		
		archivo.close();
	} catch(YAML::ParserException& e) {
		std::cout << e.what() << "\n";
		//Loguear error
	}
}

Escenario ParserYaml::getEscenario(){
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
		/*nodo = nodoEscenario->FindValue("imagen_tierra");
		nodo = nodoEscenario->FindValue("imagen_cielo");*/
		return Escenario(altopx,anchopx,altoun,anchou,nivel_agua,"","");
	} else {
		//Loguear
	}
}

int ParserYaml::getValorEscalar(const YAML::Node & nodo, string clave, const int valorPorDefecto){
	int valor;
	if(this->validarEscalar(nodo,clave,valor)){
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
					//Log::getInstance()->WARNING(message);
					return false;
				}
				valor = value_aux;
				return true;
			}
			else{
				std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es numerico.";
				//Log::getInstance()->WARNING(message);
			}
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es Scalar.";
			//Log::getInstance()->WARNING(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		//Log::getInstance()->WARNING(message);
	}
	return false;
}

string ParserYaml::getNodoInfo(const YAML::Node & nodo){
	nodo.GetMark();
	return "TODO";
}

bool ParserYaml::esNumero(const std::string& s){
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

//para validar los elementos de las secuencias lo deberia hacer el metodo invocante, luego de validar q sea una secuencia.
bool ParserYaml::ValidarSecuencia(const YAML::Node &nodo, string clave){
	const YAML::Node *nodo_aux;
	if(nodo_aux = nodo.FindValue(clave)){
		if((*nodo_aux).Type() != YAML::NodeType::Sequence){
			return true;
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es una secuencia.";
			//Log::getInstance()->WARNING(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		//Log::getInstance()->WARNING(message);
	}
return false;
}


//valida que la cadena almacenada sea la correcta.
bool ParserYaml::ValidarCadena(const YAML::Node &nodo, string clave, string cadenaValida){
	const YAML::Node *nodo_aux;
	if(nodo_aux = nodo.FindValue(clave)){
		if( (*nodo_aux).to<std::string>() == cadenaValida){
			return true;
		}
		else{
			std::string message = "Error en parseo del yaml - " + this->getNodoInfo(*nodo_aux) + ": el valor de la clave" + clave + " no es la cadena esperada.";
			//Log::getInstance()->WARNING(message);
		}
	}
	else{
		std::string message = "Error en parseo del yaml - " + this->getNodoInfo(nodo) + ": no se encontro la clave " + clave + ".";
		//Log::getInstance()->WARNING(message);
	}
return false;
}
