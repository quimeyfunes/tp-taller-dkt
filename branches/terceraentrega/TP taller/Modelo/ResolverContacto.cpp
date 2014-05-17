#include "ResolverContacto.h"


ResolverContacto::ResolverContacto() {
}

void ResolverContacto::BeginContact(b2Contact* contact) {
	  
	// Para ver si gusano toca algo por abajo y volver a saltar
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	  
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Gusano* gusano = (Gusano*) fixtureUserData;
		gusano->agregarContacto();
	}
	  
	fixtureUserData = contact->GetFixtureB()->GetUserData();
      
	if ( fixtureUserData ) {
		Gusano* gusano = (Gusano*) fixtureUserData;
		gusano->agregarContacto();
	}
}

void ResolverContacto::EndContact(b2Contact* contact) {

	// Para ver si gusano toca algo por abajo y volver a saltar
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	  
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Gusano* gusano = (Gusano*) fixtureUserData;
		gusano->sacarContacto();
	}
	  
	fixtureUserData = contact->GetFixtureB()->GetUserData();
      
	if ( fixtureUserData ) {
		Gusano* gusano = (Gusano*) fixtureUserData;
		gusano->sacarContacto();
	}
}

