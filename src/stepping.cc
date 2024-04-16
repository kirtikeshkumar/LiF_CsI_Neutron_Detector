#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	G4int copyNo = step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber();
	G4int evID = fEventAction->GetEvID();
	G4Track *track =step->GetTrack();
	G4String particleName = track->GetParticleDefinition()->GetParticleName();
	G4String VolumeName;
	G4int parent = track->GetParentID();
	G4String creatorProcess;
	if(track->GetCreatorProcess()){
		creatorProcess = track->GetCreatorProcess()->GetProcessName();
	}else if(parent == 0){creatorProcess = "primary";}
	
	G4double edep = step->GetTotalEnergyDeposit();
	
	if(edep >= 0.00025*eV and copyNo >= 0){
		if(step->GetPostStepPoint()->GetPhysicalVolume()){
			VolumeName = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
		}else{VolumeName = "physBoundary";}
		//G4cout<<evID<<" "<<edep<<" "<<copyNo<<" "<<VolumeName<<" "<<particleName<<" "<<step->GetPostStepPoint()->GetPosition().x()/m<<G4endl;
		man->FillNtupleIColumn(0, 0, evID);
		man->FillNtupleIColumn(0, 1, copyNo);
		man->FillNtupleSColumn(0, 2, VolumeName.substr(4));
		man->FillNtupleSColumn(0, 3, particleName);
		man->FillNtupleIColumn(0, 4, parent);
		man->FillNtupleSColumn(0, 5, creatorProcess);
		man->FillNtupleDColumn(0, 6, edep);
		man->AddNtupleRow(0);
	}
}
