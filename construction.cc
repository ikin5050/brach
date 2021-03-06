#include "construction.hh"
#include "G4SDManager.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
  fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

  fMessenger->DeclareProperty("nCols", nCols, "# columns");
  fMessenger->DeclareProperty("nRows", nRows, "# rows");
  fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov Setup");
  fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator Setup");
  fMessenger->DeclareProperty("isFleshArray", isFleshArray, "Toggle Flesh Array Setup");

  nCols = 100;
  nRows = 100;

  DefineMaterials();

  xWorld = 0.5*m;
  yWorld = 0.5*m;
  zWorld = 0.5*m;

  isCherenkov = false;
  isScintillator = false;
  isFleshArray = true;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{ 
  G4NistManager *nist = G4NistManager::Instance();

  SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
  SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
  SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

  H2O = new G4Material("H20", 1*g/cm3, 2);
  H2O->AddElement(nist->FindOrBuildElement("H"), 2);
  H2O->AddElement(nist->FindOrBuildElement("O"), 1);

  C = nist->FindOrBuildElement("C");

  Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
  Aerogel->AddMaterial(SiO2, 0.625);
  Aerogel->AddMaterial(H2O, 0.374);
  Aerogel->AddElement(C, 0.001);

  Na = nist->FindOrBuildElement("Na");
  I = nist->FindOrBuildElement("I");
  NaI = new G4Material("NaI", 3.67*g/cm3, 2);
  NaI->AddElement(Na, 1);
  NaI->AddElement(I, 1);

  worldMat = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");

  G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
  G4double rindexAerogel[2] = {1.1, 1.1};

  G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
  mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);

  Aerogel->SetMaterialPropertiesTable(mptAerogel);
}


void MyDetectorConstruction::ConstructCherenkov()
{
  G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
  G4double rindexWorld[2] = {1.0, 1.0};
  G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
  mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

  worldMat->SetMaterialPropertiesTable(mptWorld);

  solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);

  logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");

  fScoringVolume = logicRadiator;

  physRadiator = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);

  solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.01*m);

  logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

  for(G4int i=0; i < nRows; i++)
    {
      for(G4int j = 0; j < nCols; j++)
	{
	  physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nRows, -0.5*m+(j+0.5)*m/nCols, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*nRows, true);
	}
    }
}

void MyDetectorConstruction::ConstructScintillator()
{
  solidScintillator = new G4Box("solidScintillator", xWorld-1*cm, yWorld-1*cm, zWorld-1*cm);
  
  logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicScintillator");

  fScoringVolume = logicScintillator;
  
  physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);
}


void MyDetectorConstruction::ConstructFleshArray()
{
  solidFRadiator = new G4Box("solidFRadiator", xWorld-1*cm, yWorld-1*cm, zWorld-1*cm);

  logicFRadiator = new G4LogicalVolume(solidFRadiator, worldMat, "logicFRadiator");

  fScoringVolume = logicFRadiator;

  physFRadiator = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicFRadiator, "physFRadiator", logicWorld, false, 0, true);

  solidFDetector = new G4Box("solidFDetector", xWorld/nRows, yWorld/nCols, 0.01*m);

  logicFDetector = new G4LogicalVolume(solidFDetector, worldMat, "logicFDetector");

  for(G4int i=0; i < nRows; i++)
    {
      for(G4int j = 0; j < nCols; j++)
	{
	  physFDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nRows, -0.5*m+(j+0.5)*m/nCols, 0.*m), logicFDetector, "physFDetector", logicWorld, false, j+i*nRows, true);
	}
    }
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
  solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);

  logicWorld = new G4LogicalVolume(solidWorld, worldMat,"logicWorld");
  
  physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

  if(isCherenkov)
    ConstructCherenkov();

  if(isScintillator)
    ConstructScintillator();

  if(isFleshArray)
    ConstructFleshArray();
  
  return physWorld;
  
}


void MyDetectorConstruction::ConstructSDandField()
{
  MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

  if(isCherenkov)
    logicDetector->SetSensitiveDetector(sensDet);
}
