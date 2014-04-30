#include "ResolverContacto.h"


ResolverContacto::ResolverContacto() {
}

void ResolverContacto::BeginContact(b2Contact* contact) {
	
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	  
	// Para ver si gusano toca algo por abajo y volver a saltar
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	  
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Gusano* gusano = (Gusano*) fixtureUserData;
		if (this->escenario->getFiguraActiva() == gusano) {
			this->escenario->setPuedeSaltar(true);
		}
	}
	  
	fixtureUserData = contact->GetFixtureB()->GetUserData();
      
	if ( fixtureUserData ) {
		Gusano* gusano = (Gusano*) fixtureUserData;
		if (this->escenario->getFiguraActiva() == gusano) {
			this->escenario->setPuedeSaltar(true);
		}
	}
}

void ResolverContacto::EndContact(b2Contact* contact) {
	
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	  
	// Para ver si gusano toca algo por abajo y volver a saltar
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	  
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Gusano* gusano = (Gusano*) fixtureUserData;
		if (this->escenario->getFiguraActiva() == gusano) {
			this->escenario->setPuedeSaltar(false);
		}
	}
	  
	fixtureUserData = contact->GetFixtureB()->GetUserData();
      
	if ( fixtureUserData ) {
		Gusano* gusano = (Gusano*) fixtureUserData;
		if (this->escenario->getFiguraActiva() == gusano) {
			this->escenario->setPuedeSaltar(false);
		}
	}
}

void ResolverContacto::setEscenario(Escenario* escenario) {
	this->escenario = escenario;
}