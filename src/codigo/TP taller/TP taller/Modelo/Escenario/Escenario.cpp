#include "escenario.h"

Escenario::Escenario(){
}

Escenario::Escenario(int altoU,int anchoU,int nivelAgua, float relacionAncho, float relacionAlto, int maximosClientes){
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->listaFiguras = new list<Figura*>();
	this->maximosClientes = maximosClientes;

	b2Vec2* gravity = new b2Vec2(gravedadX, gravedadY);
	this->world = new b2World(*gravity);

	this->gusanoActivo = NULL;
	this->puedeMoverseArriba = false;
	this->puedeMoverseDerecha = false;
	this->puedeMoverseIzquierda = false;
	this->puedeDisparar = false;
	this->puedeMoverseAbajo = false;

	for(int i=0; i < this->maximosClientes; i++){
		this->figurasActivas.push_back(NULL);
		this->puedeMoverseArribaClientes.push_back(false);
		this->puedeMoverseDerechaClientes.push_back(false);
		this->puedeMoverseIzquierdaClientes.push_back(false);
		this->puedeSaltarClientes.push_back(false);
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

void Escenario::notificar() {
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->notificar();
	}

	this->moverse();
	this->moverseClientes();
}

Gusano* Escenario::crearGusano(ObjetoParseado objeto){
	Gusano* gusano = new Gusano(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.ancho,objeto.alto,objeto.masa,this->maximosClientes);
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
	
		Gusano* gusano = new Gusano(x,y,0,this->world,false,anchoGusano,altoGusano,10,this->maximosClientes);
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
				this->gusanoActivo->setMeClickearon(false);
			}
			this->gusanoActivo = (Gusano*)(*it);
			this->gusanoActivo->setMeClickearon(true);
			return true;
		}
	}
	this->terreno->destruirTerreno(x,y,30);
	this->explotar(x,y,30);
	return false;
}

void Escenario::clickCliente(int cliente, list<Gusano*> figurasCliente, list<Gusano*> figurasOtrosCliente,float x, float y){
	//Recorro solo las figuras del cliente para saber si tengo que seleccionar una figura como activa
	for (std::list<Gusano*>::const_iterator it = figurasCliente.begin(); it != figurasCliente.end(); it++) {
		if ((*it)->meClickeo(x,y) && !(*it)->estaMuerto()) {
			//printf("cliente %d clickeo uno de sus gusanos.\n",cliente);
			//this->figurasActivas[cliente] = (*it);
			if (this->figurasActivas[cliente] != NULL) {
				this->figurasActivas[cliente]->setMeClickearon(false,cliente);
			}
			this->figurasActivas[cliente] = (Gusano*)(*it);
			this->figurasActivas[cliente]->setMeClickearon(true,cliente);
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

void Escenario::arriba(bool arriba){
	this->puedeMoverseArriba = arriba;
}

void Escenario::arribaCliente(int cliente ,bool arriba){
	this->puedeMoverseArribaClientes[cliente] = arriba;
}

void Escenario::setPuedeSaltarCliente(int cliente,bool puedeSaltar) {
	this->puedeSaltarClientes[cliente] = puedeSaltar;
}

void Escenario::izquierda(bool izquierda){
	this->puedeMoverseIzquierda = izquierda;
}

void Escenario::disparar(bool disparo){
	this->puedeDisparar = disparo;
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
	if ((this->gusanoActivo != NULL) && !(this->gusanoActivo->estaMuerto())) {
		this->saltar();
		this->bajar();
		this->moverDerecha();
		this->moverIzquierda();
	}
}

void Escenario::moverseClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && !(this->figurasActivas[i]->estaMuerto()) && !(this->figurasActivas[i]->estaMuerto())) {
			this->saltarClientes();
			this->moverDerechaClientes();
			this->moverIzquierdaClientes();
		}
	}
}

