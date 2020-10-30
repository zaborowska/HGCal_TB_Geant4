//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ***********************
#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4GenericMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#ifdef MATSCAN
class G4Material;
#endif

/**
 * @brief Event action class
 *
 * Fills ntuples with information about hits in sensitive detectors.
 *
 */

class EventAction : public G4UserEventAction {
public:
  EventAction();
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event *event);
  virtual void EndOfEventAction(const G4Event *event);

  /// Vector of primary particles in an event: PDG type
  std::vector<G4int> primaries_PDG;
  /// Vector of primary particles in an event: particle energy (in GeV)
  std::vector<G4double> primaries_energy;
  /// Vector of primary particles in an event: vertex position x (in cm)
  std::vector<G4double> primaries_x;
  /// Vector of primary particles in an event: vertex position y (in cm)
  std::vector<G4double> primaries_y;
  /// Vector of primary particles in an event: vertex position z (in cm)
  std::vector<G4double> primaries_z;

  /// Vector of hits in silicon sensors: hit ID
  std::vector<G4int> Si_hits_ID;
  /// Vector of hits in silicon sensors: hit position x (in cm)
  std::vector<G4double> Si_hits_x;
  /// Vector of hits in silicon sensors: hit position y (in cm)
  std::vector<G4double> Si_hits_y;
  /// Vector of hits in silicon sensors: hit position z (in cm)
  std::vector<G4double> Si_hits_z;
  /// Vector of hits in silicon sensors: hit energy (in keV)
  std::vector<G4double> Si_hits_Edep;
  /// Vector of hits in silicon sensors: hit non-ionizing energy (in keV)
  std::vector<G4double> Si_hits_EdepNonIonising;
  /// Vector of hits in silicon sensors: hit time of arrival (in ns)
  /// calculated as global time of energy deposit which added to hit energy
  /// exceeds the toa threshold (by default threshold is 0, so it is the first
  /// deposit)
  std::vector<G4double> Si_hits_TOA;
  /// Vector of hits in silicon sensors: hit time of last arrival (in ns)
  /// calculated as global time of energy deposit which is the last deposit
  /// that fits within the digitisation time window (by default window is
  /// undefined, so it is the last deposit)
  std::vector<G4double> Si_hits_TOA_last;
  /// Vector of hits in silicon sensors: hit type
  /// Simulation defines only hits of type 0 (hexagonal cells, no calibration or
  /// edge cell is constructed)
  std::vector<G4int> Si_hits_type;

  /// Vector of hits in SiPM: hit ID
  std::vector<G4int> SiPM_hits_ID;
  /// Vector of hits in SiPM: hit position x (in cm)
  std::vector<G4double> SiPM_hits_x;
  /// Vector of hits in SiPM: hit position y (in cm)
  std::vector<G4double> SiPM_hits_y;
  /// Vector of hits in SiPM: hit position z (in cm)
  std::vector<G4double> SiPM_hits_z;
  /// Vector of hits in SiPM: hit energy (in keV)
  std::vector<G4double> SiPM_hits_Edep;
  /// Vector of hits in SiPM: hit non-ionizing energy (in keV)
  std::vector<G4double> SiPM_hits_EdepNonIonising;
  /// Vector of hits in SiPM: hit time of last arrival (in ns)
  /// calculated as global time of energy deposit which is the last deposit
  /// that fits within the digitisation time window (by default window is
  /// undefined, so it is the last deposit)
  std::vector<G4double> SiPM_hits_TOA;
  /// Vector of hits in silicon sensors: hit type
  /// Simulation defines only hits of type 1
  std::vector<G4int> SiPM_hits_type;

#ifdef MATSCAN
  /// Add information of the step material for the material scan study
  /// @param[in] aStepLength Length of the step
  /// @param[in] aMaterial Pointer to the material to retrieve its properties
  void AddStep(G4double aStepLength, G4Material *aMaterial);
  /// Vector of encountered materials: thickness/depth expressed in units of
  /// radiation length
  std::vector<G4double> material_nX0;
  /// Vector of encountered materials: thickness/depth expressed in units of
  /// nuclear interaction length
  std::vector<G4double> material_nLambda;
  /// Vector of encountered materials: thickness/depth expressed in mm
  std::vector<G4double> material_depth;
  /// Vector of encountered materials: name encoded as integer
  std::vector<G4int> material_name;
  /// Map of material names to integer key value which can be stored in an
  /// ntuple. Map is stored at the end of run in a text file to allow decoding
  std::map<G4String, G4int> material_names_map;
#endif

private:
  /// Define UI commands: digitisation of hits with the time cut on deposits
  /// (time window), and the energy threshold for the first deposit counted as
  /// the time of arrival
  void DefineCommands();
  /// Pointer to the messenger for UI commands
  G4GenericMessenger *fMessenger = nullptr;
  /// Time window for hit digitisation (in ns)
  /// By default undefined window indicates the last created deposit will set up
  /// the time
  /// Can be changed by the UI command /HGCalTestbeam/hits/timeCut
  G4double hitTimeCut = -1;
  /// Time of arrival threshold (in keV)
  /// Default value of 0 indicates the first created deposit will set up the
  /// time, independent on the amount of deposited energy
  /// Can be changed by the UI command /HGCalTestbeam/hits/toaThreshold
  G4double toaThreshold = 0;
};

#endif /* EVENTACTION_HH */
