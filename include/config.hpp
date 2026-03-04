#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "elements.hpp"

class Config {
public:
    std::string filepath;

    Config(const std::string& path);

    void Load(Elements& elements);
    void Save(Elements& elements);
};

#endif
