/*This is the main construction file where we assign all the attributes of the detector and the environment and the material of the detector*/

//Including the construction header file
#include "construction.hh"
#include "detector.hh"
#include <cmath>

MyDetectorConstruction::MyDetectorConstruction()
{	 
	 DefineMaterials();
	 
	 xWorld = 0.5*m;
	 yWorld = 0.5*m;
	 zWorld = 0.5*m;	 
	 
}

//The destructor function
MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	//G4NistManager to get the required elements
	G4NistManager *nist = G4NistManager::Instance();
	
	worldMat =nist->FindOrBuildMaterial("G4_AIR"); 						//defining world material
	Vaccum = nist->FindOrBuildMaterial("G4_Galactic");
	//Optical Properties of world
	G4double energy[2] ={1.239841939*eV/0.9, 1.239841939*eV/0.2};		
	G4double rindexWorld[2] = {1.0, 1.0};								//refractive index of world over the energy range in above variable
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX",energy, rindexWorld, 2);	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	//Lithium Fluoride
	LiF = nist->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
	
	//Cesium Iodide (no doping)
	CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
	
	//Cesium Iodide (Tl doped) (doping conc. 0.355%wt from doi:10.1088/1742-6596/1144/1/012105)
	G4double fractionmass;
	G4double TlDopantMassFraction = 0.355; 								//modify the dopant mass fraction here
	G4double CsIDopantMassFraction = 1.0-TlDopantMassFraction;
	CsI_Tl = new G4Material("CsI_Tl", 4.51*g/cm3, 2); 
	CsI_Tl->AddMaterial(CsI, fractionmass = CsIDopantMassFraction);  
	CsI_Tl->AddElement(nist->FindOrBuildElement("Tl"), fractionmass = TlDopantMassFraction); 
	 
	//Optical Properties of CsI(Tl) are defined here
	G4double energyCsI_Tl[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};//here wavelength range of 200-900 nm is considered as a block more points can be added as required
	G4double rindexCsI_Tl[2] = {1.79,1.79}; 							//a constant refractive index of 1.79 over entire range considered. more details can be added by changing the above energy and corresponding r index
	//emmission spectra of CsI(Tl) taken from Luxium datasheet 
	G4double energy_spectrum[27] = {									
		1.7529*eV, 1.8036*eV, 1.8651*eV, 1.9165*eV, 2.0102*eV, 
		2.0653*eV, 2.1186*eV, 2.1590*eV, 2.2228*eV, 2.2789*eV, 
		2.3442*eV, 2.3878*eV, 2.4430*eV, 2.4801*eV, 2.5185*eV, 
		2.5399*eV, 2.5507*eV, 2.5950*eV, 2.6447*eV, 2.6802*eV, 
		2.7583*eV, 2.7969*eV, 2.8592*eV, 2.9290*eV, 3.1004*eV, 
		3.3359*eV, 3.5461*eV};											//this is the list of energies over which emission spectra is defined	
	G4double energy_fraction[27] = {
		0.1226, 0.1963, 0.2859, 0.3994, 0.5985, 0.7041, 0.7997, 0.8773, 
		0.9590, 0.9849, 0.9472, 0.8855, 0.8000, 0.7284, 0.6468, 0.5991, 
		0.5632, 0.4737, 0.4001, 0.3544, 0.2748, 0.245 , 0.2013, 0.1695, 
		0.1138, 0.0562, 0.0165};										//Intensity of emission at energies as given above
	G4double absorption[2] = {28*cm,34*cm};								//absorption length (needs value modification)
	G4MaterialPropertiesTable *mptCsITl = new G4MaterialPropertiesTable();
	mptCsITl->AddProperty("RINDEX",energy, rindexCsI_Tl, 2);
	mptCsITl->AddProperty("SCINTILLATIONCOMPONENT1", energy_spectrum, energy_fraction, 27);
	mptCsITl->AddConstProperty("SCINTILLATIONYIELD", 54/keV);			//Light yeild in photons/keV
	mptCsITl->AddConstProperty("RESOLUTIONSCALE", 1.0);					//stdDev in no. of photons = sqrt(SCINTILLATIONTEILD)*RESOLUTIONSCALE
	mptCsITl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1000*ns);  //Primary Decay Time
	mptCsITl->AddConstProperty("SCINTILLATIONYIELD1", 1.);				
	mptCsITl->AddProperty("ABSLENGTH", energy, absorption,2);
	CsI_Tl->SetMaterialPropertiesTable(mptCsITl);
	
	
	//Defining mirror surface 
	mirrorSurface = new G4OpticalSurface("mirrorSurface");
	mirrorSurface->SetType(dielectric_metal);
	mirrorSurface->SetFinish(polished);
	mirrorSurface->SetModel(unified);
	energy[0] = 1.239841939*eV/0.9;
	energy[1] = 1.239841939*eV/0.2;
	G4double reflectivity[2] = {1.0, 1.0};
	G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
	mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
	mirrorSurface->SetMaterialPropertiesTable(mptMirror);

}

