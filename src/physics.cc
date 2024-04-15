/*This file is required to integrate the necessary physics like Electromagnetism and Optics*/
#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4DecayPhysics());
	RegisterPhysics (new G4RadioactiveDecayPhysics());
	RegisterPhysics (new G4IonBinaryCascadePhysics());
	RegisterPhysics (new G4EmStandardPhysics());
	RegisterPhysics (new G4EmExtraPhysics());
	RegisterPhysics (new G4HadronElasticPhysics());
	RegisterPhysics (new G4StoppingPhysics());
	RegisterPhysics (new G4HadronPhysicsQGSP_BIC_HP());
	//RegisterPhysics (new G4NeutronTrackingCut());
	
	//RegisterPhysics (new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
