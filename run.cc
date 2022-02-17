#include "run.hh"

MyRunAction::MyRunAction()
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  //man->CreateNtuple("Hits", "Hits");
  //man->CreateNtupleIColumn("fEvent");
  //man->CreateNtupleDColumn("fX");
  //man->CreateNtupleDColumn("fY");
  //man->CreateNtupleDColumn("fZ");
  //man->FinishNtuple(0);

  man->CreateNtuple("Scoring", "Scoring");
  man->CreateNtupleDColumn("fEdep");
  man->CreateNtupleDColumn("fGammaDep");
  //man->CreateNtupleDColumn("fElDep");
  man->FinishNtuple(0);

  //initialize H2 histogram
  man->SetVerboseLevel(1);
  man->CreateH2("10", "Energy, pos", 300, 0., 1500., 300, 0., 100);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
  //create new output file for every run
  G4AnalysisManager *man = G4AnalysisManager::Instance();
 
  G4int runID = run->GetRunID();

  std::stringstream strRunID;
  strRunID << runID;
  
  man->OpenFile("output"+strRunID.str()+".root"); 
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
  //write to histogram
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->Write();
  man->CloseFile();

}
