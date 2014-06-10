#include "escenario.h"

Escenario::Escenario(){
}

Escenario::Escenario(int altoU,int anchoU,int nivelAgua, float relacionAncho, float relacionAlto, int maximosClientes){
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->listaFiguras = new list<Figura*>();
	this->listaArmas = new list<Arma*>();
	this->maximosClientes = maximosClientes;

	b2Vec2* gravity = new b2Vec2(gravedadX, gravedadY);
	this->world = new b2World(*gravity);

	this->gusanoActivo = NULL;
	this->bloquearTeclas = false;

	this->puedeMoverseArriba = false;
	this->puedeMoverseDerecha = false;
	this->puedeMoverseIzquierda = false;
	this->puedeDisparar = false;
	this->puedeMoverseAbajo = false;
	this->puedeSaltar = false;

	for(int i=0; i < this->maximosClientes; i++){
		this->figurasActivas.push_back(NULL);
		this->puedeMoverseArribaClientes.push_back(false);
		this->puedeMoverseDerechaClientes.push_back(false);
		this->puedeMoverseIzquierdaClientes.push_back(false);
		this->puedeSaltarClientes.push_back(false);
		this->puedeDispararClientes.push_back(false);
		this->puedeMoverseAbajoClientes.push_back(false);
		this->huboDisparo = false;
	}
}

void Escenario::inicializarCliente(int cliente){
	/*this->figurasActivas[cliente] = NULL;
	this->puedeMoverseArribaClientes[cliente] = false;
	this->puedeMoverseDerechaClientes[cliente] = false;
	this->puedeMoverseIzquierdaClientes[cliente] = false;
	this->puedeSaltarClientes[cliente] = false;*/
}

int Escenario::getAltoU(){
	return altoU;
}

int Escenario::getAnchoU(){
	return anchoU;
}

int Escenario::getNivelAgua(){
	return nivelAgua;
}

int Escenario::getMaximosClientes(){
	return maximosClientes;
}

list<Figura*>* Escenario::getFiguras() {
	return this->listaFiguras;
}

b2World* Escenario::getWorld() {
	return this->world;
}

void Escenario::agregarFigura(Figura* figura) {
	this->listaFiguras->push_back(figura);
}

void Escenario::agregarArma(Arma* arma){
	this->listaArmas->push_back(arma);
}

void Escenario::notificar() {
	
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->notificar();
	}

	for (list<Arma*>::iterator it = this->listaArmas->begin(); it != this->listaArmas->end(); it++) {
		(*it)->actualizar(this->anchoU,this->altoU);
		(*it)->notificar();
	}

	this->moverse();
	this->moverseClientes();
}

