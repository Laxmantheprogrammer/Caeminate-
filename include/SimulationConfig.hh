#ifndef SIMULATIONCONFIG_HH
#define SIMULATIONCONFIG_HH

#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class SimulationConfig
{
public:
    static SimulationConfig &Instance()
    {
        static SimulationConfig instance;
        return instance;
    }

    json &GetConfig()
    {
        return config;
    }

    json &Layers()
    {
        return config[currentConfig]["layers"];
    }

    json &Beam()
    {
        return config[currentConfig]["beam"];
    }

    json &Run()
    {
        return config[currentConfig]["run"];
    }
    void SetCurrentConfig(size_t index)
    {
        currentConfig = index;
    }
    size_t CurrentConfig() const
    {
        return currentConfig;
    }
    size_t ConfigCount() const
    {
        return config.size();
    }


    private : SimulationConfig()
    {
        std::ifstream file("config.json");

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open config.json");
        }

        file >> config;
    }

    json config;
    size_t currentConfig = 0;

    SimulationConfig(const SimulationConfig &) = delete;
    SimulationConfig &operator=(const SimulationConfig &) = delete;
};

#endif