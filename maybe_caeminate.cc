#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4RootAnalysisManager.hh"

#include "DetectorConstruction.hh"
#include "physics.hh"
#include "action.hh"
#include "SimulationConfig.hh"

#include <iomanip>
#include <iostream>

void PrintProgress(std::size_t current, std::size_t total)
{
    const int barWidth = 50;
    double progress = static_cast<double>(current) / total;

    std::cout << "\rProgress: [";

    int pos = static_cast<int>(barWidth * progress);

    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }

    std::cout << "] "
              << std::setw(3)
              << static_cast<int>(progress * 100)
              << "% (" << current << "/" << total << ")"
              << std::flush;
}

int main(int argc, char **argv)
{
    // ============================================================
    // USER SETTINGS
    // ============================================================

    const bool VISUALIZATION = true;

    // ============================================================
    // RUN MANAGER
    // ============================================================

    auto *runManager = new G4RunManager();

    auto *detector = new DetectorConstruction();

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization(detector));

    auto *analysisManager = G4RootAnalysisManager::Instance();

    auto *UImanager = G4UImanager::GetUIpointer();

    // ============================================================
    // VISUALIZATION
    // ============================================================

    G4VisManager *visManager = nullptr;
    G4UIExecutive *ui = nullptr;

    if (VISUALIZATION)
    {
        visManager = new G4VisExecutive();
        visManager->Initialize();
    }

    // Initialize Geant4 after everything has been registered
    runManager->Initialize();

    if (VISUALIZATION)
    {
        ui = new G4UIExecutive(argc, argv);

        UImanager->ApplyCommand("/control/execute vis.mac");
    }

    // ============================================================
    // RUN ALL CONFIGURATIONS
    // ============================================================

    const std::size_t totalConfigs =
        SimulationConfig::Instance().ConfigCount();

    for (std::size_t i = 0; i < totalConfigs; ++i)
    {
        SimulationConfig::Instance().SetCurrentConfig(i);

        runManager->ReinitializeGeometry();

        auto &run = SimulationConfig::Instance().Run();
        int nEvents = run["events"].get<int>();

        analysisManager->SetFileName(
            "../output/Output_" + std::to_string(i));

        runManager->BeamOn(nEvents);

        PrintProgress(i + 1, totalConfigs);
        std::cout << std::endl;
    }

    // ============================================================
    // KEEP THE QT WINDOW OPEN
    // ============================================================

    if (VISUALIZATION)
    {
        ui->SessionStart();

        delete ui;
        delete visManager;
    }

    delete runManager;

    return 0;
}