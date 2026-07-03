#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGenerator();
    ~PrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4GeneralParticleSource* fGPS;
};

#endif