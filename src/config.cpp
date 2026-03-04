#include "config.hpp"
#include <fstream>
#include <iostream>
#include "json/json.h"

using json = nlohmann::json;

Config::Config(const std::string& path) : filepath(path) {}

void Config::Load(Elements& elements) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Save(elements);  
        return;
    }

    json j;
    try {
        file >> j;
        
        if (j.contains("carPosition")) {
            elements.carPosition.x = j["carPosition"][0];
            elements.carPosition.y = j["carPosition"][1];
            elements.carPosition.z = j["carPosition"][2];
        }
        if (j.contains("carRotation")) {
            elements.carRotation.x = j["carRotation"][0];
            elements.carRotation.y = j["carRotation"][1];
            elements.carRotation.z = j["carRotation"][2];
        }
        if (j.contains("carScale")) {
            elements.carScale.x = j["carScale"][0];
            elements.carScale.y = j["carScale"][1];
            elements.carScale.z = j["carScale"][2];
        }

        if (j.contains("conePosition")) {
            elements.conePosition.x = j["conePosition"][0];
            elements.conePosition.y = j["conePosition"][1];
            elements.conePosition.z = j["conePosition"][2];
        }
        if (j.contains("coneRotation")) {
            elements.coneRotation.x = j["coneRotation"][0];
            elements.coneRotation.y = j["coneRotation"][1];
            elements.coneRotation.z = j["coneRotation"][2];
        }
        if (j.contains("coneScale")) {
            elements.coneScale.x = j["coneScale"][0];
            elements.coneScale.y = j["coneScale"][1];
            elements.coneScale.z = j["coneScale"][2];
        }
    } catch (const std::exception& e) {
        // std::cerr << "Error parsing config file: " << e.what() << "\n";
    }
}

void Config::Save(Elements& elements) {
    json j;
    j["carPosition"] = {elements.carPosition.x, elements.carPosition.y, elements.carPosition.z};
    j["carRotation"] = {elements.carRotation.x, elements.carRotation.y, elements.carRotation.z};
    j["carScale"]    = {elements.carScale.x, elements.carScale.y, elements.carScale.z};

    j["conePosition"] = {elements.conePosition.x, elements.conePosition.y, elements.conePosition.z};
    j["coneRotation"] = {elements.coneRotation.x, elements.coneRotation.y, elements.coneRotation.z};
    j["coneScale"]    = {elements.coneScale.x, elements.coneScale.y, elements.coneScale.z};

    std::ofstream file(filepath);
    if (file.is_open()) {
        file << j.dump(4);
        // std::cout << "[Config] Successfully saved configurations to " << filepath << "!" << std::endl;
    } else {
        // std::cerr << "[Config] Error: Could not verify saving for " << filepath << std::endl;
    }
}
