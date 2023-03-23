#include "../include/RelativeIndex.h"
#include "../include/ConverterJSON.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> textsList; //список названий текстовых файлов в файле config
    std::vector<std::string> texts = {}; //возвращаемое содержимое текстовых файлов
    nlohmann::json configJSON;  //переменная, в которую запишем содержимое файла config.json
    std::ifstream configFile("config.json");    //открываем файл config.json для чтения
    if (configFile.is_open()) {
        configFile >> configJSON;
        if (configJSON["config"] != nullptr && configJSON["files"] != nullptr) {
            textsList = configJSON["files"];
            for (int i = 0; i < textsList.size(); i++) {    //записываем содержимое файлов в texts
                std::ifstream textFile(textsList[i]);
                if (textFile.is_open()) {
                    std::string text;
                    while (!textFile.eof()) {
                        getline(textFile, text);
                    }
                    texts.push_back(text);
                    textFile.close();
                }
                else
                    std::cerr << "file \"" << textsList[i] << "\" is missing" << std::endl;
            }
        }
        else {
            throw std::runtime_error("Error: config file is invalid");
        }
    }
    else {
        throw std::runtime_error("Error: config file is missing");
    }
    return texts;
}

int ConverterJSON::GetResponsesLimit() {
    int responsesLimit = 5;
    nlohmann::json configJSON;
    std::ifstream configFile("config.json");
    if (configFile.is_open()) {
        configFile >> configJSON;
        if (configJSON["config"]["max_responses"] != nullptr)
            responsesLimit = configJSON["config"]["max_responses"];
    }
    return responsesLimit;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests = {}; //возвращаемый список запросов
    nlohmann::json requestsJSON;    //переменная, в которую запишем содержимое файла config.json
    std::ifstream requestsFile("requests.json");
    if (requestsFile.is_open()) {
        requestsFile >> requestsJSON;
        if (requestsJSON["requests"] != nullptr)
            requests = requestsJSON["requests"];
    }
    else {
        throw std::runtime_error("Error: requests file is missing");
    }
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
    std::ofstream answersFile("answers.json");  //открываем (или создаём) файл answers.json для записи
    nlohmann::json answersJSON; //переменная, в которую запишем содержимое answers
    for (int i = 0; i < answers.size(); i++) {
        nlohmann::json answer;
        if (!answers[i].empty()) {
            answer["result"] = true;
            if (answers[i].size() == 1) {
                answer["docid"] = answers[i][0].doc_id;
                answer["rank"] = answers[i][0].rank;
            }
            else {
                nlohmann::json relevanceForDoc; //релевантность для отдельного документа
                std::vector<nlohmann::json> relevanceForRequest;    //список релевантностей для запроса
                for (int j = 0; j < answers[i].size(); j++) {
                    relevanceForDoc["docid"] = answers[i][j].doc_id;
                    relevanceForDoc["rank"] = answers[i][j].rank;
                    relevanceForRequest.push_back(relevanceForDoc);
                }
                answer["relevance"] = relevanceForRequest;
            }
        }
        else {
            answer["result"] = false;
        }
        std::string answerId;
        if (i + 1 < 100) {
            if (i + 1 < 10)
                //прибавляем единицу, чтобы id начинались с 1
                answerId = "request00" + std::to_string(i+1);
            else
                answerId = "request0" + std::to_string(i+1);
        }
        else
            answerId = "request" + std::to_string(i+1);
        answersJSON["answers"][answerId] = answer;
    }
    answersFile << answersJSON;
}
