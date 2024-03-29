/*This is the construction header file where we define the class, variables and functions required for the construction of the environment and the detector*/

//First we include all the necessary header files
#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

//Here we define the DetectorConstruction class and its variables and functions
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();//The constructor and destructor function of the class
	~MyDetectorConstruction();
	
	G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}
	
	//This is the Construct function of the type G4VPhysicalVolume which is used to construct the detector and environment with the required dimenstions and properties
	virtual G4VPhysicalVolume *Construct();
private:
//Here we define three variables for the Size of the Environment, the size of the detector and the position of the detector inside the environment

	G4ThreeVector detSize,dist,Size;	
	G4Box *solidWorld, *solidRadiator, *solidDetector;
	G4Box *solidScintillator, *detVol;
	G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector, *logicScintillator, *logicshell, *logicdetVol;
	G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, *physScintillator, *physshell, *physdetVol;
	G4VPhysicalVolume *physshellMuVetoOut, *physshellHDPEOuter, *physshellBP, *physshellLead, *physshellHDPEInner, *physshellMuVetoIn, *physshellCuColdBox;
	G4LogicalVolume *logicshellMuVetoOut, *logicshellHDPEOuter, *logicshellBP, *logicshellLead, *logicshellHDPEInner, *logicshellMuVetoIn, *logicshellCuColdBox;
	
	
	G4Material *SiO2, *H2O, *Aerogel, *worldMat, *NaI, *myTolueneMat, *BoratedPE, *HDPE, *Lead, *Copper, *Vaccum;
	G4Element *C, *Na, *I;//, *Cu, *Pb;
	

	void DefineMaterials();
	
	void ConstructCherenkov();
	void ConstructScintillator();
	G4VSolid* ConstructShell(double xsz, double ysz, double zsz, double thickness, double offset);
	
	virtual void ConstructSDandField();
	
	G4GenericMessenger *fMessenger;
	
	G4LogicalVolume *fScoringVolume;
	
	G4int nCols, nRows;
	
	G4double xWorld,yWorld,zWorld;
	G4bool isCherenkov, isScintillator;
	
	G4OpticalSurface *mirrorSurface;
};

#endif
