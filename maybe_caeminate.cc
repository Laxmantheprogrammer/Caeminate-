#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4RootAnalysisManager.hh"
#include "DetectorConstruction.hh"
#include "physics.hh"
#include "action.hh"
#include "SimulationConfig.hh"
#include "FTFP_BERT.hh"

int main(int argc, char **argv)
{
    G4UIExecutive *ui = nullptr;

    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    auto *runManager = new G4RunManager();
    
    G4VModularPhysicsList *physicsList = new FTFP_BERT;
    // ---------------- CORE GEOMETRY ----------------
    auto *detector = new DetectorConstruction();
    runManager->SetUserInitialization(detector);

    // ---------------- PHYSICS ----------------
    runManager->SetUserInitialization(new PhysicsList());

    // ---------------- ACTIONS (NOW PROPERLY WIRED) ----------------
    runManager->SetUserInitialization(
        new ActionInitialization(detector));

    auto analysisManager = G4RootAnalysisManager::Instance();
    runManager->Initialize();

    // ---------------- VIS ----------------
    auto *visManager = new G4VisExecutive();
    visManager->Initialize();

    auto *UImanager = G4UImanager::GetUIpointer();

    for (size_t i = 0; i < SimulationConfig::Instance().ConfigCount(); ++i)
    {
        SimulationConfig::Instance().SetCurrentConfig(i);
        runManager->ReinitializeGeometry();

        auto &run = SimulationConfig::Instance().Run();
        int nEvents = run["events"].get<int>();
        analysisManager->SetFileName("../output/Output_" + std::to_string(i));

        runManager->BeamOn(nEvents);
    }

    if (ui)
    {
        ui->SessionStart();
    }

    delete ui;
    delete visManager;
    delete runManager;

    return 0;
}