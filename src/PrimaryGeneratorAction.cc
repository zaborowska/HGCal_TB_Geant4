
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Threading.hh"

#ifdef WITHROOT
#include "TFile.h"
#include "TTreeReader.h"
#include "g4root.hh"
#include "G4MTRunManager.hh"
#include "G4Run.hh"
#endif
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(nullptr),
  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e+");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(30.*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));

  sigmaBeamX = 0;
  sigmaBeamY = 0;
  beamZ0 = -999 * m;
  momentumGaussianSpread = 0;

  fMessenger = new PrimaryGeneratorMessenger(this);
  if (readInputFile) OpenInput();
}

void PrimaryGeneratorAction::OpenInput() {
  // first check if job is run in either single-threaded mode, or with 1 thread
  // TODO modify below to read one file per thread
  // TODO figure out how to change analysis output path (maybe as post sim step?)
  #ifdef G4MULTITHREADED
  if (G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads() > 1)
  {
    G4ExceptionDescription msg;
    msg << "Currently MT mode with >1 threads is not supported.\n";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "NoMT", FatalErrorInArgument, msg);
  }
  #endif
  fInputFile = new TFile(pathInputFile, "READ");
  if (fInputFile == nullptr || fInputFile->IsZombie())
  {
    G4ExceptionDescription msg;
    msg << "Input file '" << pathInputFile << "' cannot be opened.\n";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "WrongInputName", FatalErrorInArgument, msg);
  }
  auto fileDirectory = dynamic_cast<TDirectory *>(fInputFile->Get("VirtualDetector"));
  if (fileDirectory == nullptr)
  {
    G4ExceptionDescription msg;
    msg << "Input file '" << pathInputFile << "' does not contain 'VirtualDetector' directory.\n";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "WrongInputDirectory", FatalErrorInArgument, msg);
  }
  fHgcalReader = new TTreeReader("HGCAL", fileDirectory);
  fHgcalEventId = new TTreeReaderValue<Float_t>(*fHgcalReader, "EventID");
  fHgcalPdgId = new TTreeReaderValue<Float_t>(*fHgcalReader, "PDGid");
  fHgcalPosX = new TTreeReaderValue<Float_t>(*fHgcalReader, "x");
  fHgcalPosY = new TTreeReaderValue<Float_t>(*fHgcalReader, "y");
  fHgcalPosZ = new TTreeReaderValue<Float_t>(*fHgcalReader, "z");
  fHgcalMomX = new TTreeReaderValue<Float_t>(*fHgcalReader, "Px");
  fHgcalMomY = new TTreeReaderValue<Float_t>(*fHgcalReader, "Py");
  fHgcalMomZ = new TTreeReaderValue<Float_t>(*fHgcalReader, "Pz");

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  #ifdef WITHROOT
  delete fInputFile;
  #endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  #ifdef WITHROOT
  if (readInputFile)
  {
    // get primary particles from input file
    if( fInputFile == nullptr ) {
      OpenInput();
    }
    Float_t currentEventId = -1;
    while(fHgcalReader->Next()) {
      // First particle - set event ID
      if (currentEventId == -1) currentEventId = **fHgcalEventId;
      else // check if current particle belongs still to the same event
      if (currentEventId != **fHgcalEventId) {break;}
      
      auto vertex = new G4PrimaryVertex(G4ThreeVector(**fHgcalPosX , **fHgcalPosY, 32.5), 0*s);
      auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(**fHgcalPdgId);
      auto particle = new G4PrimaryParticle(particleDefinition);
      G4ThreeVector momentum(**fHgcalMomX , **fHgcalMomY, **fHgcalMomZ);
      particle->SetMomentumDirection(momentum.unit());    
      particle->SetKineticEnergy(momentum.mag());
      vertex->SetPrimary(particle);
      anEvent->AddPrimaryVertex(vertex);
    }
    // if no particles are left - terminate run
    if (anEvent->GetNumberOfPrimaryVertex() == 0)
    {
      G4ExceptionDescription msg;
      msg << "Input file does not contain any more events (current event ID = " << anEvent->GetEventID() << ").\n";
      msg << "Run will be aborted with " << anEvent->GetEventID() << " events processed.\n";
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "EndOfInputFile", JustWarning, msg);
      G4RunManager::GetRunManager()->AbortRun();
    }
    // warn if this is the last event and some events are left in the file unprocessed
    if (G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed() == anEvent->GetEventID() + 1)
    {
      G4ExceptionDescription msg;
      msg << "Input file contains more events than requested in the run.\n";
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "UnusedEventsInInputFile", JustWarning, msg);
    }
  }
  else
  #endif
  {
    G4cout << "Particle generator: particle gun" << G4endl;
    // Beam position: z
    // Use value set by UI command, or set it to edge of the world volume
    G4double z0 = beamZ0;
    if (beamZ0 == -999 * m)
    {
      G4double worldDZ = 0;
      if (!fEnvelopeBox)
      {
        G4LogicalVolume *envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
        if (envLV)
          fEnvelopeBox = dynamic_cast<G4Box *>(envLV->GetSolid());
      }
      if (fEnvelopeBox)
      {
        worldDZ = fEnvelopeBox->GetZHalfLength();
        z0 = -worldDZ;
      }
      else
      {
        G4ExceptionDescription msg;
        msg << "World volume of box shape not found.\n";
        msg << "Perhaps you have changed geometry.\n";
        msg << "The gun will be place at the center.";
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "WorldVolume", JustWarning, msg);
      }
    }
    // Beam position: x,y
    switch (beamType)
    {
    case eNone:
    default:
      fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, z0));
      break;
    case eGaussian:
      fParticleGun->SetParticlePosition(G4ThreeVector(G4RandGauss::shoot(0., sigmaBeamX), G4RandGauss::shoot(0., sigmaBeamY), z0));
      break;
    case eFlat:
      fParticleGun->SetParticlePosition(G4ThreeVector(G4RandFlat::shoot(-sigmaBeamX, sigmaBeamX), G4RandFlat::shoot(-sigmaBeamY, sigmaBeamY), z0));
      break;
    }
    // Particle momentum
    if (momentumGaussianSpread > 0)
    {
      double energy = fParticleGun->GetParticleEnergy();
      energy += G4RandGauss::shoot(0., momentumGaussianSpread) * energy;
      fParticleGun->SetParticleEnergy(energy);
    }
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  // for(int i=0; i<anEvent->GetNumberOfPrimaryVertex();i++) 
  //   anEvent->GetPrimaryVertex(i)->Print();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