Gusano* Escenario::crearGusano(ObjetoParseado objeto){
	Gusano* gusano = new Gusano(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,anchoGusano,altoGusano,objeto.masa,this->maximosClientes);
	if (this->haySuperposicion(gusano)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(gusano->getBody());
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else if(this->haySuperposicionConTerreno(gusano)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(gusano->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else {
		this->agregarFigura(gusano);
		return gusano;
	}
}

Gusano* Escenario::crearGusanoParaJugador(){
	//La posiciones tiene que ser sobre el terreno, aleatoria
	int verticesCount = terreno->getBody()->GetFixtureList()->GetShape()->GetChildCount();
	b2ChainShape* chain = (b2ChainShape*)terreno->getBody()->GetFixtureList()->GetShape();
	do {
		int index = rand()%verticesCount;
		b2Vec2 vec = chain->m_vertices[index];
		int x = vec.x;
		int y = vec.y - altoGusano;
	
		Gusano* gusano = new Gusano(x,y,0,this->world,false,anchoGusano,altoGusano,masaGusano,this->maximosClientes);
		if (this->haySuperposicion(gusano) || this->haySuperposicionConTerreno(gusano) ||  vec.y + altoGusano > this->nivelAgua){
			//Si hay superposicion o esta al nivel del agua creo en otra posicion;
			this->world->DestroyBody(gusano->getBody());
			delete gusano;
		}
		else {
			this->agregarFigura(gusano);
			return gusano;
		}
	} while (true);
}

Poligono* Escenario::crearPoligono(ObjetoParseado objeto){
	Poligono* poligono = new Poligono(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.escala,objeto.masa,objeto.tipo);
	if (this->haySuperposicion(poligono)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(poligono->getBody());
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else if (this->haySuperposicionConTerreno(poligono)) {
		//Remuevo figura del world
		this->getWorld()->DestroyBody(poligono->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else {
		this->agregarFigura(poligono);
		return poligono;
	}
}

Circulo* Escenario::crearCirculo(ObjetoParseado objeto){
	Circulo* circulo = new Circulo(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.escala,objeto.masa);
	if (this->haySuperposicion(circulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(circulo->getBody());
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else if (this->haySuperposicionConTerreno(circulo)) {
		//Remuevo figura del world
		this->getWorld()->DestroyBody(circulo->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else {
		this->agregarFigura(circulo);
		return circulo;
	}
}

Rectangulo* Escenario::crearRectangulo(ObjetoParseado objeto){
	Rectangulo* rectangulo = new Rectangulo(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.ancho,objeto.alto,objeto.masa);
	if (this->haySuperposicion(rectangulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(rectangulo->getBody());
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else if(this->haySuperposicionConTerreno(rectangulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(rectangulo->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else {
		this->agregarFigura(rectangulo);
		return rectangulo;
	}
}

void Escenario::simularAgua () {
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->simularAgua(this->nivelAgua);
	}
		for (list<Arma*>::iterator it = this->listaArmas->begin(); it != this->listaArmas->end(); it++) {
		(*it)->simularAgua(this->nivelAgua);
	}
}

void Escenario::reiniciar(){
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->reiniciar();
	}
}

void Escenario::reiniciarTeclas(){
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->quieto();
	}
}

bool Escenario::haySuperposicion(Figura* figura){
	bool chocan = false;
	for (b2Fixture* fixture = figura->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		for (std::list<Figura*>::const_iterator iterator = this->listaFiguras->begin(); iterator != this->listaFiguras->end(); ++iterator) {
			Figura* figuraActual = *iterator;
			for (b2Fixture* fixtureActual = figuraActual->getBody()->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()) {
				chocan = b2TestOverlap(fixture->GetShape(),0,fixtureActual->GetShape(),0,figura->getBody()->GetTransform(),figuraActual->getBody()->GetTransform());
				if(chocan){
					break;
				}
			}
		}
	}

	return chocan;
}

bool Escenario::haySuperposicionConTerreno(Figura* figura){
	//Primero chequeo si la figura se superpone con la cadena
	Terreno* terreno = this->getTerreno();
	b2ChainShape* shapeTerreno = (b2ChainShape*) terreno->getBody()->GetFixtureList()->GetShape();
	for (b2Fixture* fixture = figura->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		for (int i = 0; i < shapeTerreno->GetChildCount();i++) {
			bool chocan = b2TestOverlap(fixture->GetShape(),0,shapeTerreno,i,figura->getBody()->GetTransform(),terreno->getBody()->GetTransform());
			if(chocan){
				return true;
			}
		}
	}
	//Si no choca con los bordes del terreno tengo que chequear con la matriz
	pixel** matrizTerreno = terreno->getLectorTerreno()->getMatrizTerreno();
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	b2Vec2 posicion = figura->getBody()->GetPosition();
	float x = posicion.x * e->anchoPx / this->anchoU;
	float y = posicion.y * e->altoPx / this->altoU;
	//Si x coincide con el ancho de la matriz, le resto uno para que no se vaya de rango
	if (x == terreno->getLectorTerreno()->getAnchoMatriz()) x--;
	//Si y coincide con el alto de la matriz, le resto uno para que no se vaya de rango
	if (y == terreno->getLectorTerreno()->getAltoMatriz()) y--;
	if(y >= terreno->getLectorTerreno()->getAltoMatriz() || x >= terreno->getLectorTerreno()->getAnchoMatriz() ||  y < 0 || x < 0){
		return false;
	}
	if(terreno->getLectorTerreno()->esTierra(matrizTerreno[(int)floor(x)][(int)floor(y)])){
		//Como hay un 1 quiere decir que el centro esta dentro del terreno	
		return true;
	}
	return false;
}

Terreno* Escenario::getTerreno(){
	return this->terreno;
}

void Escenario::setTerreno(Terreno* terreno){
	this->terreno = terreno;
}

std::stringstream Escenario::getMensajeSuperposicionObjeto(int linea){
	std::stringstream info;
	if(linea > 0){
		info << "Error al agregar figura: la figura de la linea " << linea << " se superpone con una agregada con anterioridad.";
	}else{
		info << "Error al agregar figura: la figura default se superpone con una agregada con anterioridad.";
	}

	return info;
}

std::stringstream Escenario::getMensajeSuperposicionTerreno(int linea){
	std::stringstream info;
	if(linea > 0){
		info << "Error al agregar figura: la figura de la linea " << linea << " se superpone con el terreno.";
	}else{
		info << "Error al agregar figura: la figura default se superpone con el terreno.";
	}

	return info;
}

bool Escenario::click(float x, float y){
	for (std::list<Figura*>::const_iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		if ((*it)->meClickeo(x,y) && !(*it)->estaMuerto()) {
			if (this->gusanoActivo != NULL) {
				this->gusanoActivo->setActivo(false);
			}
			this->gusanoActivo = (Gusano*)(*it);
			this->gusanoActivo->setActivo(true);
			return true;
		}
	}
	//this->terreno->destruirTerreno(x,y,5);
	//this->explotar(x,y,5);
	return false;
}

void Escenario::clickCliente(int cliente, list<Gusano*> figurasCliente, list<Gusano*> figurasOtrosCliente,float x, float y){
	//Recorro solo las figuras del cliente para saber si tengo que seleccionar una figura como activa
	for (std::list<Gusano*>::const_iterator it = figurasCliente.begin(); it != figurasCliente.end(); it++) {
		if ((*it)->meClickeo(x,y) && !(*it)->estaMuerto()) {
			//printf("cliente %d clickeo uno de sus gusanos.\n",cliente);
			//this->figurasActivas[cliente] = (*it);
			/*if (this->figurasActivas[cliente] != NULL) {
				this->figurasActivas[cliente]->setMeClickearon(false,cliente);
			}*/
			this->gusanoActivo = (Gusano*)(*it);
			this->gusanoActivo->setMeClickearon(true,cliente);
		}
	}

	//Recorro el resto de las figuras
	for (std::list<Gusano*>::const_iterator it = figurasOtrosCliente.begin(); it != figurasOtrosCliente.end(); it++) {
		if ((*it)->meClickeo(x,y) && !(*it)->estaMuerto()) {
			(*it)->setMeClickearon(true,cliente);
			if (this->figurasActivas[cliente])
				this->figurasActivas[cliente]->setMeClickearon(false,cliente);
			this->figurasActivas[cliente] = NULL;
		}else{
			(*it)->setMeClickearon(false,cliente);
		}
	}
}

void Escenario::enter(bool enter){
	this->puedeSaltar = enter;
}

void Escenario::enterCliente(int cliente, bool enter){
	this->puedeSaltarClientes[cliente] = enter;
}

void Escenario::arriba(bool arriba){
	this->puedeMoverseArriba = arriba;
}

void Escenario::abajo(bool abajo){
	this->puedeMoverseAbajo = abajo;
}

void Escenario::espacio(bool esp){
		this->puedeDisparar = esp;
}

void Escenario::espacioCliente(int cliente, bool esp){
		this->puedeDispararClientes[cliente] = esp;
}

void Escenario::arribaCliente(int cliente ,bool arriba){
	this->puedeMoverseArribaClientes[cliente] = arriba;
}

void Escenario::abajoCliente(int cliente ,bool arriba){
	this->puedeMoverseAbajoClientes[cliente] = arriba;
}

void Escenario::setPuedeSaltarCliente(int cliente,bool puedeSaltar) {
	this->puedeSaltarClientes[cliente] = puedeSaltar;
}

void Escenario::izquierda(bool izquierda){
	this->puedeMoverseIzquierda = izquierda;
}

void Escenario::disparar(bool disparo){
	//this->puedeDisparar = disparo;
}

void Escenario::izquierdaCliente(int cliente,bool izquierda){
	this->puedeMoverseIzquierdaClientes[cliente] = izquierda;
}

void Escenario::derecha(bool derecha){
	this->puedeMoverseDerecha = derecha;
}

void Escenario::derechaCliente(int cliente,bool derecha){
	this->puedeMoverseDerechaClientes[cliente] = derecha;
}

void Escenario::moverse(){
	if (!this->bloquearTeclas) {
		if ((this->gusanoActivo != NULL) && !(this->gusanoActivo->estaMuerto())) {
			this->saltar();
			this->bajar();
			this->subir();
			this->moverDerecha();
			this->moverIzquierda();
			this->cargarDisparo();
			this->gusanoActivo->chequearChoqueConArma();
		}
	}
}

void Escenario::cargarDisparo(){
	if((puedeDisparar)&&(this->gusanoActivo != NULL)&&(this->gusanoActivo->armaActual.armaTipo != NINGUNA)){ 
		if(this->gusanoActivo->armaActual.puedeCargarse){
			if(this->gusanoActivo->armaActual.potenciaDisparo == 0){
				Reproductor::getReproductor()->reproducirSonido(CARGANDODISPARO);
				if(this->gusanoActivo->armaActual.armaTipo == BAZOOKA){
					Reproductor::getReproductor()->reproducirSonido(ENEMIGOALAS12);
				}
			}
			if(this->gusanoActivo->armaActual.potenciaDisparo < POTENCIA_MAXIMA_DISPARO){
				this->gusanoActivo->armaActual.potenciaDisparo += AUMENTO_POTENCIA;
			}
		}
		//cout<<this->getGusanoActivo()->armaActual.potenciaDisparo<<endl;
	}else{
		this->gusanoActivo->armaActual.potenciaDisparo = 0;
	}


}

void Escenario::cargarDisparoClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && (this->puedeDispararClientes[i]) && (this->figurasActivas[i]->armaActual.armaTipo != NINGUNA) &&!(this->figurasActivas[i]->estaMuerto())) {
			if(this->figurasActivas[i]->armaActual.puedeCargarse){
				if(this->figurasActivas[i]->armaActual.potenciaDisparo == 0){
					Reproductor::getReproductor()->reproducirSonido(CARGANDODISPARO);
					if(this->figurasActivas[i]->armaActual.armaTipo == BAZOOKA){
						Reproductor::getReproductor()->reproducirSonido(ENEMIGOALAS12);
					}
				}
				if(this->figurasActivas[i]->armaActual.potenciaDisparo < POTENCIA_MAXIMA_DISPARO){
					this->figurasActivas[i]->armaActual.potenciaDisparo += AUMENTO_POTENCIA;
				}
			}
		}else{
			this->figurasActivas[i]->armaActual.potenciaDisparo = 0;
		}
	}
}

void Escenario::moverseClientes(){
	/*for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && !(this->figurasActivas[i]->estaMuerto()) && !(this->figurasActivas[i]->estaMuerto())) {
			this->saltarClientes();
			this->bajarClientes();
			this->subirClientes();
			this->moverDerechaClientes();
			this->moverIzquierdaClientes();
			this->cargarDisparoClientes();
		}
	}*/
}

void Escenario::subir(){
	if(this->puedeMoverseArriba){
		if(this->gusanoActivo->tieneUnArma()){
			if(this->gusanoActivo->armaActual.anguloDisparo < 90)
				this->gusanoActivo->armaActual.anguloDisparo+=1.0f;
		}
	}
}

void Escenario::saltar(){
	if (this->puedeSaltar) {
		if (this->gusanoActivo->puedeSaltar()) {
			b2Body* cuerpo = this->gusanoActivo->getBody();
			if(!Reproductor::getReproductor()->estaReproduciendo(SALTO))
				Reproductor::getReproductor()->reproducirSonido(SALTO);
			cuerpo->SetLinearVelocity(b2Vec2(0,saltoGusano));
		}
		//cuerpo->ApplyLinearImpulse(b2Vec2(0,-100),this->figuraActiva->getPosicion(),true);
	}
}

void Escenario::bajar(){
	if(this->puedeMoverseAbajo && (!this->gusanoActivo->estaMuerto()) && (this->gusanoActivo->tieneUnArma())){
			if(this->gusanoActivo->armaActual.anguloDisparo > -90)
				this->gusanoActivo->armaActual.anguloDisparo-=1.0f;
	}
}

void Escenario::bajarClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && (this->puedeMoverseAbajoClientes[i]) && !(this->figurasActivas[i]->estaMuerto()) && (this->figurasActivas[i]->tieneUnArma())) {
				if(this->figurasActivas[i]->armaActual.anguloDisparo > -90)
				this->figurasActivas[i]->armaActual.anguloDisparo-=1.0f;
		}
	}
}


void Escenario::subirClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && (this->figurasActivas[i]->tieneUnArma())) {
				if(this->figurasActivas[i]->armaActual.anguloDisparo < 90)
					this->figurasActivas[i]->armaActual.anguloDisparo+=1.0f;
		}
	}
}

void Escenario::saltarClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && ((Gusano*)this->figurasActivas[i])->puedeSaltar() && !(this->figurasActivas[i]->estaMuerto())) {
			b2Body* cuerpo = this->figurasActivas[i]->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(0,saltoGusano));
		}
	}
}

