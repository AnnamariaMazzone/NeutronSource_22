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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    DetectorConstruction();
    ~DetectorConstruction();
    
public:
    
    virtual G4VPhysicalVolume* Construct();
    
    G4Material*
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);
    
    void SetDetThickness (G4double);
    void SetContainerRadius   (G4double);
    void SetContainerLength   (G4double);
    void SetAbsorMaterial (G4String);
    
    void SetContainThickness (G4double);
    void SetContainTappoThickness (G4double);
    void SetLayerThickness(G4double);
    void SetContainMaterial (G4String);
    void SetLayerMaterial (G4String);
    void SetDetMaterial (G4String);
    
    //    void ConstructSDandField();
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    
public:
    
    G4double           GetContainerRadius()     {return fContainerRadius;};
    G4double           GetContainerLength()     {return fContainerLength;};
    G4Material*        GetAbsorMaterial()   {return fAbsorMaterial;};
    
    G4double           GetContainThickness()  {return fContainThickness;};
    G4double           GetLayerThickness()  {return fLayerThickness;};
    G4double           GetContainTappoThickness()  {return fContainTappoThickness;};
    G4Material*        GetContainMaterial()   {return fContainMaterial;};
    G4Material*        GetLayerMaterial()   {return fLayerMaterial;};
    G4Material*        GetDetMaterial()   {return fDetMaterial;};
    void               PrintParameters();
    
private:
    
    G4double           fContainerRadius, fContainerLength;
    G4Material*        fAbsorMaterial;
    G4LogicalVolume*   fLAbsor;
    G4LogicalVolume*   fScoringVolume;
    G4double           fContainThickness;
    G4double           fLayerThickness;
    G4double           fDetThickness;
    G4double           fContainTappoThickness;
    G4Material*        fContainMaterial;
    G4Material*        fLayerMaterial;
    G4Material*        fDetMaterial;
    G4LogicalVolume*   fLContain;
    G4LogicalVolume*   fLLayer;
    G4LogicalVolume*   fLDet;
    
    G4Material*        fWorldMaterial;
    G4VPhysicalVolume* fPWorld;
    
    DetectorMessenger* fDetectorMessenger;
    
private:
    
    void               DefineMaterials();
    G4VPhysicalVolume* ConstructVolumes();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

