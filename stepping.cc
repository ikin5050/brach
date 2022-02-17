#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
  fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
  //Define Track
  G4Track *track = step->GetTrack();

  //Get Touchable and copy number

  const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
  G4int copyNo = touchable->GetCopyNumber();
  
  //Get volume containing particle
  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  //Get scoring volume
  const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  
  G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

  //Compare volume with particle and volume of interest
  if(volume != fScoringVolume)
    return;

  G4double edep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edep);

  //Get secondaries

  const std::vector<const G4Track*> *secondaries = step->GetSecondaryInCurrentStep();

  // Get x and y position in detector array
  G4int x = copyNo%100;
  G4int y = (copyNo-(copyNo%100))/100;

  //Check secondaries and store gamma energy

  if (secondaries->size()>0){
    for(unsigned int i=0; i<secondaries->size(); i++){
      if (secondaries->at(i)->G4Track::GetParentID()>0){
	if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4Gamma::Definition()){
	  //somehow separate out deexcitation energy, only processes showing are comp and ionIoni

	  double gdep = secondaries->at(i)->GetTotalEnergy();
	  //Defined in event action, adds to array at index x y 
	  fEventAction->AddAEdep(gdep, x, y);
	}
      }
    }
  }
}
