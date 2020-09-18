#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class PrimaryGeneratorAction;

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction * aPrimaryGeneratorAction);
    ~PrimaryGeneratorMessenger();

  public:
    void SetNewValue(G4UIcommand * command,G4String newValues);
    G4String GetCurrentValue(G4UIcommand * command);

  private:
    PrimaryGeneratorAction* fPrimaryGenerator;

  private: //commands
    G4UIdirectory * fDirectory;
    #ifdef WITHROOT
    G4UIcmdWithABool * fReadInputCmd;
    G4UIcmdWithAString * fPathInputCmd;
    #endif
    G4UIcmdWithADouble * fMomentumSpreadCmd;
    G4UIcmdWithAString * fBeamSpreadTypeCmd;
    G4UIcmdWithADoubleAndUnit * fBeamSpreadXCmd;
    G4UIcmdWithADoubleAndUnit * fBeamSpreadYCmd;
    G4UIcmdWithADoubleAndUnit * fBeamZ0Cmd;
};

#endif