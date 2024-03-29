/*This is the main simulation file*/

/*Including all the required header files*/
#include<iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "Shielding.hh"
//#include "physics.hh"
#include "action.hh"
#include <G4OpticalPhysics.hh>

/*The main function where all the objects are initialized and all the commands needed are performed*/

int main(int argc,char** argv)
{
//creating a G4RunManager instance to initialize all our required objects and functions
	G4RunManager *runManager = new G4RunManager();
	
//Initializing the detector construction, physics implementation and action initialization files	
	runManager->SetUserInitialization(new MyDetectorConstruction());
	//runManager->SetUserInitialization(new MyPhysicsList());
	//runManager->SetUserInitialization(new Shielding);
  	G4VModularPhysicsList *physicsList = new Shielding;
	//G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
	//physicsList->RegisterPhysics(opticalPhysics);
	runManager->SetUserInitialization(physicsList);
	//runManager->SetUserInitialization(physicsList);
	runManager->SetUserInitialization(new MyActionInitialization());
	runManager->Initialize();
	
//creating an instance of the UI Executive and Vis Manager for UI and visualization of our world	
	G4UIExecutive *ui = 0;
	
	if(argc == 1)
	{
		ui = new G4UIExecutive(argc,argv);
	}
	
	G4VisManager *visManager= new G4VisExecutive();
	visManager->Initialize();
	
	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	
	
	if(ui)
	{
		UImanager->ApplyCommand("/control/execute vis.mac");
//finally we start the session	
		ui->SessionStart();
	}
	else
	{
		G4String command = "/control/execute ";
		G4String fileName=argv[1];
		UImanager->ApplyCommand(command+fileName);
	}

	return 0;
}