void Escenario::moverIzquierda(){
	if (this->puedeMoverseIzquierda && !(this->gusanoActivo->estaMuerto())) {
		b2Body* cuerpo = this->gusanoActivo->getBody();
		cuerpo->SetLinearVelocity(b2Vec2(-velocidadGusano,cuerpo->GetLinearVelocity().y));
		this->gusanoActivo->setMovimientoIzq(true);
		this->gusanoActivo->setMovimientoDer(false);
		this->gusanoActivo->armaActual.sentidoDisparo = true;
	}
}

void Escenario::moverIzquierdaClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) &&  (this->puedeMoverseIzquierdaClientes[i]) && !(this->figurasActivas[i]->estaMuerto())) {
			b2Body* cuerpo = this->figurasActivas[i]->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(-velocidadGusano,cuerpo->GetLinearVelocity().y));
			this->figurasActivas[i]->setMovimientoIzq(true);
			this->figurasActivas[i]->setMovimientoDer(false);
		}
	}
}

void Escenario::moverDerecha(){

	if (this->puedeMoverseDerecha && !(this->gusanoActivo->estaMuerto())) {
		b2Body* cuerpo = this->gusanoActivo->getBody();
		cuerpo->SetLinearVelocity(b2Vec2(velocidadGusano,cuerpo->GetLinearVelocity().y));
		this->gusanoActivo->setMovimientoDer(true);
		this->gusanoActivo->armaActual.sentidoDisparo = false;
		this->gusanoActivo->setMovimientoIzq(false);
	}
}

