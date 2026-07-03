#include "RunMessenger.hh"
#include "run.hh"

#include "G4RunManager.hh"

RunMessenger::RunMessenger(RunAction* run)
: fRunAction(run)
{
    fDir = new G4UIdirectory("/run/");
    fDir->SetGuidance("Run configuration commands");

    fMatCmd = new G4UIcmdWithAString("/run/setMaterial", this);
    fMatCmd->SetGuidance("Set detector material");
    fMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fParticleCmd = new G4UIcmdWithAString("/run/setParticle", this);
    fParticleCmd->SetGuidance("Set primary particle");
    fParticleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fEnergyCmd = new G4UIcmdWithADoubleAndUnit("/run/setEnergy", this);
    fEnergyCmd->SetGuidance("Set beam energy");
    fEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

RunMessenger::~RunMessenger()
{
    delete fMatCmd;
    delete fParticleCmd;
    delete fEnergyCmd;
    delete fDir;
}

void RunMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == fMatCmd)
    {
        fRunAction->SetMaterial(value);
    }
    else if (command == fParticleCmd)
    {
        fRunAction->SetParticle(value);
    }
    else if (command == fEnergyCmd)
    {
        fRunAction->SetEnergy(
            fEnergyCmd->GetNewDoubleValue(value)
        );
    }
}