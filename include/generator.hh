#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "DetectorConstruction.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGenerator(DetectorConstruction* detector);
    ~PrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4GeneralParticleSource* fGPS;
    DetectorConstruction* fDetector;
};

#endif