void Escenario::realizarDisparo(){
	//if (this->puedeDisparar && !(this->gusanoActivo->estaMuerto())){
	//	if(! this->gusanoActivo->getArmaSeleccionada()->fueDisparada())
	//		if(this->gusanoActivo->tieneUnArma())
	//			this->gusanoActivo->getArmaSeleccionada()->disparar();
	//}


}

Gusano* Escenario::getGusanoActivo(){

	return this->gusanoActivo;
}

void Escenario::moverDerechaClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) &&  (this->puedeMoverseDerechaClientes[i]) && !(this->figurasActivas[i]->estaMuerto())) {
			b2Body* cuerpo = this->figurasActivas[i]->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(velocidadGusano,cuerpo->GetLinearVelocity().y));
			this->figurasActivas[i]->setMovimientoDer(true);
			this->figurasActivas[i]->setMovimientoIzq(false);
		}
	}
}

Figura* Escenario::getFiguraActiva(){
	return this->gusanoActivo;
}

vector<Gusano*> Escenario::getFigurasActivas(){
	return this->figurasActivas;
}

b2Vec3 Escenario::hayExplosion() {
	list<Arma*>::iterator it = this->listaArmas->begin();
	while (it != this->listaArmas->end()) {
		if ((*it)->getExplotar()) {
			b2Vec2 pos = (*it)->getBody()->GetPosition();
			float radio = (*it)->getRadio();
			this->explotar(pos.x,pos.y,radio);
			this->terreno->destruirTerreno(pos.x,pos.y,radio);
			this->world->DestroyBody((*it)->getBody());
			this->gusanoActivo->setArma(NULL);
			delete (*it);
			this->listaArmas->erase(it);
			return b2Vec3(pos.x,pos.y,radio);
		} else {
			++it;
		}
	}
	list<Figura*>::iterator it2 = this->listaFiguras->begin();
	while (it2 != this->listaFiguras->end()) {
		Gusano* gusano = (Gusano*) (*it2);
		if (gusano->getExplota()){
			b2Vec2 pos = gusano->getBody()->GetPosition();
			this->explotar(pos.x ,pos.y,radioExplosionGusano);
			this->terreno->destruirTerreno(pos.x,pos.y,radioExplosionGusano);
			gusano->setExplota(false);
			return b2Vec3(pos.x,pos.y,radioExplosionGusano);
		} else {
			++it2;
		}
	}
	return b2Vec3(0,0,-1);
}

