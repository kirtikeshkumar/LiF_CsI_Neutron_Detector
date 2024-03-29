#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{

	G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	
	G4bool IsEnter = step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary;
	G4bool IsKill = step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary;
	
	const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	
	G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
	
	if(step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber() == 8){
		fEventAction->AddNum(1);
	}
	
	if(volume != fScoringVolume){
		return;
	}
	
	
	G4double edep = step->GetTotalEnergyDeposit();
	fEventAction->AddEdep(edep);
	/*
	 
	*/
	
}
