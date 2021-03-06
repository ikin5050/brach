#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  fEdep = 0.;
  fGammaDep = 0.;
  x = 0;
  y = 0;
}

MyEventAction::~MyEventAction()
{}

//add edep to index x,y in array

void MyEventAction::AddAEdep(const G4double edep, G4int posX, G4int posY)
{
  array[posX][posY] += edep;
  x = posX;
  y = posY;
  
}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
  fGammaDep = 0.;
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 100; j++){
      array[i][j] = 0;
	}
  }
  x = 0;
  y = 0;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  for(int row = 0; x<100; x++){
    for(int column = 0; y<100; y++){
      man->FillH2(0, x, y, array[row][column]);
    }
  }
  man->FillNtupleDColumn(0, 0, fEdep);
  man->FillNtupleDColumn(0, 1, fGammaDep);

  man->AddNtupleRow(0);
}
