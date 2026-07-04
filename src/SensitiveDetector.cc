#include "SensitiveDetector.hh"
#include "G4RootAnalysisManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "run.hh"

SensitiveDetector::SensitiveDetector(const G4String &name)
    : G4VSensitiveDetector(name)
{
}

SensitiveDetector::~SensitiveDetector()
{
}

void SensitiveDetector::Initialize(G4HCofThisEvent *)
{

}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    // nothing needed for now
}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4Track *track = aStep->GetTrack();

    G4String particleName =
        track->GetParticleDefinition()->GetParticleName();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();

    const G4VTouchable *touchable = preStepPoint->GetTouchable();

    G4int copyNo = touchable->GetCopyNumber();

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();

    auto *man = G4RootAnalysisManager::Instance();
    G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
    
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    
    G4double kineticEnergy = track->GetKineticEnergy() / MeV;
    G4double energyDeposit = aStep->GetTotalEnergyDeposit() / MeV;
    RunAction::GetInstance()->AddEnergyDeposit(energyDeposit);

    man->FillNtupleIColumn(0, runID);

    man->FillNtupleIColumn(1, trackID);
    man->FillNtupleIColumn(2, parentID);

    man->FillNtupleSColumn(3, particleName);

    man->FillNtupleDColumn(4, kineticEnergy);
    man->FillNtupleDColumn(5, energyDeposit);

    man->FillNtupleDColumn(6, posPhoton.x() / mm);
    man->FillNtupleDColumn(7, posPhoton.y() / mm);
    man->FillNtupleDColumn(8, posPhoton.z() / mm);
           return true;
}