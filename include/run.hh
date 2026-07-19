#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class RunMessenger;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run *) override;
    void EndOfRunAction(const G4Run *) override;

    // Called by RunMessenger
    void SetMaterial(G4String mat);
    void SetParticle(G4String p);
    void SetEnergy(G4double e);
    void SetSize(G4double x, G4double y, G4double z);
    static RunAction *GetInstance();
    static RunAction *fInstance;
    void AddEnergyDeposit(G4double edep);
    void IncrementTransmittedParticles();
    G4int GetTransmittedParticles() const;
    void SetTotalEvents(G4int n);
    G4int GetTotalEvents() const;
    
    private:
    // simulation parameters
    G4String fMaterial;
    G4String fParticle;
    G4double fEnergy;
    
    G4double fX, fY, fZ;
    G4int fRunID;
    RunMessenger *fMessenger;
    G4double fTotalEnergyDeposit = 0.0;
    G4int fTransmittedParticles = 0;
    G4int fTotalEvents = 0;
};

#endif