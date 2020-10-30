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
// ********************************************************************
//
#include "G4Cons.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

/// Add module of silicon sensor with absorbers, readout, and support, for the
/// electromagnetic calorimeter (EE).
/// @param[in] EEid ID of module in the setup (not all modules are identical)
/// @param[out] dz_map List of element names and air gap to be placed in front
/// of the element
/// @param[in] airBefore How much air should be added before the current module
/// (before the first iron)
/// @param[in] airMid How much air should be added in the middle (before the
/// first PCB)
void add_EE(uint EEid, std::vector<std::pair<std::string, G4double>> &dz_map,
            double airBefore, double airMid);

/// Add module of scintillator sensor with absorbers, readout, and support, for
/// the hadronic calorimeter (FH).
/// @param[in] FHid ID of module in the setup (not all modules are identical)
/// @param[out] dz_map List of element names and air gap to be placed in front
/// of the element
/// @param[in] airBefore How much air should be added before the current module
/// (before the first iron)
/// @param[in] airMid How much air should be added in the middle (before the
/// first PCB)
void add_FH(uint FHid, std::vector<std::pair<std::string, G4double>> &dz_map,
            double airBefore, double airMid);

/// Define detector setup for test beam run in October 2018
/// @param[out] dz_map List of element names and air gap to be placed in front
/// of the element
/// @param[out] viewpoint Targer point to be set in the visualisation
void defineConfig22_October2018_1(
    std::vector<std::pair<std::string, G4double>> &dz_map, G4double &viewpoint);