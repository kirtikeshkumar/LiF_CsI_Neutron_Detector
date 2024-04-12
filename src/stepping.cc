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
	G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
	
	G4int evID = fEventAction->GetEvID();
	
	G4Track *track =step->GetTrack();
	G4String particleName = track->GetParticleDefinition()->GetParticleName();
	G4int parent = track->GetParentID();
	G4String creatorProcess;
	if(track->GetCreatorProcess()){
		creatorProcess = track->GetCreatorProcess()->GetProcessName();
	}else if(parent == 0){creatorProcess = "primary";}
	
	
	G4double edep = step->GetTotalEnergyDeposit();
	
	if(copyNo % 2 == 0 and edep != 0.0){								//Energy deposit in CsI
		man->FillNtupleIColumn(1, 0, evID);
		man->FillNtupleIColumn(1, 1, copyNo);
		man->FillNtupleSColumn(1, 2, particleName);
		man->FillNtupleIColumn(1, 3, parent);
		man->FillNtupleSColumn(1, 4, creatorProcess);
		man->FillNtupleDColumn(1, 5, edep);
		man->AddNtupleRow(1);
	}
	
	if(copyNo % 2 == 1 and edep != 0.0){								//Energy deposit in LiF
		//G4cout<<" "<<evID<<" "<<copyNo<<" "<<particleName<<" "<<parent<<" "<<creatorProcess<<" "<<edep<<G4endl;
		man->FillNtupleIColumn(0, 0, evID);
		man->FillNtupleIColumn(0, 1, copyNo);
		man->FillNtupleSColumn(0, 2, particleName);
		man->FillNtupleIColumn(0, 3, parent);
		man->FillNtupleSColumn(0, 4, creatorProcess);
		man->FillNtupleDColumn(0, 5, edep);
		man->AddNtupleRow(0);
	}
}
