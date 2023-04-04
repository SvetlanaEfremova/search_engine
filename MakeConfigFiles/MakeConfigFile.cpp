#include "MakeConfigFile.h"
#include "nlohmann/json.hpp"
#include <fstream>

void MakeConfigFile () {
    nlohmann::json configJSON;
    nlohmann::json config;
    config["name"] = "SkillboxSearchEngine";
    config["version"] = "1.0";
    config["max_responses"] = 5;
    nlohmann::json files;
    files = {"../../resources/file001.txt",
             "../../resources/file002.txt",
             "../../resources/file003.txt",
             "../../resources/file004.txt",
             "../../resources/file005.txt",
             "../../resources/file006.txt",
             "../../resources/file007.txt"};
    configJSON["config"] = config;
    configJSON["files"] = files;
    std::ofstream configFile("../config.json");    //открываем файл config.json для записи
    configFile << configJSON;   //записываем данные в файл
}