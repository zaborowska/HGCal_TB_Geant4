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
/// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#ifdef MATSCAN
#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
class EventAction;

/**
 * @brief Stepping action
 * 
 * Compiled only if -DMATSCAN=1 flag was used with CMake. 
 *
 * Registers what materials particle encounter on its trajectory and stores
 * them in an event action. It is meant to * be used with geantinos and for
 * single-particle runs.
 * 
 */

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction*);
  virtual ~SteppingAction();

  /// Read information of material and the step length
  virtual void UserSteppingAction(const G4Step* step);

private:
  /// Pointer to the event action
  EventAction* fEventAction;
};

#endif /* STEPPINGACTION_HH */
#endif /* MATSCAN */