void MyDetectorConstruction::ConstructSetup()
{	
	//define the size of each component
	G4ThreeVector LiFSz 	= G4ThreeVector(0.1, 40., 40.);				//size of LiF layer
	G4ThreeVector ScintSz 	= G4ThreeVector(3.0, 40., 40.);				//size of Scintillator Layer
	
	//Creating Layers as G4Box
	solidLiF	 = new G4Box("LiF", 0.5*LiFSz.x()*mm, 0.5*LiFSz.y()*mm, 0.5*LiFSz.z()*mm);
	solidScint	 = new G4Box("Scint", 0.5*ScintSz.x()*mm, 0.5*ScintSz.y()*mm, 0.5*ScintSz.z()*mm);
	
	//Defining Logical Volumes
	logicLiF	 = new G4LogicalVolume(solidLiF, LiF, "logicLiF");
	//logicScint 	 = new G4LogicalVolume(solidScint, CsI, "logicScint");
	logicScint 	 = new G4LogicalVolume(solidScint, CsI_Tl, "logicScint"); //if CsI(Tl) to be used
	
	//skin = new G4LogicalSkinSurface("skin", logicScint, mirrorSurface);
	
	/*//Defining Physical positionings
	 * Here the copynumber of each layer is as follows:
	 * 		1 for the leftmost LiF layer
	 * 		2 for the leftmost Scintillator layer
	 * 		then alternating between the layers
	*/
	G4int numScintLayers = 2;											//Give number of scintillator layers here
	G4int numLiFLayers = numScintLayers + 1;
	G4double xpos = 0.;
	for(G4int i = 0; i < numScintLayers; i++){							//Positioning Scintillator layers
		xpos = (i-0.5*(numScintLayers-1))*(ScintSz.x()+LiFSz.x());
		physScint = new G4PVPlacement(0,G4ThreeVector(xpos,0.,0.), logicScint,"physScint",logicWorld,false,2*(i+1),true);
	}
	for(G4int i = 0; i < numLiFLayers; i++){							//Positioning LiF layers
		xpos = (i-0.5*(numLiFLayers-1))*(ScintSz.x()+LiFSz.x());
		physLiF = new G4PVPlacement(0,G4ThreeVector(xpos,0.,0.), logicLiF,"physLiF",logicWorld,false,2*i+1,true);
	}
}

//The Construct function where we define the material of the detector and define the physical and logical volume of the environment and detector.
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	//Defining the dimenstions of the world environment	
	solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld);

	//Setiing Logical volume where we integrate the material to the world box	
	logicWorld= new G4LogicalVolume(solidWorld,worldMat,"logicWorld");
	
	//Integrating the position of the world to the logical volume to create our final Physical Volume	
	physWorld = new G4PVPlacement(0,G4ThreeVector(0., 0., 0.),logicWorld,"physWorld",0,false,100,true);
	
	//Define the rest of setup 
	ConstructSetup();
	
	//Finally we return the physWorld as output	
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField(){
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	//logicdetVol->SetSensitiveDetector(sensDet);
}
