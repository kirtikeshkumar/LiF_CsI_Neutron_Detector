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
	
	G4double particleTime = (step->GetPostStepPoint()->GetGlobalTime())/ms;
	
	G4ThreeVector PreStepPos = step->GetPreStepPoint()->GetPosition();
	G4ThreeVector PostStepPos = step->GetPostStepPoint()->GetPosition();
	G4double StepDistance = ((PreStepPos-PostStepPos).mag())/um;
	G4ThreeVector creationPos = step->GetTrack()->GetVertexPosition();
	G4double netDisplacement = ((PostStepPos-creationPos).mag())/um;
	
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
		man->FillNtupleDColumn(0, 7, particleTime);
		man->FillNtupleDColumn(0, 8, StepDistance);
		man->FillNtupleDColumn(0, 9, netDisplacement);
		
		man->AddNtupleRow(0);
	}
}
