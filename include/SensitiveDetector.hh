#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent*) override;

private:
    G4int fEventID;
};

#endif