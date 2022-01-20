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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4VisAttributes.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fAbsorMaterial(0), fLAbsor(0),fScoringVolume(0), fContainMaterial(0),fLayerMaterial(0),fDetMaterial(0), fLContain(0),fLLayer(0),fLDet(0),
 fWorldMaterial(0), fPWorld(0), fDetectorMessenger(0)
{
  fContainerRadius = 11.7*mm;
    fDetThickness=0.5*cm;
  fContainerLength = 200*mm;
  fContainThickness = 0.5*mm;
    fContainTappoThickness= 0.5*mm;
  DefineMaterials();
    SetDetMaterial  ("Lyso");
  SetAbsorMaterial  ("Helium3");
  SetContainMaterial("Stainless-Steel");
  SetLayerMaterial("G4_Al");
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4int ncomponents, natoms;
  
  G4Element* Be = new G4Element("Beryllium","Be" ,  4.,  9.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen" ,"N"  ,  7., 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"   ,"O"  ,  8., 16.00*g/mole);
  G4Element* Cr = new G4Element("Chromium" ,"Cr" , 24., 51.99*g/mole);
  G4Element* Fe = new G4Element("Iron"     ,"Fe" , 26., 55.84*g/mole);
  G4Element* Ni = new G4Element("Nickel"   ,"Ni" , 28., 58.69*g/mole);
  
  G4Element* Zn = G4NistManager::Instance()->FindOrBuildElement("Zn");
  G4Element* Mg = G4NistManager::Instance()->FindOrBuildElement("Mg");
  G4Element* Al =G4NistManager::Instance()->FindOrBuildElement("Al");
  G4Element* Cu =G4NistManager::Instance()->FindOrBuildElement("Cu");
  
  G4Element* Zr = G4NistManager::Instance()->FindOrBuildElement("Zr");
    G4Element* Sc = G4NistManager::Instance()->FindOrBuildElement("Sc");
    G4Element* Mn = G4NistManager::Instance()->FindOrBuildElement("Mn");
    G4Element* Ti = G4NistManager::Instance()->FindOrBuildElement("Ti");
    G4Element* V = G4NistManager::Instance()->FindOrBuildElement("V");//vanadio
    
  G4Element* C =G4NistManager::Instance()->FindOrBuildElement("C");
  G4Element* H  = G4NistManager::Instance()->FindOrBuildElement("H");
  
    G4Element* Lu =G4NistManager::Instance()->FindOrBuildElement("Lu");
    G4Element* Si  = G4NistManager::Instance()->FindOrBuildElement("Si");
    G4Element* Y  = G4NistManager::Instance()->FindOrBuildElement("Y");
    
  G4Material* BeO =
  new G4Material("BeO", 3.05*g/cm3, ncomponents=2);
  BeO->AddElement(Be, natoms=1);
  BeO->AddElement( O, natoms=1);
  
  G4Material* inox =
  new G4Material("Stainless-Steel", 8*g/cm3, ncomponents=3);
  inox->AddElement(Fe, 74*perCent);
  inox->AddElement(Cr, 18*perCent);
  inox->AddElement(Ni,  8*perCent);

  G4Material* Air =
  new G4Material("Air", 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, 70.*perCent);
  Air->AddElement(O, 30.*perCent);
  
  G4Material* Vacuum =
  new G4Material("Galactic", 1, 1.01*g/mole, universe_mean_density,
  kStateGas, 2.73*kelvin, 3.e-18*pascal);
    
  
  // He-3 detector materials
    // helium 3 material gas
    // Density at 21.1°C (70°F) ad 1 atm: 0.1650 kg/m3
         // G4double density =  1.65*mg/cm3;//0.17850*mg/cm3;//0.00049*g/cm3;
    G4double pressure = 30*bar;
    G4double molar_constant = Avogadro*k_Boltzmann;  //from clhep
    G4double temperature = 300*kelvin;
    
         //T=(atomicMass*pressure)/(density*molar_constant);
    G4double atomicMass = 3.016*g/mole;//massa molare)
    G4double density=(atomicMass*pressure)/(temperature*molar_constant);
    G4cout<<"density="<<density<<G4endl;
    G4Isotope* he3 = new G4Isotope("He3",2,3,atomicMass);
    G4Element* He3 = new G4Element("helium3","He3",1);
    He3->AddIsotope(he3,100*perCent);
    G4Material* He3_30bar = new G4Material("Helium3", density, 1,kStateGas, temperature,pressure);
    He3_30bar->AddElement(He3, 100*perCent);
    
    atomicMass = 15.99* g/mole;//massa molare)
    density=(atomicMass*pressure)/(temperature*molar_constant);
    G4Isotope* o16 = new G4Isotope("o16",8,16,atomicMass);
    G4Element* O16 = new G4Element("oxigen16","O16",1);
    O16->AddIsotope(o16,100*perCent);
    G4Material* O16_30bar = new G4Material("16O", density, 1,kStateGas, temperature,pressure);
    O16_30bar->AddElement(O16, 100*perCent);
    
  G4Material* ergal =
  new G4Material("Ergal", 2.88*g/cm3, ncomponents=4);
  ergal->AddElement(Zn, 5.85*perCent);
  ergal->AddElement(Mg, 2.3*perCent);
  ergal->AddElement(Cu, 1.4*perCent);
  ergal->AddElement(Al, 90.45*perCent);
  
    density = 1.81*g/cm3;//0.145*g/cm3;
    G4Material* CarbonF = new G4Material("CF",density,1);
    CarbonF->AddElement(C,1);
    
//LYSO:d= 7.10 g/cm3  [Lu]=71.45%   [Y]= 4.03 %   [Si]= 6.37%  [O]=18.15%
    G4Material* lyso = new G4Material("Lyso", 7.10*g/cm3, ncomponents=4);
    lyso->AddElement(Lu, 71.45*perCent);
    lyso->AddElement(Y, 4.03*perCent);
    lyso->AddElement(Si, 6.37*perCent);
    lyso->AddElement(O, 18.15*perCent);
    
    G4Material* Sci =
    new G4Material("EJ-200", density= 1.023*g/cm3, ncomponents=2);
    Sci->AddElement(C, natoms=21);
    Sci->AddElement(H, natoms=19);
    
//    G4Material* PET = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
//    
//    G4Material* Kapton = G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
    
    //Epoxy resin (C11H12O3)
    G4double d_Epoxy = 1.1*g/cm3;        //weight ratio 70%:30%
    G4Material* Epoxy = new G4Material("EpoxyResin", d_Epoxy, 3);
    Epoxy->AddElement(H, 12);
    Epoxy->AddElement(C, 11);
    Epoxy->AddElement(O, 3);
    
    //Carbon Fibre with Epoxy resin
    G4double d_CarbonFiber = 1.597*g/cm3;
    G4Material* CarbonFiber = new G4Material("CarbonFiber", d_CarbonFiber, 2);
    CarbonFiber->AddMaterial(CarbonF, 0.7);
    CarbonFiber->AddMaterial(Epoxy, 0.3);

    G4Material* ScalmAlloy =
    new G4Material("ScAlMalloy", 2.67*g/cm3, ncomponents=12);
    ScalmAlloy->AddElement(Mg, 4.9*perCent);
    ScalmAlloy->AddElement(Sc, 0.8*perCent);
    ScalmAlloy->AddElement(Zr, 0.5*perCent);
    ScalmAlloy->AddElement(Mn, 0.8*perCent);
    ScalmAlloy->AddElement(Si, 0.4*perCent);
    ScalmAlloy->AddElement(Fe, 0.4*perCent);
    ScalmAlloy->AddElement(Zn, 0.25*perCent);
    ScalmAlloy->AddElement(Cu, 0.1*perCent);
    ScalmAlloy->AddElement(Ti, 0.15*perCent);
    ScalmAlloy->AddElement(O, 0.05*perCent);
    ScalmAlloy->AddElement(V, 0.05*perCent);
    double percAl=100-4.9-0.8-0.5-0.8-0.4-0.4-0.25-0.1-0.15-0.05-0.05;
    ScalmAlloy->AddElement(Al, percAl*perCent);
    
    fWorldMaterial = Vacuum;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::MaterialWithSingleIsotope( G4String name,
                           G4String symbol, G4double density, G4int Z, G4int A)
{
 // define a material from an isotope
 //
 G4int ncomponents;
 G4double abundance, massfraction;

 G4Isotope* isotope = new G4Isotope(symbol, Z, A);
 
 G4Element* element  = new G4Element(name, symbol, ncomponents=1);
 element->AddIsotope(isotope, abundance= 100.*perCent);
 
 G4Material* material = new G4Material(name, density, ncomponents=1);
 material->AddElement(element, massfraction=100.*perCent);

    return material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  // compute dimensions
//    G4double AbsorRadius = fContainerRadius-fContainThickness;//fAbsorRadius + fContainThickness;
//    G4double AbsorRadiusTappo = fContainerRadius-fContainTappoThickness;//fAbsorRadius + fContainThickness;
    G4double AbsorLength = fContainerLength;//-2*fContainThickness;//fAbsorLength + 2*fContainThickness;
    G4double ScreenLength = 40*cm;
    G4double ScreenRadius = 5*cm;
  G4double WorldSizeXY = 2.*ScreenLength;//;2.*2*(ScreenRadius+fDetThickness);
  G4double WorldSizeZ  = 2.*ScreenLength;
  
  // World
  //
  G4Box*
  sWorld = new G4Box("World",                                    //name
              0.5*WorldSizeXY,0.5*WorldSizeXY,0.5*WorldSizeZ);   //dimensions
                   
  G4LogicalVolume*
  lWorld = new G4LogicalVolume(sWorld,                  //shape
                             fWorldMaterial,            //material
                             "World");                  //name

  fPWorld = new G4PVPlacement(0,                        //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            lWorld,                     //logical volume
                            "World",                    //name
                            0,                          //mother volume
                            false,                      //no boolean operation
                            0,true);                         //copy number
    
//    // Screen volume
//    G4Tubs* screenC1 = new G4Tubs("Screen1", ScreenRadius, ScreenRadius+fDetThickness, 0.5*ScreenLength, 0., twopi);
//    fLDet = new G4LogicalVolume(screenC1, fDetMaterial , "ScreenLV");
//
//    new G4PVPlacement(0, G4ThreeVector(),
//                      fLDet, "Screen", lWorld, false, 0, 0);
//    G4VisAttributes* screenVisAtt = new G4VisAttributes( G4Colour(0,0,1) );
//    screenVisAtt -> SetForceSolid();
//    fLDet->SetVisAttributes( screenVisAtt );
//    fLDet->SetVisAttributes( G4VisAttributes::GetInvisible() );
   
    //il cilindro rivelatore è sostituito con due piani
    G4double screenDimy=fDetThickness;
    G4double screenDimx=40*cm;
    G4double screenDimz=40*cm;
    G4double distanza_det=10*cm;
   G4Box* screenC1 = new G4Box("Screen1",                                    //name
                                    0.5*screenDimx,0.5*screenDimy,0.5*screenDimz);
    G4Box* screenC2 = new G4Box("Screen2",                                    //name
                                    0.5*screenDimx,0.5*screenDimy,0.5*screenDimz);
    G4ThreeVector TransScreen(0, distanza_det+screenDimy, 0);
    G4RotationMatrix RotScreen(0,0,0);
    G4Transform3D trScrenn(RotScreen, TransScreen);
    G4UnionSolid* screen=new G4UnionSolid("screen_tot", screenC1, screenC2, trScrenn);
    fLDet = new G4LogicalVolume(screen, fDetMaterial , "ScreenLV");
//    G4RotationMatrix *RotScreentot= new G4RotationMatrix(0,90,0);

        new G4PVPlacement(0,-TransScreen/2.,
                          fLDet, "Screen", lWorld, false, 0, 0);
        G4VisAttributes* screenVisAtt = new G4VisAttributes( G4Colour(0,0,1) );
        screenVisAtt -> SetForceSolid();
        fLDet->SetVisAttributes( screenVisAtt );
//        fLDet->SetVisAttributes( G4VisAttributes::GetInvisible() );
    
  // Container
  //
    G4double rmin=0;//fContainerRadius-fContainThickness;
    G4double rmax=fContainerRadius;
    G4double rmin_tappo=0;//fContainerRadius-fContainTappoThickness;
    G4double rmax_tappo=fContainerRadius;
  G4Tubs*
  sContain_parete = new G4Tubs("Container_p",                            //name
                        rmin, rmax, 0.5*fContainerLength, 0., 360*degree);  //dimensions
  G4Sphere* sContain_tappo=new  G4Sphere("Container_t",rmin_tappo,rmax_tappo,0,180*degree,0,180*degree );
    G4RotationMatrix* yRot =new G4RotationMatrix;// Rotates X and Z axes only
    yRot->rotateX(-M_PI/2.*rad);// Rotates 45 degrees
    G4RotationMatrix* yRot2 =new G4RotationMatrix;// Rotates X and Z axes only
    yRot2->rotateX(M_PI/2.*rad);// Rotates 45 degrees
    
    G4ThreeVector zTrans(0, 0, 0.5*fContainerLength);
    G4RotationMatrix rotm1 = yRot->invert();
    G4RotationMatrix rotm2 = yRot2->invert();
    G4ThreeVector position1 = zTrans;
    G4ThreeVector position2 = -zTrans;
    G4Transform3D tr1(rotm1, position1);
    G4Transform3D tr2(rotm2, position2);
    
    G4UnionSolid* sContain_parz =new G4UnionSolid("sContain_parz", sContain_parete, sContain_tappo, tr1);
    G4UnionSolid* sContain =new G4UnionSolid("sContain", sContain_parz, sContain_tappo, tr2);

    fLContain = new G4LogicalVolume(sContain,            //shape
                       fContainMaterial,               //material
                       fContainMaterial->GetName());   //name

           new G4PVPlacement(0,                        //no rotation
                       G4ThreeVector(),                //at (0,0,0)
                       fLContain,                      //logical volume
                       fContainMaterial->GetName(),    //name
                       lWorld,                         //mother  volume
                       false,                          //no boolean operation
                       0,true);                             //copy number
//    G4VisAttributes* contVisAtt = new G4VisAttributes( G4Colour(0,1,1) );
//    //contVisAtt -> SetForceWireframe( false );
//    contVisAtt -> SetForceSolid();
//    fLContain->SetVisAttributes( contVisAtt );
    fLContain->SetVisAttributes(G4VisAttributes::GetInvisible());
///////////////da attivare solo nel caso metta alluminio
    // strato mylar+alluminio
    //
    rmax=fContainerRadius-fContainTappoThickness;
    rmax_tappo=fContainerRadius-fContainTappoThickness;
    G4Tubs*
    sContainM_parete = new G4Tubs("ContainerM_p",                            //name
                          rmin, rmax, 0.5*fContainerLength, 0., 360*degree);  //dimensions
    G4Sphere* sContainM_tappo=new  G4Sphere("ContainerM_t",rmin_tappo,rmax_tappo,0,180*degree,0,180*degree );
      G4RotationMatrix* yRotM =new G4RotationMatrix;// Rotates X and Z axes only
      yRotM->rotateX(-M_PI/2.*rad);// Rotates 45 degrees
      G4RotationMatrix* yRotM2 =new G4RotationMatrix;// Rotates X and Z axes only
      yRotM2->rotateX(M_PI/2.*rad);// Rotates 45 degrees
      
      G4ThreeVector zTransM(0, 0, 0.5*fContainerLength);
      G4RotationMatrix rotm1M = yRotM->invert();
      G4RotationMatrix rotm2M = yRotM2->invert();
      G4ThreeVector position1M = zTransM;
      G4ThreeVector position2M = -zTransM;
      G4Transform3D tr1M(rotm1M, position1M);
      G4Transform3D tr2M(rotm2M, position2M);
      
      G4UnionSolid* sContainM_parz =new G4UnionSolid("sContainM_parz", sContainM_parete, sContainM_tappo, tr1M);
      G4UnionSolid* sContainM =new G4UnionSolid("sContainM", sContainM_parz, sContainM_tappo, tr2M);

      fLLayer = new G4LogicalVolume(sContainM,            //shape
                         fLayerMaterial,               //material
                         fLayerMaterial->GetName());   //name

             new G4PVPlacement(0,                        //no rotation
                         G4ThreeVector(),                //at (0,0,0)
                         fLLayer,                      //logical volume
                         fLayerMaterial->GetName(),    //name
                         fLContain,                         //mother  volume
                         false,                          //no boolean operation
                         0,true);
    fLLayer->SetVisAttributes(G4VisAttributes::GetInvisible());
//    G4VisAttributes* layerVisAtt = new G4VisAttributes( G4Colour(1,1,1) );
//    layerVisAtt -> SetForceSolid();
//        fLLayer->SetVisAttributes( layerVisAtt );
    G4double spessoreLayerMylar=0.025*mm;
    rmax=fContainerRadius-fContainTappoThickness-spessoreLayerMylar;
        rmax_tappo=fContainerRadius-fContainTappoThickness-spessoreLayerMylar;
        G4Tubs*
        sContainAl_parete = new G4Tubs("ContainerAl_p",                            //name
                              rmin, rmax, 0.5*fContainerLength, 0., 360*degree);  //dimensions
        G4Sphere* sContainAl_tappo=new  G4Sphere("ContainerAl_t",rmin_tappo,rmax_tappo,0,180*degree,0,180*degree );
          G4RotationMatrix* yRotAl =new G4RotationMatrix;// Rotates X and Z axes only
          yRotAl->rotateX(-M_PI/2.*rad);// Rotates 45 degrees
          G4RotationMatrix* yRotAl2 =new G4RotationMatrix;// Rotates X and Z axes only
          yRotAl2->rotateX(M_PI/2.*rad);// Rotates 45 degrees
          
          G4ThreeVector zTransAl(0, 0, 0.5*fContainerLength);
          G4RotationMatrix rotm1Al = yRotAl->invert();
          G4RotationMatrix rotm2Al = yRotAl2->invert();
          G4ThreeVector position1Al = zTransAl;
          G4ThreeVector position2Al = -zTransAl;
          G4Transform3D tr1Al(rotm1Al, position1Al);
          G4Transform3D tr2Al(rotm2Al, position2Al);
          
          G4UnionSolid* sContainAl_parz =new G4UnionSolid("sContainAl_parz", sContainAl_parete, sContainAl_tappo, tr1Al);
          G4UnionSolid* sContainAl =new G4UnionSolid("sContainAl", sContainAl_parz, sContainAl_tappo, tr2Al);
      G4Material* Aluminium =G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
      G4LogicalVolume* fLLayerAL = new G4LogicalVolume(sContainAl,            //shape
                             Aluminium,               //material
                             Aluminium->GetName());   //name

                 new G4PVPlacement(0,                        //no rotation
                             G4ThreeVector(),                //at (0,0,0)
                             fLLayerAL,                      //logical volume
                             fLayerMaterial->GetName(),    //name
                             fLLayer,                         //mother  volume
                             false,                          //no boolean operation
                             0,true);
    //    fLLayerAL->SetVisAttributes(G4VisAttributes::GetInvisible());
        G4VisAttributes* layerAlVisAtt = new G4VisAttributes( G4Colour(1,0,1) );
        layerAlVisAtt -> SetForceSolid();
            fLLayerAL->SetVisAttributes( layerAlVisAtt );
    G4double spessoreLayerAl=0.005*mm;
    G4double AbsorRadius = fContainerRadius-fContainThickness-spessoreLayerMylar-spessoreLayerAl;
    G4double AbsorRadiusTappo = fContainerRadius-fContainTappoThickness-spessoreLayerMylar-spessoreLayerAl;
    
//!---------------------------------------------------------------------
  // Absorber
  //
    G4Tubs*
    sAbsor_parete = new G4Tubs("Absorber_p",0., AbsorRadius, 0.5*AbsorLength, 0., twopi);
    G4cout <<"AbsorRadius"<<AbsorRadius<<" "<<G4endl;
    //dimensions
    G4Sphere* sAbsor_tappo=new  G4Sphere("Absorber_t",0,AbsorRadiusTappo,0,pi,0,pi );
    
    G4ThreeVector zTrans_a(0, 0, 0.5*AbsorLength);
    G4ThreeVector position1_a = zTrans_a;
    G4ThreeVector position2_a = -zTrans_a;
    G4Transform3D tr1_a(rotm1, position1_a);
    G4Transform3D tr2_a(rotm2, position2_a);
    G4UnionSolid* sAbsor_parz =new G4UnionSolid("sAbsor_parz", sAbsor_parete, sAbsor_tappo, tr1_a);
    G4UnionSolid* sAbsor =new G4UnionSolid("sAbsor", sAbsor_parz, sAbsor_tappo, tr2_a);

  fLAbsor = new G4LogicalVolume(sAbsor,                //shape
                       fAbsorMaterial,                 //material
                       fAbsorMaterial->GetName());     //name

           new G4PVPlacement(0,                        //no rotation
                       G4ThreeVector(),                //at (0,0,0)
                       fLAbsor,                        //logical volume
                       "Absorb_phys",      //name
                       fLLayerAL,                      //mother  volume
                       false,                          //no boolean operation
                       0,true);                             //copy number
G4VisAttributes* absVisAtt = new G4VisAttributes( G4Colour(0,0,1) );
absVisAtt -> SetForceSolid();
    fLAbsor->SetVisAttributes( absVisAtt );
//    fLAbsor->SetVisAttributes(G4VisAttributes::GetInvisible());
    PrintParameters();
    fScoringVolume=fLDet;
    lWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  //always return the root volume
  //
  return fPWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
    G4cout << "\n The Container  is a cylinder of " << fContainMaterial->GetName()
        << "  radius = " << G4BestUnit(fContainerRadius,"Length")
        << "  length = " << G4BestUnit(fContainerLength,"Length")
        << "  thickness = " << G4BestUnit(fContainThickness,"Length")<< G4endl;
 G4cout << " The Absorber is a cylinder of " << fAbsorMaterial->GetName()<< G4endl;
 G4cout << " The Screen is a cylinder of " << fDetMaterial->GetName()<< G4endl;
 G4cout << "\n" << fAbsorMaterial << G4endl;
 G4cout << "\n" << fContainMaterial << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  
  if (pttoMaterial) {
    fAbsorMaterial = pttoMaterial;
    if(fLAbsor) { fLAbsor->SetMaterial(fAbsorMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetAbsorMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetContainMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  
  if (pttoMaterial) {
    fContainMaterial = pttoMaterial;
    if(fLContain) { fLContain->SetMaterial(fContainMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetContainMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}
void DetectorConstruction::SetLayerMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  
  if (pttoMaterial) {
    fLayerMaterial = pttoMaterial;
    if(fLLayer) { fLLayer->SetMaterial(fLayerMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetContainMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}
void DetectorConstruction::SetDetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  
  if (pttoMaterial) {
    fDetMaterial = pttoMaterial;
    if(fLDet) { fLDet->SetMaterial(fDetMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetDetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetDetThickness(G4double value)
{
  fDetThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetContainerRadius(G4double value)
{
  fContainerRadius = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetContainerLength(G4double value)
{
  fContainerLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetContainThickness(G4double value)
{
  fContainThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetContainTappoThickness(G4double value)
{
  fContainTappoThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetLayerThickness(G4double value)
{
  fLayerThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//void DetectorConstruction::ConstructSDandField()
//{
//  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
//
//  //
//  // Sensitive detectors
//  //
//  auto screenSD = new ScreenSD("ScreenSD");
//  G4SDManager::GetSDMpointer()->AddNewDetector(screenSD);
//  SetSensitiveDetector("Screen", screenSD);
//}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

