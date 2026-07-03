#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction *det)
    : fDetector(det)
{
    detectorDir = new G4UIdirectory("/detector/");
    detectorDir->SetGuidance("Detector control");

    materialCmd =
        new G4UIcmdWithAString("/detector/setMaterial", this);

    materialCmd->SetGuidance("Set radiator material");
    materialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    setSizeCmd =
        new G4UIcmdWith3VectorAndUnit("/detector/setSize", this);

    setSizeCmd->SetGuidance("Set radiator size (X Y Z)");
    setSizeCmd->SetUnitCategory("Length");
    setSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    checkOverlapCmd =
        new G4UIcmdWithABool("/detector/checkOverlaps", this);

    checkOverlapCmd->SetGuidance("Enable or disable geometry overlap checking");
    checkOverlapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
    delete materialCmd;
    delete setSizeCmd;
    delete checkOverlapCmd;
    delete detectorDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
    if (command == materialCmd)
    {
        fDetector->SetMaterial(value);
    }
    else if (command == setSizeCmd)
    {
        auto v = setSizeCmd->GetNew3VectorValue(value);

        fDetector->SetRadiatorSize(
            v.x(),
            v.y(),
            v.z());
    }
    else if (command == checkOverlapCmd)
    {
        fDetector->SetCheckOverlaps(
            checkOverlapCmd->GetNewBoolValue(value));
    }
}