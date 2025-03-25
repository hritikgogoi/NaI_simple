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
//
/// \file B1/src/EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include"G4AnalysisManager.hh"
#include <fstream> // Include for file handling
#include <random> // Include for random number generation

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  

   // Open the text file in append mode
    std::ofstream outfile("optical_photons_per_event.text", std::ios::app);
    if (outfile.is_open() && fEdep != 0)
    {
        // Convert energy to eV
        G4double energyInEV = fEdep / CLHEP::eV;

        // Energy per photon (mean ± std deviation)
        G4double energyPerPhoton = 17.2; // Mean energy per photon in eV
        G4double energyDeviation = 0.4;   // Standard deviation in eV

        // Random number generator for the photon energy
        std::random_device rd;  // Random device
        std::mt19937 gen(rd()); // Mersenne Twister generator
        std::normal_distribution<> d(energyPerPhoton, energyDeviation); // Normal distribution

        // Generate the energy for a single photon, and calculate the number of photons produced
        int numberOfPhotons = static_cast<int>(energyInEV / std::abs(d(gen))); // Use absolute to avoid negative values

        // Write energy and number of photons to the file
        outfile<< numberOfPhotons << std::endl;

        // Close the file after writing
        outfile.close();
    }
   
  
  
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
