#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name): 
G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory 
*ROhist)
{
	G4Track *track =aStep->GetTrack();
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	
	G4int evt= G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		
	return true;
}
