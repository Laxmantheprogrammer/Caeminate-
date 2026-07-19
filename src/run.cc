#include "run.hh"
#include "RunMessenger.hh"
#include "G4RootAnalysisManager.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "SimulationConfig.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <sstream>
RunAction *RunAction::fInstance = nullptr;
RunAction *RunAction::GetInstance()
{
    return fInstance;
}
RunAction::RunAction()
{
    G4cout << "=================================" << G4endl;
    G4cout << "RUNACTION CONSTRUCTOR CALLED" << G4endl;
    G4cout << "=================================" << G4endl;

    fMessenger = new RunMessenger(this);
    RunAction::fInstance = this;

    auto *man = G4RootAnalysisManager::Instance();

    man->CreateNtuple("Hits", "Particle Hits");

    // Run information
    man->CreateNtupleIColumn("RunID");

    // Track information
    man->CreateNtupleIColumn("TrackID");
    man->CreateNtupleIColumn("ParentID");

    // Particle information
    man->CreateNtupleSColumn("ParticleName");
    man->CreateNtupleDColumn("KineticEnergy_MeV");
    man->CreateNtupleDColumn("EnergyDeposit_MeV");

    // Position
    man->CreateNtupleDColumn("X_mm");
    man->CreateNtupleDColumn("Y_mm");
    man->CreateNtupleDColumn("Z_mm");
    man->FinishNtuple();

    // =====================
    // Metadata tree
    // =====================
    man->CreateNtuple("Metadata", "Run Metadata");

    man->CreateNtupleIColumn("RunID");
    man->CreateNtupleSColumn("Particle");
    man->CreateNtupleDColumn("Energy_keV");
    man->CreateNtupleIColumn("NumLayers");

    // simple stack strings
    man->CreateNtupleSColumn("LayerStack");
    man->CreateNtupleSColumn("ThicknessStack");
    man->CreateNtupleSColumn("DensityStack");

    man->FinishNtuple();
    man->CreateNtuple("Summary", "Run Summary");

    man->CreateNtupleIColumn("RunID");

    man->CreateNtupleDColumn("TransmissionFraction");
    man->CreateNtupleDColumn("SecondaryCount");
    man->CreateNtupleDColumn("TotalEnergyDeposit_MeV");

    man->FinishNtuple();
}

RunAction::~RunAction()
{
    delete fMessenger;
}

void RunAction::SetMaterial(G4String mat)
{
    fMaterial = mat;
}

void RunAction::SetParticle(G4String p)
{
    fParticle = p;
}

void RunAction::SetEnergy(G4double e)
{
    fEnergy = e;
}
void RunAction::AddEnergyDeposit(G4double edep)
{
    fTotalEnergyDeposit += edep;
}
void RunAction::IncrementTransmittedParticles()
{
    G4cout << "Transmitted count = "
           << fTransmittedParticles + 1
           << G4endl;
    ++fTransmittedParticles;
}

void RunAction::SetTotalEvents(G4int n)
{
    fTotalEvents = n;
}

G4int RunAction::GetTotalEvents() const
{
    return fTotalEvents;
}

G4int RunAction::GetTransmittedParticles() const
{
    return fTransmittedParticles;
}
void RunAction::BeginOfRunAction(const G4Run *)
{
    fTotalEnergyDeposit = 0.0;
    fTransmittedParticles = 0;
    auto *man = G4RootAnalysisManager::Instance();

    man->OpenFile();

    auto &config = SimulationConfig::Instance();
    auto &layers = config.Layers();
    auto &beam = config.Beam();

    fRunID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();

    std::string layerStr, thickStr, densStr;

    auto *nist = G4NistManager::Instance();

    for (size_t i = 0; i < layers.size(); i++)
    {
        std::string materialName =
            layers[i]["material"].get<std::string>();

        layerStr += materialName;

        thickStr +=
            std::to_string(
                layers[i]["thickness_mm"].get<double>());

        G4Material *mat =
            nist->FindOrBuildMaterial(materialName);

        densStr +=
            std::to_string(mat->GetDensity() / (g / cm3));

        if (i != layers.size() - 1)
        {
            layerStr += "|";
            thickStr += "|";
            densStr += "|";
        }
    }

    man->FillNtupleIColumn(1, 0, fRunID);
    man->FillNtupleSColumn(1, 1, beam["particle"].get<std::string>());
    man->FillNtupleDColumn(1, 2, beam["energy_keV"].get<double>());
    man->FillNtupleIColumn(1, 3, layers.size());

    man->FillNtupleSColumn(1, 4, layerStr);
    man->FillNtupleSColumn(1, 5, thickStr);
    man->FillNtupleSColumn(1, 6, densStr);

    man->AddNtupleRow(1);
}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto *man = G4RootAnalysisManager::Instance();

    G4double transmission = 0.0;

    if (fTotalEvents > 0)
    {
        transmission =
            static_cast<G4double>(fTransmittedParticles) /
            static_cast<G4double>(fTotalEvents);
    }

    man->FillNtupleIColumn(2, 0, fRunID);
    man->FillNtupleDColumn(2, 1, transmission);
    man->FillNtupleDColumn(2, 2, 0.0); // SecondaryCount (placeholder)
    man->FillNtupleDColumn(2, 3, fTotalEnergyDeposit);

    man->AddNtupleRow(2);

    man->Write();
    man->CloseFile();
}