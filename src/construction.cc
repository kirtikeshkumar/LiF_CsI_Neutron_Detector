/*This is the main construction file where we assign all the attributes of the detector and the environment and the material of the detector*/

//Including the construction header file
#include "construction.hh"
#include "detector.hh"

//In the constructor function we use std::cin to take the dimensions of the environment and the detector along with the position of the detector as user input
MyDetectorConstruction::MyDetectorConstruction()
{
	 /*std::cout<<"Enter Environment Dimensions:";
	 std::cin>>Size;
	 std::cout<<"Enter Detector Dimensions:";
	 std::cin>>detSize;
	 std::cout<<"Enter Detector Position:";
	 std::cin>>dist;*/
	 fMessenger= new G4GenericMessenger(this, "/detector/", "Detector Construction");
	 
	 fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	 fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	 fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Taggle Cherenkov setup");
	 fMessenger->DeclareProperty("isScintillator", isScintillator, "Taggle Scintillator setup");	
	 
	 nCols = 100;
	 nRows = 100; 
	 
	 DefineMaterials();
	 
	 xWorld = 5.*m;
	 yWorld = 5.*m;
	 zWorld = 5.*m;
	 
	 isCherenkov = false;
	 isScintillator = true;
	 
	 Size=G4ThreeVector(0.5,0.5,0.5);
	 detSize=G4ThreeVector(0.4,0.4,0.001);
	 dist=G4ThreeVector(0.,0.,0.1);
}

//The destructor function
MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	//G4NistManager to get the required elements
	G4NistManager *nist = G4NistManager::Instance();
	
	//Here we define the material of our detector. Here we are using Aerogel as our 		detector material which is made up of a mixture of SiO2,H2O and Carbon
	
	/*//Creating Sio2 from Silicon and Oxygen	
	SiO2 = new G4Material("Si02",2.201*g/cm3,2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"),1);
	SiO2->AddElement(nist->FindOrBuildElement("O"),2);
	//Creating H20	
	H2O = new G4Material("H2O",1.000*g/cm3,2);
	H2O->AddElement(nist->FindOrBuildElement("H"),2);
	H2O->AddElement(nist->FindOrBuildElement("O"),1);
	//Getting Carbon	
	C = nist->FindOrBuildElement("C");

	//Preparing Aerogel from SiO2,H20 and C by combining them in their appropriate 			proportions	
	Aerogel = new G4Material("Aerogel",0.200*g/cm3,3);
	Aerogel->AddMaterial(SiO2,62.5*perCent);
	Aerogel->AddMaterial(H2O,37.4*perCent);
	Aerogel->AddElement(C,0.1*perCent);
	*/
	worldMat =nist->FindOrBuildMaterial("G4_AIR");
	myTolueneMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	Vaccum = nist->FindOrBuildMaterial("G4_Galactic");
	
	G4double fractionmass;
	//Borated Poly-ethylene 30
	BoratedPE = new G4Material("BoratedPE",1.04*g/cm3,2);
	BoratedPE->AddMaterial(nist->FindOrBuildMaterial("G4_POLYETHYLENE"),fractionmass=95.*perCent);
	BoratedPE->AddElement(nist->FindOrBuildElement("B"),fractionmass=5.*perCent);
	
	HDPE = new G4Material("HDPE",0.97*g/cm3,1);
	HDPE->AddMaterial(nist->FindOrBuildMaterial("G4_POLYETHYLENE"),100.*perCent);
	
	Lead = new G4Material("Pb",11.4*g/cm3,1);
	Lead->AddElement(nist->FindOrBuildElement("Pb"),100.*perCent);
	Copper = new G4Material("Cu",8.96*g/cm3,1);
	Copper->AddElement(nist->FindOrBuildElement("Cu"),100.*perCent);
	
	//Defining the refractive index of the Aerogel detector and the environment so that 	  we can see the Cherenkov Light
	G4double energy[2] ={1.239841939*eV/0.9, 1.239841939*eV/0.2};
	G4double rindexWorld[2] = {1.0, 1.0};
	G4double rindexPVT[2] = {1.58, 1.58};
	G4double rindexAcrylic[2] = {1.60, 1.60};
	G4double rindexOptical[2] = {1.49, 1.49};
	
	
	G4double reflectivity[2] = {1.0, 1.0};
	
	G4double energy_spectrum[31] = {2.3884*eV, 2.4523*eV, 2.5240*eV, 2.5945*eV, 2.6358*eV, 2.6664*eV, 2.7058*eV, 2.7528*eV, 2.7948*eV, 2.8122*eV, 2.8337*eV, 2.8414*eV, 2.8491*eV, 2.8743*eV, 2.8995*eV, 2.9227*eV, 2.9604*eV, 2.9873*eV, 3.0093*eV, 3.0254*eV, 3.0494*eV, 3.0995*eV, 3.1235*eV, 3.1503*eV, 3.1862*eV, 3.2275*eV, 3.2700*eV, 3.3184*eV, 3.3538*eV, 3.4048*eV, 3.4474*eV};
	G4double energy_fraction[31] = {0.0225734, 0.0539278, 0.0985471, 0.171136, 0.229339, 0.296938, 0.395068, 0.489164, 0.579315, 0.641705, 0.745296, 0.82371, 0.898134, 0.971118, 0.992241, 0.972151, 0.877506, 0.761649, 0.676411, 0.56328, 0.427499, 0.249022, 0.18772, 0.146354, 0.112922, 0.0821281, 0.0659618, 0.0484438, 0.0376407, 0.0281014, 0.0279145};
	
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX",energy, rindexWorld, 2);	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	G4double fraction[2]= {1.0,1.0};
	G4double absorption[2] = {2.1*m,2.1*m};
	
	G4MaterialPropertiesTable *mptPVT = new G4MaterialPropertiesTable();
	mptPVT->AddProperty("RINDEX",energy, rindexPVT, 2);
	mptPVT->AddProperty("SCINTILLATIONCOMPONENT1", energy_spectrum, energy_fraction, 31);
	mptPVT->AddConstProperty("SCINTILLATIONYIELD", 11136/MeV);
	mptPVT->AddConstProperty("RESOLUTIONSCALE", 1.0);
	mptPVT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1*ns);
	mptPVT->AddConstProperty("SCINTILLATIONYIELD1", 1.);
	mptPVT->AddProperty("ABSLENGTH", energy, absorption,2);
	
	myTolueneMat->SetMaterialPropertiesTable(mptPVT);
	
	mirrorSurface = new G4OpticalSurface("mirrorSurface");
	
	mirrorSurface->SetType(dielectric_metal);
	mirrorSurface->SetFinish(ground);
	mirrorSurface->SetModel(unified);
	
	G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
	mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
	mirrorSurface->SetMaterialPropertiesTable(mptMirror);

}

