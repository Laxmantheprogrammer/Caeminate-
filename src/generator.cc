#include "generator.hh"
#include "SimulationConfig.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
PrimaryGenerator::PrimaryGenerator(DetectorConstruction *detector)
    : fDetector(detector)
{
    fGPS = new G4GeneralParticleSource();

    auto &beam = SimulationConfig::Instance().Beam();

    G4String particle = beam["particle"];
    G4double energy = beam["energy_keV"].get<double>() * keV;

    G4double beamZ = fDetector->GetShieldFrontFace() - 1.0 * mm;
    auto dir = beam["direction"];

    // Particle
    fGPS->SetParticleDefinition(
        G4ParticleTable::GetParticleTable()->FindParticle(particle));

    // Energy
    fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);

    // Position
    fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(
        0.0,
        0.0,
        beamZ));

    // Direction
    fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(
        dir[0].get<double>(),
        dir[1].get<double>(),
        dir[2].get<double>()));
}
PrimaryGenerator::~PrimaryGenerator()
{
    delete fGPS;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fGPS->GeneratePrimaryVertex(anEvent);
}
