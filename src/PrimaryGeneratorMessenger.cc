#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction * aPrimaryGeneratorAction)
:fPrimaryGenerator(aPrimaryGeneratorAction)
{
  fDirectory = new G4UIdirectory("/Simulation/generator/");
  fDirectory->SetGuidance("Primary generator control commands.");

#ifdef WITHROOT
  fReadInputCmd = new G4UIcmdWithABool("/Simulation/generator/readInputFile",this);
  fReadInputCmd->SetGuidance("If instead of particle gun, particle data should be read from file.");
  fReadInputCmd->SetParameterName("ifInput", true);
  fReadInputCmd->SetDefaultValue(false);

  fPathInputCmd = new G4UIcmdWithAString("/Simulation/generator/pathInputFile",this);
  fPathInputCmd->SetGuidance("Path to input file containing particle data.");
  fPathInputCmd->SetParameterName("path",true);

  fStartFromEventCmd = new G4UIcmdWithAnInteger("/Simulation/generator/startFromEvent",this);
  fStartFromEventCmd->SetGuidance("From which event in the file simulation should be started.");
  fStartFromEventCmd->SetParameterName("startFrom",true);
  fStartFromEventCmd->SetDefaultValue(0);
#endif
  fMomentumSpreadCmd = new G4UIcmdWithADouble("/Simulation/generator/momentumSpread",this);
  fMomentumSpreadCmd->SetGuidance("For particle gun generator:");
  fMomentumSpreadCmd->SetGuidance("Gaussian momentum spread relative to gun energy");
  fMomentumSpreadCmd->SetGuidance("(e.g. 0.05 means 5% * gun energy))");
  fMomentumSpreadCmd->SetParameterName("momentumSpread",true);
  fMomentumSpreadCmd->SetRange("momentumSpread>=0");
  fMomentumSpreadCmd->SetDefaultValue(0);

  fBeamSpreadTypeCmd = new G4UIcmdWithAString("/Simulation/generator/beamSpread",this);
  fBeamSpreadTypeCmd->SetGuidance("Type of beam profile spread.");
  fBeamSpreadTypeCmd->SetParameterName("beamSpreadType",true);
  fBeamSpreadTypeCmd->SetCandidates("none Gaussian flat");
  fBeamSpreadTypeCmd->SetDefaultValue("none");

  fBeamSpreadXCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/generator/beamSpreadX",this);
  fBeamSpreadXCmd->SetGuidance("Defines sigma_X for Gaussian spread, or half-side range for flat spread.");
  fBeamSpreadXCmd->SetParameterName("sigmaBeamX",true,true);
  fBeamSpreadXCmd->SetRange("sigmaBeamX>=0");
  fBeamSpreadXCmd->SetDefaultUnit("cm");
  fBeamSpreadXCmd->SetUnitCandidates("micron mm cm m km");

  fBeamSpreadYCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/generator/beamSpreadY",this);
  fBeamSpreadYCmd->SetGuidance("Defines sigma_Y for Gaussian spread, or half-side range for flat spread.");
  fBeamSpreadYCmd->SetParameterName("sigmaBeamY",true,true);
  fBeamSpreadYCmd->SetRange("sigmaBeamY>=0");
  fBeamSpreadYCmd->SetDefaultUnit("cm");
  fBeamSpreadYCmd->SetUnitCandidates("micron mm cm m km");

  fBeamZ0Cmd = new G4UIcmdWithADoubleAndUnit("/Simulation/generator/beamZ0",this);
  fBeamZ0Cmd->SetGuidance("Beam position along the beam line (default: edge of the world volume).");
  fBeamZ0Cmd->SetParameterName("z0",true,true);
  fBeamZ0Cmd->SetDefaultUnit("cm");
  fBeamZ0Cmd->SetUnitCandidates("micron mm cm m km");
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete fDirectory;
    #ifdef WITHROOT
    delete fReadInputCmd;
    delete fPathInputCmd;
    delete fStartFromEventCmd;
    #endif
    delete fMomentumSpreadCmd;
    delete fBeamSpreadTypeCmd;
    delete fBeamSpreadXCmd;
    delete fBeamSpreadYCmd;
    delete fBeamZ0Cmd;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
    if (command == fMomentumSpreadCmd)
    {
        fPrimaryGenerator->SetMomentumSpread(fMomentumSpreadCmd->GetNewDoubleValue(newValues));
    }
    else if (command == fBeamSpreadTypeCmd)
    {
        fPrimaryGenerator->SetBeamSpreadType(newValues);
    }
    else if (command == fBeamSpreadXCmd)
    {
        fPrimaryGenerator->SetBeamSpreadX(fBeamSpreadXCmd->GetNewDoubleValue(newValues));
    }
    else if (command == fBeamSpreadYCmd)
    {
        fPrimaryGenerator->SetBeamSpreadY(fBeamSpreadYCmd->GetNewDoubleValue(newValues));
    }
    else if (command == fBeamZ0Cmd)
    {
        fPrimaryGenerator->SetBeamZ0(fBeamZ0Cmd->GetNewDoubleValue(newValues));
    }
    #ifdef WITHROOT
    else if (command == fReadInputCmd)
    {
        fPrimaryGenerator->SetIfUseInputFiles(fReadInputCmd->GetNewBoolValue(newValues));
    }
    else if (command == fPathInputCmd)
    {
        fPrimaryGenerator->SetInputFiles(newValues);
    }
    else if (command == fStartFromEventCmd)
    {
        fPrimaryGenerator->SetStartFromEvent(fStartFromEventCmd->GetNewIntValue(newValues));
    }
    #endif 
}

G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand *command)
{
    G4String cv;
    if (command == fMomentumSpreadCmd)
    {
        cv = fMomentumSpreadCmd->ConvertToString(fPrimaryGenerator->GetMomentumSpread());
    }
    else if (command == fBeamSpreadTypeCmd)
    {
        cv = fPrimaryGenerator->GetBeamSpreadType();
    }
    else if (command == fBeamSpreadXCmd)
    {
        cv = fBeamSpreadXCmd->ConvertToString(fPrimaryGenerator->GetBeamSpreadX());
    }
    else if (command == fBeamSpreadYCmd)
    {
        cv = fBeamSpreadYCmd->ConvertToString(fPrimaryGenerator->GetBeamSpreadY());
    }
    else if (command == fBeamZ0Cmd)
    {
        cv = fBeamZ0Cmd->ConvertToString(fPrimaryGenerator->GetBeamZ0());
    }
    #ifdef WITHROOT
    else if (command == fReadInputCmd)
    {
        cv = fReadInputCmd->ConvertToString(fPrimaryGenerator->GetIfUseInputFiles());
    }
    else if (command == fPathInputCmd)
    {
        cv = fPrimaryGenerator->GetInputFiles();
    }
    #endif
    return cv;
}
