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

#ifndef SIPMHIT_HHinline 
#define SIPMHIT_HHinline 

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include <vector>
#include <cstdlib>

/**
 * @brief SiPM hit
 *
 * Stores information of energy deposited in the SiPM.
 * 
 * Hits can be digitised, to take into account the time window for the deposits
 * as well as to set the time information based on the energy threshold.
 * By default no time window is used (all energy deposits are counted), as well
 * as no energy threshold is used (time of the first energy deposit, however
 * small, is counted as hit time).
 * 
 */

class SiPMHit : public G4VHit {
	public:
	///	Constructor
	/// @param[in] aName Name of the pixel volume
	/// @param[in] aCopyNoSensor ID of the sensor
	/// @param[in] aCopyNoCell ID of the cell
		SiPMHit(G4String aName, G4int aCopyNoSensor, G4int aCopyNoCell);
		~SiPMHit() {};
	/// Get hit ID calculated as 1000 * sensorID + cellID
		G4int ID() {return 1000*copy_no_sensor+copy_no_cell;}
		/// Add non-zero energy deposit to vector of deposits
		/// @param[in] aEnergy Deposited energy
		/// @param[in] aTime Time of deposit
		inline void AddEdep(const G4double aEnergy, const G4double aTime) {if (aEnergy>0) eDep.push_back(std::make_pair(aEnergy, aTime)); }
		/// Add non-zero non-ionizing energy deposit to vector of deposits
		/// @param[in] aEnergy Deposited energy
		/// @param[in] aTime Time of deposit
		inline void AddEdepNonIonizing(const G4double aEnergy, const G4double aTime) {if (aEnergy>0) edep_nonIonizing.push_back(std::make_pair(aEnergy, aTime)); }
		/// Digitise hit
		/// Calculate time of hit as global time of energy deposit which added
		/// to hit energy exceeds the energy threshold. Take into account only
		/// deposits with global time within the timeWindow.
		/// @param[in] timeWindow Maximal global time for deposit, caounted from
		/// the time of the first deposit
		/// @param[in] toaThreshold Energy threshold, first deposit that adds to
		/// the hit energy and exceeds the threshold is counted as time of
		/// arrival.
		void Digitise(const G4double timeWindow, const G4double toaThreshold);
		/// Set hit position
		/// @param[in] x X position
		/// @param[in] y Y position
		/// @param[in] z Z position
		void SetPosition(G4double x, G4double y, G4double z) {
			this->pos_x = x;
			this->pos_y = y;
			this->pos_z = z;
		}
		/// Get hit X position
		inline G4double GetX() const {return this->pos_x;}
		/// Get hit Y position
		inline G4double GetY() const {return this->pos_y;}
		/// Get hit Z position
		inline G4double GetZ() const {return this->pos_z;}
		/// Check if hit is valid
		inline bool isValidHit() const {return _isValidHit;}
		/// Get hit energy
		inline G4double GetEdep() const {return eDep_digi;}
		/// Get hit non-ionizing energy
		inline G4double GetEdepNonIonizing() const {return edep_nonIonizing_digi;}
		/// Get time of arrival
		inline G4double GetTOA() const {return timeOfArrival_digi;}

	private:
	///	Name of the logical volume
		G4String vol_name;
		/// ID of the sensor
		G4int copy_no_cell; 
		/// ID of the cell 
		G4int copy_no_sensor; 
		/// Position along x axis
		G4double pos_x;
		/// Position along y axis
		G4double pos_y;
		/// Position along z axis
		G4double pos_z;
		/// Vector of energy deposits (and their global time)
		std::vector<std::pair<G4double, G4double> > eDep;
		/// Vector of non-ionizing energy deposits (and their global time)
		std::vector<std::pair<G4double, G4double> > edep_nonIonizing;
		/// Flag indicating if hit is valid (digitised and with non-zero energy)
		bool _isValidHit;
		/// Energy of the digitised hit
		G4double eDep_digi;
		/// Non-ionizing energy of the digitised hit
		G4double edep_nonIonizing_digi;
		/// Time of arrival of the digitised hit
		G4double timeOfArrival_digi;
};

typedef G4THitsCollection<SiPMHit> SiPMHitCollection;

#endif /* SIPMHIT_HH */