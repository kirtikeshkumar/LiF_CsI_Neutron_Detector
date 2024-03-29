/*This file is required to integrate the necessary physics like Electromagnetism and Optics*/
#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics());
	RegisterPhysics (new G4OpticalPhysics());
	//RegisterPhysics (new G4DecayPhysics());
	//RegisterPhysics (new G4RadioactiveDecayPhysics());
	//G4VModularPhysicsList *physicsList=new Shielding;
}

MyPhysicsList::~MyPhysicsList()
{}
