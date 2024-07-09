/*In this file we define the particle that we want to generate from the primary generator*/

/*Including the generator header file with all the required classes and functions*/
 #include "generator.hh"
 #include "Randomize.hh"
 #include <cmath>
 
 /*Constructor and destruction function of the primary generator which creates a new instance of the particle gun and deletes the particle gun respectively.*/
 MyPrimaryGenerator::MyPrimaryGenerator()
 {
 	fParticleGun = new G4ParticleGun(1);
 	
 	G4double pi		= 4.0*atan(1.0);
 	G4double R 		= 40;												//in cm
 	G4double Theta 	= 0.0 * (pi/180.0);
 	G4double Phi	= 0.0 * (pi/180.0);
 	
 	//G4double x = R * sin(Theta) * cos(Phi);
 	//G4double y = R * sin(Theta) * sin(Phi);
 	//G4double z = R * cos(Theta);
 	
 	G4double xsz = 0.615;
 	G4double ysz = 4.0;
 	
 	G4double x = xsz*0.5*(G4UniformRand()-0.5);
 	G4double y = ysz*0.5*(G4UniformRand()-0.5);
 	G4double z = R;
 	
 	G4cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<G4endl;
 	
    /*Getting the attributes of our particle(proton) from the G4ParticleTable using FindParticle function.*/
 	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
 	//G4ParticleDefinition *particle = particleTable->FindParticle("geantino");
 	//G4ParticleDefinition *particle = particleTable->FindParticle("gamma");
 	G4ParticleDefinition *particle = particleTable->FindParticle("neutron");
 	
 	/*Defining the position and momentum of the particle using G4ThreeVector to define and SetParticle function to define the properties*/
 	G4ThreeVector pos(x*cm, y*cm, z*cm);
 	G4ThreeVector mom(0, 0, -1.0);
 	
 	fParticleGun->SetParticlePosition(pos);
 	fParticleGun->SetParticleMomentumDirection(mom);
 	//fParticleGun->SetParticleMomentum(0.0*MeV);
 	fParticleGun->SetParticleEnergy(0.025*eV);
 	fParticleGun->SetParticleDefinition(particle);
 	
 	
 }
 
 MyPrimaryGenerator::~MyPrimaryGenerator()
 {
 	delete fParticleGun;
 }
 
 /*In this function we define which particle we need from our particle gun and define its properties.*/
 void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
 {
 	G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();
 	
 	if(particle == G4Geantino::Geantino())
 	{
 		G4int Z = 55;
 		G4int A = 137;
 		
 		G4double charge = 0.*eplus;
 		G4double energy = 0.*keV;
 		
 		G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
 		
 		fParticleGun->SetParticleDefinition(ion);
 		fParticleGun->SetParticleCharge(charge);
 	}
 	/*Here we generate the particle*/
 	fParticleGun->GeneratePrimaryVertex(anEvent);	
 }
 
 /*MyPrimaryGenerator::MaxwellVDist(G4double V){
	 G4double pi		= 4.0*atan(1.0);
	 G4double kT		= 25.0*meV;
	 G4double prefac 	= 1.0/sqrt(pi*(E*meV)*kT);
	 G4double expn		= -(E*meV)/kT;
	 return exp(expn)*prefac;
 }*/
 	
 	
