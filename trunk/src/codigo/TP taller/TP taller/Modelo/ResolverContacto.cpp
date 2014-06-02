#include "ResolverContacto.h"


ResolverContacto::ResolverContacto() {
}

void ResolverContacto::BeginContact(b2Contact* contact) {

	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if ( fixtureUserData ) {
		Figura* fig = (Figura*) fixtureUserData;
		fig->BeginContact();
	}
	  
	fixtureUserData = contact->GetFixtureB()->GetUserData();
      
	if ( fixtureUserData ) {
		Figura* fig = (Figura*) fixtureUserData;
		fig->BeginContact();
	}
}

void ResolverContacto::EndContact(b2Contact* contact) {

	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if ( fixtureUserData ) {
		Figura* fig = (Figura*) fixtureUserData;
		fig->EndContact();
	}
     
	fixtureUserData = contact->GetFixtureB()->GetUserData();

	if ( fixtureUserData ) {
		Figura* fig = (Figura*) fixtureUserData;
		fig->EndContact();
	}
}

void ResolverContacto::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){

	b2Body* cuerpo = contact->GetFixtureA()->GetBody();
	
	for (b2Fixture* fixture = cuerpo->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		void* fixtureUserData = fixture->GetUserData();
		if( fixtureUserData ) {	
			Figura* fig = (Figura*) fixtureUserData;
			fig->PostSolve(impulse->normalImpulses[0]);
		}
	}
     
	cuerpo = contact->GetFixtureB()->GetBody();
	
	for (b2Fixture* fixture = cuerpo->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		void* fixtureUserData = fixture->GetUserData();
		if ( fixtureUserData ) {
			Figura* fig = (Figura*) fixtureUserData;
			fig->PostSolve(impulse->normalImpulses[0]);
		}
	}

}