void MyDetectorConstruction::ConstructCherenkov()
{
//Similarly setting the dimensions,material and position of the detector to create 		  the final Physical volume of the detector	
	solidRadiator =new G4Box("solidRadiator",detSize[0]*m,detSize[1]*m,detSize[2]*m);
	
	logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
	
	fScoringVolume = logicRadiator;
	
	physRadiator = new G4PVPlacement(0,G4ThreeVector(dist[0]*m,dist[1]*m,dist[2]*m), logicRadiator,"physRadiator",logicWorld,false,0,true);
	
	solidDetector = new G4Box("solidDetector",xWorld/nRows,yWorld/nCols,0.01*m);
	
	logicDetector = new G4LogicalVolume(solidDetector,worldMat,"logicDetector");
	
	for(G4int i=0;i< nRows;i++)
	{
		for(G4int j=0;j< nCols;j++)
		{
			physDetector = new G4PVPlacement(0,G4ThreeVector(-0.5*m+(i+0.5)*m/nRows,-0.5*m+(j+0.5)*m/nCols,0.49*m), logicDetector,
"physDetector",logicWorld,false,j+i*nCols,true);
		}
	}

}

void MyDetectorConstruction::ConstructScintillator()
{
	/*solidScintillator = new G4Box("solidScintillator", 0.05*m,0.5*m,0.05*m);
	
	logicScintillator = new G4LogicalVolume(solidScintillator, myTolueneMat, "logicalScintillator");
	
	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);
	
	fScoringVolume = logicScintillator;
	
	physScintillator = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.055*m), logicScintillator,"physScintillator",logicWorld,false,0,true);*/
	
	/*solidDetector = new G4Box("solidDetector",0.05*m,0.01*m,0.05*m);
	logicDetector = new G4LogicalVolume(solidDetector,worldMat,"logicDetector");
	physDetector = new G4PVPlacement(0,G4ThreeVector(0.,0.51*m,0.05*m), logicDetector,"physDetector",logicWorld,false,0,true);	
	physDetector = new G4PVPlacement(0,G4ThreeVector(0.,-0.51*m,0.05*m), logicDetector,"physDetector",logicWorld,false,0,true);	*/
	
	G4VSolid* shellMuVetoOut = ConstructShell(150.,150.,175.,4.,0.);
	G4VSolid* shellHDPEOuter = ConstructShell(142.,142.,167.,10.,0.);
	G4VSolid* shellBP = ConstructShell(122.,122.,147.,10.,0.);
	G4VSolid* shellLead = ConstructShell(102.,102.,127.,10.,0.);
	G4VSolid* shellHDPEInner = ConstructShell(82.,82.,107.,10.,0.);
	G4VSolid* shellMuVetoIn = ConstructShell(62.,62.,87.,4.,0.);
	G4VSolid* shellCuColdBox = ConstructShell(54.,54.,79.,2.,0.);
	detVol = new G4Box("Boxsensdet",25.*cm,25.*cm,37.5*cm);
	
	
	logicshellMuVetoOut = new G4LogicalVolume(shellMuVetoOut,myTolueneMat,"logicshellMuVetoOut");
	logicshellHDPEOuter = new G4LogicalVolume(shellHDPEOuter,HDPE,"logicshellHDPEOuter");
	logicshellBP 		= new G4LogicalVolume(shellBP,BoratedPE,"logicshellBP");
	logicshellLead 		= new G4LogicalVolume(shellLead,Lead,"logicshellLead");
	logicshellHDPEInner = new G4LogicalVolume(shellHDPEInner,HDPE,"logicshellHDPEInner");
	logicshellMuVetoIn 	= new G4LogicalVolume(shellMuVetoIn,myTolueneMat,"logicshellMuVetoIn");
	logicshellCuColdBox = new G4LogicalVolume(shellCuColdBox,Copper,"logicshellCuColdBox");
	logicdetVol			= new G4LogicalVolume(detVol,Vaccum,"logicdetVol");
	
	
	G4LogicalSkinSurface *skin1 = new G4LogicalSkinSurface("skin1", logicshellMuVetoOut, mirrorSurface);
	G4LogicalSkinSurface *skin2 = new G4LogicalSkinSurface("skin2", logicshellMuVetoIn, mirrorSurface);
	
	physshellMuVetoOut = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellMuVetoOut,"physshellMuVetoOut",logicWorld,false,1,true);
	physshellHDPEOuter = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellHDPEOuter,"physshellHDPEOuter",logicWorld,false,2,true);
	physshellBP 	   = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellBP,"physshellBP",logicWorld,false,3,true);
	physshellLead 	   = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellLead,"physshellLead",logicWorld,false,4,true);
	physshellHDPEInner = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellHDPEInner,"physshellHDPEInner",logicWorld,false,5,true);
	physshellMuVetoIn  = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellMuVetoIn,"physshellMuVetoIn",logicWorld,false,6,true);
	physshellCuColdBox = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicshellCuColdBox,"physshellCuColdBox",logicWorld,false,7,true);
	physdetVol		   = new G4PVPlacement(0,G4ThreeVector(0.9*m,0.,0.), logicdetVol,"physdetVol",logicWorld,false,8,true);
	
}

