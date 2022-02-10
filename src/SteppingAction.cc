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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector <G4double> saved_eventID(10,999999999999);
vector <G4ThreeVector> P_saved(10);
vector <G4double>  E_saved(10), mass_saved(10);
vector <G4double>  x_saved(10),y_saved(10), z_saved(10);
G4int ncont=0;
//
//bool buona = false;
//G4double dE_saved;
SteppingAction::SteppingAction(EventAction* event)
: G4UserSteppingAction(), fEventAction(event),fScoringVolume(0)
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4int index=G4Threading::G4GetThreadId();
    // count processes
    //
    const G4StepPoint* endPoint = aStep->GetPostStepPoint();
    const G4VProcess* process   = endPoint->GetProcessDefinedStep();
    //    if (!process){
    Run* run = static_cast<Run*>(
                                 G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    run->CountProcesses(process);
    if(process->GetProcessName()=="annihil")
    {
//        G4cout<<G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()<<G4endl;
        if(aStep->GetSecondary()->size()==0){
            ncont++;
            G4cout<<"senza gamma = "<<ncont<<G4endl;}
    }
    // energy deposit
    //
    if (!fScoringVolume) {
        const DetectorConstruction* detectorConstruction
        = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        
        fScoringVolume = detectorConstruction->GetScoringVolume();
    }
    G4LogicalVolume* volume
    = aStep->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();
    const G4ParticleDefinition* particle = aStep->GetTrack()->GetParticleDefinition();
    G4String name   = particle->GetParticleName();
    // check if we are in scoring volume
    if (volume != fScoringVolume) return;
    if(aStep->GetTrack()->GetParentID()!=0)return;
    
    if( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary){
        
        if(saved_eventID[index]==999999999999||G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() != saved_eventID [index]){
            saved_eventID[index]= G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            mass_saved[index] = particle->GetPDGMass();
            E_saved[index]=aStep->GetPreStepPoint()->GetKineticEnergy();
            P_saved[index]=aStep->GetPreStepPoint()->GetMomentumDirection();
            x_saved[index]=aStep->GetPreStepPoint()->GetPosition().x();
            y_saved[index]=aStep->GetPreStepPoint()->GetPosition().y();
            z_saved[index]=aStep->GetPreStepPoint()->GetPosition().z();
            //si deve salvare energia e momento della prima
            return;
        }
//        if (G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() != saved_eventID [index]){
//            saved_eventID[index]= G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
//            mass_saved[index] = particle->GetPDGMass();
//            E_saved[index]=aStep->GetPreStepPoint()->GetKineticEnergy();
//            P_saved[index]=aStep->GetPreStepPoint()->GetMomentumDirection();
//            x_saved[index]=aStep->GetPreStepPoint()->GetPosition().x();
//            y_saved[index]=aStep->GetPreStepPoint()->GetPosition().y();
//            z_saved[index]=aStep->GetPreStepPoint()->GetPosition().z();
//            return;
//        }
        //si prende energia e momento della seconda E CALCOLA MI e rimette a 99999 eventid
        G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
        G4double mass = particle->GetPDGMass();
        //        G4ThreeVector P=aStep->GetTrack()->GetMomentumDirection();
        G4ThreeVector P=aStep->GetPreStepPoint()->GetMomentumDirection();
        G4ThreeVector Pc=aStep->GetPreStepPoint()->GetMomentum();
        G4double mom1=Pc.mag();
        G4double mom2=sqrt(Pc.x()*Pc.x()+Pc.y()*Pc.y()+Pc.z()*Pc.z());
        G4double mom_saved=sqrt(pow(E_saved[index],2)-pow(mass_saved[index],2));
        G4double mom=sqrt(pow(energy,2)-pow(mass,2));
        G4double invMass=sqrt(pow(mass_saved[index],2)+pow(mass,2)+
                              2*(energy*E_saved[index]-mom_saved*mom*P.dot(P_saved[index])));//cos dell'ang tra i vettori
        
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(17,invMass);
        G4double angleE = P.angle(P_saved[index]);
        analysisManager->FillH1(16,angleE);
        //        //prendo la posizione sul det
        //        G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
        G4String nomefile="Plot1_IPC_he300bar";
        G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
        G4double Y=0;
        G4double em, ep , x,y,z, x1,y1,z1;
        if(particle->GetParticleName()=="e+"){
            Y=(energy-E_saved[index])/(E_saved[index]+energy);
            em=E_saved[index];
            ep=energy;
            x=x_saved[index];
            y=y_saved[index];
            z=z_saved[index];
            x1=pos.x();
            y1=pos.y();
            z1=pos.z();
        }
        else{
            Y=(E_saved[index]-energy)/(E_saved[index]+energy);
            ep=E_saved[index];
            em=energy;
            x1=x_saved[index];
            y1=y_saved[index];
            z1=z_saved[index];
            x=pos.x();
            y=pos.y();
            z=pos.z();
        }
        
        std::ofstream WriteDataIn(nomefile, std::ios::app);
        WriteDataIn
        <<   em       <<" " //  1
        <<   ep     <<" " //  2
        <<   mass         <<" " //  3
        <<   angleE       <<" " //  5
        <<   x         <<" "
        <<   y        <<" "
        <<   z         <<" "
        <<   x1        <<" "
        <<   y1        <<" "
        <<   z1        <<" "
        <<   G4endl;
        
        saved_eventID[index]=999999999999;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

