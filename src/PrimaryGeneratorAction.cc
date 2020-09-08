
#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


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
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="mu+");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(120.*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));

  sigmaBeamX = .1 * mm;
  sigmaBeamY = .1 * mm;
  beamZ0 = -999 * m;
  beamTypeGaussian = false;
  momentumGaussianSpread = 0;

  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::DefineCommands() {
  fMessenger 
    = new G4GenericMessenger(this, 
                             "/Simulation/generator/", 
                             "Primary generator control");

  // momentum Gaussian spread command
  auto& momentumSpreadCmd
    = fMessenger->DeclareProperty("momentumSpread", momentumGaussianSpread,
        "Gaussian momentum spread relative to gun energy (e.g. 0.05 means 5% * gun energy))");
  momentumSpreadCmd.SetParameterName("momentumSpread", true);
  momentumSpreadCmd.SetRange("momentumSpread>=0.");
  momentumSpreadCmd.SetDefaultValue("0");

  // beam spread in x command
  auto& beamSpreadXCmd
    = fMessenger->DeclarePropertyWithUnit("sigmaBeamX", "cm", sigmaBeamX, 
        "Gaussian beam spread in X, half-side range in X for flat gun");
  beamSpreadXCmd.SetParameterName("sigmaBeamX", true);
  beamSpreadXCmd.SetRange("sigmaBeamX>0.");                                
  beamSpreadXCmd.SetDefaultValue(".001");

  // beam spread in y command
  auto& beamSpreadYCmd
    = fMessenger->DeclarePropertyWithUnit("sigmaBeamY", "cm", sigmaBeamY, 
        "Gaussian beam spread in Y, half-side range in > for flat gun");
  beamSpreadYCmd.SetParameterName("sigmaBeamY", true);
  beamSpreadYCmd.SetRange("sigmaBeamY>0.");                                
  beamSpreadYCmd.SetDefaultValue(".001");

  auto& gunSpreadCmd
    = fMessenger->DeclareProperty("GaussianBeam", beamTypeGaussian);
  gunSpreadCmd.SetParameterName("GaussianBeam", true);
  gunSpreadCmd.SetDefaultValue("false");

  // beam position along beam line
  auto& beamZ0Cmd
    = fMessenger->DeclarePropertyWithUnit("z0", "m", beamZ0, 
        "Beam position along the beam line");
  beamZ0Cmd.SetParameterName("z0", true);
  beamZ0Cmd.SetDefaultValue("0");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  G4double worldDZ = 0;

  if (!fEnvelopeBox)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if ( fEnvelopeBox ) {
    worldDZ = fEnvelopeBox->GetZHalfLength();
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  G4double z0 = (beamZ0==-999*m) ? -worldDZ : beamZ0;
  if (beamTypeGaussian) fParticleGun->SetParticlePosition(G4ThreeVector(G4RandGauss::shoot(0., sigmaBeamX),G4RandGauss::shoot(0., sigmaBeamY), z0));
  else fParticleGun->SetParticlePosition(G4ThreeVector(G4RandFlat::shoot(-sigmaBeamX, sigmaBeamX),G4RandFlat::shoot(-sigmaBeamY, sigmaBeamY), z0));
  if(momentumGaussianSpread > 0) {
    double energy = fParticleGun->GetParticleEnergy();
    energy += G4RandGauss::shoot(0., momentumGaussianSpread) * energy;
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

