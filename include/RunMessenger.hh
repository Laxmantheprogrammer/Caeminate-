#ifndef RUNMESSENGER_HH
#define RUNMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

class RunAction;

class RunMessenger : public G4UImessenger
{
public:
    RunMessenger(RunAction* run);
    ~RunMessenger() override;

    void SetNewValue(G4UIcommand* command, G4String value) override;

private:
    RunAction* fRunAction = nullptr;

    G4UIdirectory* fDir = nullptr;

    G4UIcmdWithAString* fMatCmd = nullptr;
    G4UIcmdWithAString* fParticleCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fEnergyCmd = nullptr;
};

#endif