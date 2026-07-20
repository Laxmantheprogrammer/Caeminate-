#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

class G4VPhysicalVolume;
class SensitiveDetector;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume *Construct() override;
    void ConstructSDandField() override;

    void SetMaterial(G4String name);
    void SetRadiatorSize(G4double x, G4double y, G4double z);
    void SetCheckOverlaps(G4bool check);
    G4double GetShieldFrontFace() const
    {
        return fShieldFrontFace;
    }

private:
    G4LogicalVolume *logicDetector;

    G4Material *fRadiatorMaterial;

    G4double fRadiatorX;
    G4double fRadiatorY;
    G4double fRadiatorZ;

    DetectorMessenger *fMessenger;
    G4bool fCheckOverlaps = true;
    G4double fShieldFrontFace;
};

#endif