#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0.;
	num=0;
}	

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event* anEvent)
{
	num=0;
	fEdep = 0.;
	G4int evID = anEvent->GetEventID();
	if(evID%10000==0){
		G4cout<<"event: "<<evID<<G4endl;
	}
}

void MyEventAction::EndOfEventAction(const G4Event* anEvent)
{
	/*if((fEdep-0.0)<=1E-9){
		G4cout << "Energy deposition: "<< fEdep << G4endl;
	}*/
	
	G4int evID = anEvent->GetEventID();
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	if(num!=0){
		G4cout << "Number of Particles in Event: "<< evID << " is "<< num << G4endl;
	}
	//man->FillNtupleDColumn(0, 0, fEdep);
	
	//man->AddNtupleRow(0);
	
	
}
