#include "run.hh"

MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->CreateNtuple("E Deposition","E Deposition");
	man->CreateNtupleIColumn("fEvtNo");
	man->CreateNtupleIColumn("fCopyNo");
	man->CreateNtupleSColumn("fMaterial");
	man->CreateNtupleSColumn("fParticle");
	man->CreateNtupleIColumn("fParentID");
	man->CreateNtupleSColumn("fProdProcess");
	man->CreateNtupleDColumn("fEnergyDep");
	man->FinishNtuple(0);
	
	//man->CreateNtuple("CsI","CsI");
	//man->CreateNtupleIColumn("fEvtNo");
	//man->CreateNtupleIColumn("fCopyNo");	
	//man->CreateNtupleSColumn("fParticle");
	//man->CreateNtupleIColumn("fParentID");
	//man->CreateNtupleSColumn("fProdProcess");
	//man->CreateNtupleDColumn("fEnergyDep");
	//man->FinishNtuple(1);	
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	G4int runID = run->GetRunID();
	
	std::stringstream strRunID;
	strRunID << runID;
	
	man->OpenFile("../outputs/output"+strRunID.str()+".root");	
	
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	G4cout<<"Run Finished"<<G4endl;
	
	man->Write();
	man->CloseFile();
}
