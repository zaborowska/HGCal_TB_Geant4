#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include "G4GenericMessenger.hh"
#ifdef MATSCAN
class G4Material;
#endif

/// Event action class
///

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    std::vector<G4int>        hits_ID;
    std::vector<G4double>     hits_x;
    std::vector<G4double>     hits_y;
    std::vector<G4double>     hits_z;
    std::vector<G4double>     hits_Edep;
    std::vector<G4double>     hits_EdepNonIonising;
    std::vector<G4double>     hits_TOA;
    std::vector<G4double>     hits_TOA_last;
    std::vector<G4int>        hits_type;
    #ifdef MATSCAN
    void AddStep(G4double aStepLength, G4Material* aMaterial);
    std::vector<G4double>     material_nX0;
    std::vector<G4double>     material_nLambda;
    std::vector<G4double>     material_depth;
    std::vector<G4int>        material_name;
    std::map<G4String, G4int> material_names_map;
    #endif
private:
    void DefineCommands();
    G4GenericMessenger* fMessenger;
    G4double hitTimeCut;
    G4double toaThreshold;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