G4VSolid* MyDetectorConstruction::ConstructShell(double xsz, double ysz, double zsz, double thickness, double offset=0.0){
	G4VSolid* boxout = new G4Box("Boxout",0.5*xsz*cm,0.5*ysz*cm,0.5*zsz*cm);
	G4VSolid* boxin = new G4Box("Boxin",(0.5*xsz-thickness)*cm,(0.5*ysz-thickness)*cm,(0.5*zsz-thickness)*cm);
	G4VSolid* shell = new G4SubtractionSolid("BoxOut-BoxIn", boxout, boxin, 0, G4ThreeVector(0.,0.,offset*cm));
	return shell;
}

//The Construct function where we define the material of the detector and define the physical and logical volume of the environment and detector.
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	//Defining the dimenstions of the world environment	
	solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld);

	//Setiing Logical volume where we integrate the material to the world box	
	 //logicWorld= new G4LogicalVolume(solidWorld,worldMat,"logicWorld");
	 logicWorld= new G4LogicalVolume(solidWorld,Vaccum,"logicWorld");
	//Integrating the position of the world to the logical volume to create our final 			Physical Volume	
	 physWorld = new G4PVPlacement(0,G4ThreeVector(0., 0., 0.),logicWorld,"physWorld",0,false,0,true);
	 
	 if(isCherenkov)
	 	ConstructCherenkov();
	 
	 if(isScintillator)
	 	ConstructScintillator();
	

//Finally we return the physWorld as output	
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField(){
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
	/*logicshellMuVetoOut->SetSensitiveDetector(sensDet);
	logicshellHDPEOuter->SetSensitiveDetector(sensDet);
	logicshellBP->SetSensitiveDetector(sensDet);
	logicshellLead->SetSensitiveDetector(sensDet);
	logicshellHDPEInner->SetSensitiveDetector(sensDet);
	logicshellMuVetoIn->SetSensitiveDetector(sensDet);
	logicshellCuColdBox->SetSensitiveDetector(sensDet);*/
	logicdetVol->SetSensitiveDetector(sensDet);
}
