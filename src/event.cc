#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* run)
{
	fRun = run;
	fEdep = 0.;
	num=0;
}	

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event* anEvent)
{
	num=0;
	fEdep = 0.;
	evID = anEvent->GetEventID();
	if(evID%10000==0){
		G4cout<<"event: "<<evID<<G4endl;
	}
}

void MyEventAction::EndOfEventAction(const G4Event* anEvent)
{	
	evID = anEvent->GetEventID();	
}
