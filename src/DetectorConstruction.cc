#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "DetectorMessenger.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "SimulationConfig.hh"

DetectorConstruction::DetectorConstruction()
{
    fRadiatorMaterial = nullptr;

    fRadiatorX = 80 * cm;
    fRadiatorY = 80 * cm;
    fRadiatorZ = 2 * cm;

    fMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    auto nist = G4NistManager::Instance();
    auto &layers = SimulationConfig::Instance().Layers();
    G4double totalThickness = 0;

    for (const auto &layer : layers)
    {
        totalThickness += layer["thickness_mm"].get<double>() * mm;
    }

    auto firstLayer = layers[0];

    G4String materialName = firstLayer["material"];

    G4double firstThickness =
        firstLayer["thickness_mm"].get<double>() * mm;

    G4Material *firstMaterial =
        nist->FindOrBuildMaterial(materialName);

    // ---------------- WORLD ----------------

    G4Material *worldMat =
        nist->FindOrBuildMaterial("G4_AIR");

    G4Box *solidWorld =
        new G4Box("World",
                  0.5 * m,
                  0.5 * m,
                  0.5 * m);

    G4LogicalVolume *logicWorld =
        new G4LogicalVolume(
            solidWorld,
            worldMat,
            "WorldLV");

    // ---------------- MATERIAL SYSTEM ----------------

    // ---------------- MATERIAL SYSTEM ----------------

    G4Material *SiO2 = G4Material::GetMaterial("SiO2");
    if (!SiO2)
    {
        SiO2 = new G4Material("SiO2", 2.201 * g / cm3, 2);
        SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
        SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
    }

    G4Material *H2O = G4Material::GetMaterial("H2O");
    if (!H2O)
    {
        H2O = new G4Material("H2O", 1.0 * g / cm3, 2);
        H2O->AddElement(nist->FindOrBuildElement("H"), 2);
        H2O->AddElement(nist->FindOrBuildElement("O"), 1);
    }

    G4Material *Aerogel = G4Material::GetMaterial("Aerogel");
    if (!Aerogel)
    {
        G4Element *C = nist->FindOrBuildElement("C");

        Aerogel = new G4Material("Aerogel", 0.2 * g / cm3, 3);
        Aerogel->AddMaterial(SiO2, 62.5 * perCent);
        Aerogel->AddMaterial(H2O, 37.4 * perCent);
        Aerogel->AddElement(C, 0.1 * perCent);
    }

    // ---------------- OPTICAL PROPERTIES ----------------

    G4double energy[2] =
        {
            1.239841939 * eV / 0.9,
            1.239841939 * eV / 0.2};

    G4double rindexAerogel[2] = {1.1, 1.1};
    G4double rindexWorld[2] = {1.0, 1.0};

    auto *mptA =
        new G4MaterialPropertiesTable();

    mptA->AddProperty(
        "RINDEX",
        energy,
        rindexAerogel,
        2);

    Aerogel->SetMaterialPropertiesTable(mptA);

    auto *mptW =
        new G4MaterialPropertiesTable();

    mptW->AddProperty(
        "RINDEX",
        energy,
        rindexWorld,
        2);

    worldMat->SetMaterialPropertiesTable(mptW);

    // ---------------- RADIATOR ----------------
    G4double currentZ =
        (0.25 * m) - (totalThickness / 2);
    fShieldFrontFace = currentZ;

    for (const auto &layer : layers)
    {
        G4String materialName = layer["material"];

        G4Material *radiatorMaterial =
            nist->FindOrBuildMaterial(materialName);

        G4double thickness =
            layer["thickness_mm"].get<double>() * mm;

        // Everything below stays unchanged for now...
        G4Box *solidRadiator =
            new G4Box(
                "Radiator",
                fRadiatorX / 2,
                fRadiatorY / 2,
                thickness / 2);

        G4LogicalVolume *logicRadiator =
            new G4LogicalVolume(
                solidRadiator,
                radiatorMaterial,
                "RadiatorLV");

        new G4PVPlacement(
            0,
            G4ThreeVector(0, 0, currentZ + thickness / 2),
            logicRadiator,
            "Radiator",
            logicWorld,
            false,
            0,
            fCheckOverlaps);

        G4cout
            << "Radiator material = "
            << radiatorMaterial->GetName()
            << G4endl;
        currentZ += thickness;
    }

    // ---------------- DETECTOR GRID ----------------

    G4Box *solidDetector =
        new G4Box(
            "Detector",
            0.005 * m,
            0.005 * m,
            0.01 * m);

    logicDetector =
        new G4LogicalVolume(
            solidDetector,
            worldMat,
            "DetectorLV");

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            new G4PVPlacement(
                0,
                G4ThreeVector(
                    -0.5 * m + (i + 0.5) * m / 100,
                    -0.5 * m + (j + 0.5) * m / 100,
                    currentZ + 1 * mm),
                logicDetector,
                "Detector",
                logicWorld,
                false,
                j + i * 100,
                fCheckOverlaps);
        }
    }

    // ---------------- WORLD PLACEMENT ----------------

    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(
            0,
            {},
            logicWorld,
            "World",
            0,
            false,
            0,
            fCheckOverlaps);

    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    auto *sens =
        new SensitiveDetector(
            "SensitiveDetector");

    logicDetector->SetSensitiveDetector(sens);
}

void DetectorConstruction::SetMaterial(G4String name)
{
    auto nist =
        G4NistManager::Instance();

    if (name == "Aerogel")
    {
        fRadiatorMaterial = nullptr;
    }
    else
    {
        fRadiatorMaterial =
            nist->FindOrBuildMaterial(name);
    }

    G4RunManager::GetRunManager()
        ->ReinitializeGeometry();

    G4cout
        << "Radiator material set to: "
        << name
        << G4endl;
}

void DetectorConstruction::SetCheckOverlaps(G4bool check)
{
    fCheckOverlaps = check;

    G4cout
        << "Geometry overlap checking "
        << (check ? "ENABLED" : "DISABLED")
        << G4endl;

    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetRadiatorSize(
    G4double x,
    G4double y,
    G4double z)
{
    fRadiatorX = x;
    fRadiatorY = y;
    fRadiatorZ = z;

    G4RunManager::GetRunManager()
        ->ReinitializeGeometry();

    G4cout
        << "Radiator size set to: "
        << fRadiatorX / cm << " x "
        << fRadiatorY / cm << " x "
        << fRadiatorZ / cm << " cm"
        << G4endl;
}