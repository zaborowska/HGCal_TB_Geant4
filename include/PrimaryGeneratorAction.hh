#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;
class PrimaryGeneratorMessenger;

#ifdef WITHROOT
#include "TTreeReaderValue.h"
class TFile;
class TTreeReader;
#endif

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    void OpenInput();

    #ifdef WITHROOT
    void SetIfUseInputFiles(G4bool aUseInputFiles) {readInputFile = aUseInputFiles;};
    inline G4bool GetIfUseInputFiles() {return readInputFile;};
    void SetInputFiles(G4String aInputFiles) {pathInputFile = aInputFiles;};
    inline G4String GetInputFiles() const {return pathInputFile;};
    #endif
    void SetMomentumSpread(G4double aMomentumSpread) {momentumGaussianSpread = aMomentumSpread;};
    inline G4double GetMomentumSpread() const {return momentumGaussianSpread;};
    void SetBeamSpreadType(G4String aType) {
      if (aType == "none") beamType = eNone;
      if (aType == "Gaussian") beamType = eGaussian;
      if (aType == "flat") beamType = eFlat;
    };
    inline G4String GetBeamSpreadType() const {
      switch(beamType){
        case eNone: return "none"; break;
        case eGaussian: return "Gaussian"; break;
        case eFlat: return "flat"; break;
        }
        return "";
      };
    void SetBeamSpreadX(G4double aBeamSpreadX) {sigmaBeamX = aBeamSpreadX;};
    inline  G4double GetBeamSpreadX() const {return sigmaBeamX;};
    void SetBeamSpreadY(G4double aBeamSpreadY) {sigmaBeamY = aBeamSpreadY;};
    inline  G4double GetBeamSpreadY() const {return sigmaBeamY;};
    void SetBeamZ0(G4double aBeamZ0) {beamZ0 = aBeamZ0;};
    inline G4double GetBeamZ0() const {return beamZ0;};
    

  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4Box* fEnvelopeBox;

    PrimaryGeneratorMessenger* fMessenger;

    enum eBeamType{eNone, eGaussian, eFlat};
    eBeamType beamType;
    G4double sigmaBeamX;
    G4double sigmaBeamY;
    G4double beamZ0;
    G4double momentumGaussianSpread;

    #ifdef WITHROOT
    G4bool readInputFile = false;
    G4String pathInputFile = "Electron_v44_Nikosbeam_30.GeV_4729.root";
    TFile* fInputFile = nullptr;
    TTreeReader* fHgcalReader = nullptr;
    TTreeReaderValue<Float_t>* fHgcalEventId;
    TTreeReaderValue<Float_t>* fHgcalPdgId;
    TTreeReaderValue<Float_t>* fHgcalPosX;
    TTreeReaderValue<Float_t>* fHgcalPosY;
    TTreeReaderValue<Float_t>* fHgcalPosZ;
    TTreeReaderValue<Float_t>* fHgcalMomX;
    TTreeReaderValue<Float_t>* fHgcalMomY;
    TTreeReaderValue<Float_t>* fHgcalMomZ;
    #endif
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
