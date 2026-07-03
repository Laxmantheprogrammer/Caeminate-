#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"

class DetectorConstruction;

class DetectorMessenger : public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction *);
    ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String);

private:
    DetectorConstruction *fDetector;

    G4UIdirectory *detectorDir;

    G4UIcmdWithAString *materialCmd;

    G4UIcmdWith3VectorAndUnit *setSizeCmd;
    G4UIcmdWithABool *checkOverlapCmd;
};

#endif