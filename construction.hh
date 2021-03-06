#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4Orb.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction();

  G4LogicalVolume *GetScoringVolume() const { return fScoringVolume;}

  virtual G4VPhysicalVolume *Construct();

private:
  virtual void ConstructSDandField();

  G4int nCols, nRows;
  G4double xWorld, yWorld, zWorld;
  G4bool isCherenkov, isScintillator, isFleshArray;

  G4Box *solidWorld, *solidRadiator, *solidDetector, *solidFRadiator, *solidFDetector;
  G4Box *solidScintillator;
  G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector, *logicScintillator, *logicFRadiator, *logicFDetector;
  G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, *physScintillator, *physFRadiator, *physFDetector;

  G4GenericMessenger *fMessenger;

  G4Material *SiO2, *H2O, *Aerogel, *worldMat, *NaI;
  G4Element *C, *Na, *I;

  G4LogicalVolume *fScoringVolume;


  void DefineMaterials();
  void ConstructCherenkov();
  void ConstructScintillator();
  void ConstructFleshArray();
};

#endif
