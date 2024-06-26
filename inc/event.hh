#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"
//#include "g4root.hh"
#include "run.hh"

class MyEventAction : public G4UserEventAction
{
public:
	MyEventAction(MyRunAction*);
	~MyEventAction();
	
	virtual void BeginOfEventAction(const G4Event*);
	virtual void EndOfEventAction(const G4Event*);
	
	void AddEdep(G4double edep){ fEdep += edep; }
	void AddNum(G4int a){num+=a;}
	G4int GetEvID(){return evID;}
private:
	MyRunAction* fRun;
	G4double fEdep;
	G4int evID;
	G4int num;
};

#endif
