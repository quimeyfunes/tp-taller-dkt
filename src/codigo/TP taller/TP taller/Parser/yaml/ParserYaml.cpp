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

		parser.GetNextDocument(doc);
		for(unsigned i=0;i<doc.size();i++) {
			//doc[i];
			int altopx;
			doc[0]["alto_px"] >> altopx;
		}
		archivo.close();
	} catch(YAML::ParserException& e) {
		std::cout << e.what() << "\n";
	}
}