void Escenario::saltar(){
	if (this->gusanoActivo->puedeSaltar() && (this->puedeMoverseArriba) && !(this->gusanoActivo->estaMuerto())) {
		if(!this->gusanoActivo->tieneUnArma()){
			b2Body* cuerpo = this->gusanoActivo->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(0,-25));
		}else{
			this->gusanoActivo->getArmaSeleccionada()->aumentarAnguloDisparo();
		}
		//cuerpo->ApplyLinearImpulse(b2Vec2(0,-100),this->figuraActiva->getPosicion(),true);
	}
}

void Escenario::bajar(){
	if(this->puedeMoverseAbajo && (!this->gusanoActivo->estaMuerto()) && (this->gusanoActivo->tieneUnArma())){
		this->gusanoActivo->getArmaSeleccionada()->disminuirAnguloDisparo();
	}
}

void Escenario::saltarClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) && ((Gusano*)this->figurasActivas[i])->puedeSaltar() && (this->puedeMoverseArribaClientes[i]) && !(this->figurasActivas[i]->estaMuerto())) {
			b2Body* cuerpo = this->figurasActivas[i]->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(0,-25));
		}
	}
}

void Escenario::moverIzquierda(){
	if (this->puedeMoverseIzquierda && !(this->gusanoActivo->estaMuerto())) {
		b2Body* cuerpo = this->gusanoActivo->getBody();
		cuerpo->SetLinearVelocity(b2Vec2(-5,cuerpo->GetLinearVelocity().y));
		this->gusanoActivo->setMovimientoIzq(true);
		this->gusanoActivo->setMovimientoDer(false);
	}
}

void Escenario::moverIzquierdaClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) &&  (this->puedeMoverseIzquierdaClientes[i]) && !(this->figurasActivas[i]->estaMuerto())) {
			b2Body* cuerpo = this->figurasActivas[i]->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(-5,cuerpo->GetLinearVelocity().y));
			this->figurasActivas[i]->setMovimientoIzq(true);
			this->figurasActivas[i]->setMovimientoDer(false);
		}
	}
}

void Escenario::moverDerecha(){
	if (this->puedeMoverseDerecha && !(this->gusanoActivo->estaMuerto())) {
		b2Body* cuerpo = this->gusanoActivo->getBody();
		cuerpo->SetLinearVelocity(b2Vec2(5,cuerpo->GetLinearVelocity().y));
		this->gusanoActivo->setMovimientoDer(true);
		this->gusanoActivo->setMovimientoIzq(false);
	}
}

void Escenario::realizarDisparo(){
	if (this->puedeDisparar && !(this->gusanoActivo->estaMuerto())){
		if(! this->gusanoActivo->getArmaSeleccionada()->fueDisparada())
			if(this->gusanoActivo->tieneUnArma())
				this->gusanoActivo->getArmaSeleccionada()->disparar();
	}


}

Gusano* Escenario::getGusanoActivo(){

	return this->gusanoActivo;
}

void Escenario::moverDerechaClientes(){
	for(int i=0; i < this->maximosClientes; i++){
		if ((this->figurasActivas[i] != NULL) &&  (this->puedeMoverseDerechaClientes[i]) && !(this->figurasActivas[i]->estaMuerto())) {
			b2Body* cuerpo = this->figurasActivas[i]->getBody();
			cuerpo->SetLinearVelocity(b2Vec2(5,cuerpo->GetLinearVelocity().y));
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

void Escenario::explotar(float x, float y, int radio) {
	float potenciaPorUnidad = 3000; //Iria en constantesFisicas, q en esta version todavia no esta
	float potenciaTotal = potenciaPorUnidad * radio;
	b2Vec2 posicion,distanciaV,fuerza;
	float distancia,fuerzaTotal,angulo;
	for (std::list<Figura*>::const_iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		posicion = (*it)->getPosicion();
		distanciaV = b2Vec2(posicion.x - x, posicion.y - y );
		distancia = distanciaV.Length();
		if (distancia < radio ) {
			fuerzaTotal = potenciaTotal / distancia;
			angulo = atan2(distanciaV.y , distanciaV.x);
			fuerza.x = fuerzaTotal * cos (angulo);
			fuerza.y = fuerzaTotal * sin (angulo);
			(*it)->getBody()->ApplyForceToCenter(fuerza,true);
		}

	}
}

