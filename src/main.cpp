#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

/**
 * Функция для создания файла "config.json"
 */
void MakeConfigFile () {
    nlohmann::json configJSON;
    nlohmann::json config;
    config["name"] = "SkillboxSearchEngine";
    config["version"] = "0.1";
    config["max_responses"] = 5;
    nlohmann::json files;
    files = {"resources/file001.txt",
             "resources/file002.txt",
             "resources/file003.txt",
             "resources/file004.txt",
             "resources/file005.txt",
             "resources/file006.txt",
             "resources/file007.txt"};
    configJSON["config"] = config;
    configJSON["files"] = files;
    std::ofstream configFile("config.json");    //открываем файл config.json для записи
    configFile << configJSON;   //записываем данные в файл
}

/**
 * Функция для создания файла "requests.json"
 */
void MakeRequestsFile() {
    nlohmann::json requests;
    requests["requests"] = {"the","it was going to rain","sometimes","had","at that moment"};
    std::ofstream requestsFile("requests.json");    //открываем файл для записи
    requestsFile << requests;   //записываем запросы в файл
}

int main() {
    /**
     * Функции MakeConfigFile и MakeRequestsFile вызываются в случае
     * отсутствия файлов config.json и requests.json
     */
    //MakeConfigFile();
    //MakeRequestsFile();
    ConverterJSON converter;
    InvertedIndex index;
    try {
        index.UpdateDocumentBase(converter.GetTextDocuments());
        SearchServer server(index);
        auto answers = server.search(converter.GetRequests());
        converter.putAnswers(answers);
    }
    catch (const std::exception &ex) {
        std::cerr << ex.what();
    }
    return 0;
}