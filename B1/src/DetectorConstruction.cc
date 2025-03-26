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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"




#include "G4Tubs.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

  //

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 50*cm;
  G4double world_sizeZ  = 50*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking




  //  Define Materials
  G4Material* NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4Material* Tl = nist->FindOrBuildMaterial("G4_Tl");
  G4Material* MgO = nist->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
  G4Material* Al = nist->FindOrBuildMaterial("G4_Al");

  // Define NaI(Tl) Scintillator
  G4Material* NaI_Tl = new G4Material("NaI_Tl", 3.67*g/cm3, 2);
  NaI_Tl->AddMaterial(NaI, 99.5 * perCent);
  NaI_Tl->AddMaterial(Tl, 0.5 * perCent);

  //  Define Detector Dimensions
  G4double NaI_radius = 2.54 * cm;  // 2-inch diameter → 1-inch radius
  G4double NaI_height = 5.08 * cm;  // 2-inch height
  G4double MgO_thickness = 0.5 * mm;
  G4double Al_thickness = 0.5 * mm;

  G4double MgO_front_shifitng=NaI_height/2+MgO_thickness/2;
  G4double Al_front_shifitng=NaI_height/2+MgO_thickness+Al_thickness/2;

  //  Create NaI(Tl) Detector (Scintillator)
  G4Tubs* solidNaI = new G4Tubs("NaI_Tl", 0, NaI_radius, NaI_height / 2, 0, 360 * deg);
  G4LogicalVolume* logicNaI = new G4LogicalVolume(solidNaI, NaI_Tl, "NaI_Tl_Logical");

  //  Create MgO Reflective Layer (Covers  Front & Sides, Open Back)
  G4Tubs* solidMgO = new G4Tubs("MgO_Layer", NaI_radius, NaI_radius + MgO_thickness, NaI_height / 2, 0, 360 * deg);
  G4LogicalVolume* logicMgO = new G4LogicalVolume(solidMgO, MgO, "MgO_Logical");
  G4Tubs* solidMgO_front = new G4Tubs("MgO_Layer", 0, NaI_radius + MgO_thickness,  MgO_thickness/ 2, 0, 360 * deg);
  G4LogicalVolume* logicMgO_front = new G4LogicalVolume(solidMgO_front, MgO, "MgO_Logical_Front");
  //  Create Aluminum Outer Layer (Covers  Front & Sides, Open Back)
  G4Tubs* solidAl = new G4Tubs("Al_Layer", NaI_radius + MgO_thickness, NaI_radius + MgO_thickness + Al_thickness, NaI_height / 2, 0, 360 * deg);
  G4LogicalVolume* logicAl = new G4LogicalVolume(solidAl, Al, "Al_Logical");
  G4Tubs* solidAl_front = new G4Tubs("Al_Layer", 0, NaI_radius + MgO_thickness + Al_thickness,  Al_thickness/ 2, 0, 360 * deg);
  G4LogicalVolume* logicAl_front  = new G4LogicalVolume(solidAl_front , Al, "Al_Logical_Front");
    //  Place Components in World Volume

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicNaI, "NaI_Tl_Physical", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicMgO, "MgO_Physical", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, -MgO_front_shifitng), logicMgO_front, "MgO_Physical_Front", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAl, "Al_Physical", logicWorld, false, 0, true);
  new G4PVPlacement(0, G4ThreeVector(0, 0, -Al_front_shifitng), logicAl_front, "Al_Physical_Front", logicWorld, false, 0, true);



///////////////////___Add Lead cylinder to cover the enrtire setup

    //__Lead 


G4Tubs* lead=
new G4Tubs("Lead",9*cm,12*cm,15*cm,0,360*deg);

G4LogicalVolume* log_lead = new G4LogicalVolume(lead, nist->FindOrBuildMaterial("G4_Pb"), "Lead");


G4VPhysicalVolume* phy_lead = new G4PVPlacement(0, G4ThreeVector(),log_lead , "Lead", logicWorld, false, 0);

//__Lead_front


G4Tubs* lead_f=
new G4Tubs("Lead_f",0*cm,9*cm,1*cm,0,360*deg);

G4LogicalVolume* log_lead_f = new G4LogicalVolume(lead_f, nist->FindOrBuildMaterial("G4_Pb"), "Lead_f");


G4VPhysicalVolume* phy_lead_f = new G4PVPlacement(0, G4ThreeVector(0,0,-14*cm),log_lead_f , "Lead_f", logicWorld, false, 0);

//__Lead_Back


G4Tubs* lead_b=
new G4Tubs("Lead_b",0*cm,9*cm,1*cm,0,360*deg);

G4LogicalVolume* log_lead_b = new G4LogicalVolume(lead_b, nist->FindOrBuildMaterial("G4_Pb"), "Lead_b");


G4VPhysicalVolume* phy_lead_b = new G4PVPlacement(0, G4ThreeVector(0,0,14*cm),log_lead_b , "Lead_b", logicWorld, false, 0);



  // Set Crystal as scoring volume
  //
  fScoringVolume = logicNaI;


    // Visualization
    G4VisAttributes* NaI_VisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0)); // Yellow color
     NaI_VisAtt->SetForceSolid(true);
     logicNaI->SetVisAttributes(NaI_VisAtt); // Yellow for NaI(Tl)
     logicMgO->SetVisAttributes(new G4VisAttributes(G4Colour(0.9, 0.9, 0.9))); // White for MgO
     logicAl->SetVisAttributes(new G4VisAttributes(G4Colour(0.5, 0.5, 0.5))); // Gray for Aluminum


  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