void Escenario::explotar(float x, float y, int radio) {
	float potenciaTotal = potenciaPorUnidad * radio;
	b2Vec2 posicion,distanciaV,fuerza;
	float distancia,fuerzaTotal,angulo;
	for (std::list<Figura*>::const_iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		posicion = (*it)->getPosicion();
		distanciaV = b2Vec2(posicion.x - x, posicion.y - y );
		distancia = distanciaV.Length();
		if (distancia < radio ) {
			if (distancia < anchoGusano) {
				distancia = anchoGusano;
			}
			fuerzaTotal = potenciaTotal / distancia;
			angulo = atan2(distanciaV.y , distanciaV.x) * RADTODEG;
			if (angulo < 0) {
				angulo += 360;
			} else {
				if (angulo > 360) {
					angulo -= 360;
				}
			}
			fuerza.x = fuerzaTotal * coseno[(int) (angulo)];
			fuerza.y = fuerzaTotal * seno[(int) (angulo)];
			(*it)->getBody()->ApplyForceToCenter(fuerza,true);
			(*it)->explotar(fuerzaTotal);
		}

	}
}

void Escenario::detenerMovimientos(){
	 puedeMoverseArriba = false;
	 puedeMoverseAbajo = false;
	 puedeMoverseIzquierda = false;
	 puedeMoverseDerecha = false;
	 puedeDisparar = false;
	 puedeSaltar= false;
	 Reproductor::getReproductor()->detenerSonido(CAMINANDO);

}


void Escenario::setGusanoActivo(Gusano* gusano){
	//el gusano q era activo antes tiene q volver a ser normal
	if(this->gusanoActivo != NULL){
		this->gusanoActivo->setActivo(false);
		this->gusanoActivo->setTipoArma(NINGUNA);
		this->gusanoActivo->quedateQuietoWachin();
	}

	this->gusanoActivo = gusano;
}

bool Escenario::hayExposionPendiente(){

	if(!huboDisparo) return false;
	//si hubo disparo chequeo q no se este cargando la barrita, y tambien si hay explosiones pendientes:

	//si puedeDisparar esta true es porq estan cargando la potencia con la barra.
	else if(puedeDisparar == true)  return true;

	//si la lista de armas no esta vacia, hay explosiones pendientes
	else if (!listaArmas->empty()) return true;

	else return false;

}

bool Escenario::getHuboDisparo(){
	return this->huboDisparo;
}

void Escenario::setHuboDisparo(bool b){ 
	this->huboDisparo = b;
}

bool Escenario::getPuedeDisparar(){
	return this->puedeDisparar;
}

int Escenario::restarVidaGusanos(){

	int restado = 0;

	for (std::list<Figura*>::const_iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		
		Gusano* gus = (Gusano*)(*it);

		if(gus->bufferVida > 0){
			if(gus->vida > 0){
				gus->vida--;
				restado++;	
			}
			gus->bufferVida--;
						
		}

		if (gus->getVida()==0 && gus->bufferVida==0 && gus->numContactos > 0){
			gus->setMuerto(true);
		}


	}
	return restado;
}

void Escenario::setBloquearTeclas(bool bloquear){
	this->bloquearTeclas = bloquear;
}