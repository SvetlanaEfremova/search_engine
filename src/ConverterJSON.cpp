#include "RelativeIndex.h"
#include "ConverterJSON.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

void ReadTextFiles(std::vector<std::string> textsList, std::vector<std::string>& texts) {
    for (int i = 0; i < textsList.size(); i++) {
        std::ifstream textFile(textsList[i]);
        if (textFile.is_open()){
            std::string text;
            while (!textFile.eof()) {
                getline(textFile, text);
            }
            textFile.close();
            texts.push_back(text);
        }
        else
            std::cerr << "file \"" << textsList[i] << "\" is missing" << std::endl;
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> textsList; //список названий текстовых файлов в файле config
    std::vector<std::string> texts = {}; //возвращаемое содержимое текстовых файлов
    nlohmann::json configJSON;  //переменная, в которую запишем содержимое файла config.json
    std::ifstream configFile("../../config.json");    //открываем файл config.json для чтения
    if (!configFile.is_open())
        throw std::runtime_error("Error: \"config.json\" file is missing");
    configFile >> configJSON;
    if (configJSON["config"] == nullptr || configJSON["files"] == nullptr)
        throw std::runtime_error("Error: \"config.json\" file is invalid");
    textsList = configJSON["files"];
    ReadTextFiles(textsList,texts);    //записываем содержимое текстовых файлов в texts
    return texts;
}

int ConverterJSON::GetResponsesLimit() {
    int responsesLimit = 5;
    nlohmann::json configJSON;
    std::ifstream configFile("../../config.json");
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
    std::ifstream requestsFile("../../requests.json");
    if (requestsFile.is_open()) {
        requestsFile >> requestsJSON;
        if (requestsJSON["requests"] != nullptr)
            requests = requestsJSON["requests"];
    }
    else {
        throw std::runtime_error("Error: \"requests.json\" file is missing");
    }
    return requests;
}

nlohmann::json makeJSONAnswer(std::vector<RelativeIndex> answer) {
    nlohmann::json answerJSON;
    if (!answer.empty()) {
        answerJSON["result"] = true;
        if (answer.size() == 1) {
            answerJSON["docid"] = answer[0].doc_id;
            answerJSON["rank"] = answer[0].rank;
        } else {
            nlohmann::json relevanceForDoc; //релевантность для отдельного документа
            std::vector<nlohmann::json> relevanceForRequest;    //список релевантностей для запроса
            for (int j = 0; j < answer.size(); j++) {
                relevanceForDoc["docid"] = answer[j].doc_id;
                relevanceForDoc["rank"] = answer[j].rank;
                relevanceForRequest.push_back(relevanceForDoc);
            }
            answerJSON["relevance"] = relevanceForRequest;
        }
    } else {
        answerJSON["result"] = false;
    }
    return answerJSON;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
    std::ofstream answersFile("../../answers.json");  //открываем (или создаём) файл answers.json для записи
    if (answersFile.is_open()) {
        nlohmann::json answersJSON; //переменная, в которую запишем содержимое answers
        for (int i = 0; i < answers.size(); i++) {
            nlohmann::json answerJSON = makeJSONAnswer(answers[i]); //ответ на i-й запрос в формате json
            std::string answerId;
            if (i + 1 < 100) {
                if (i + 1 < 10)
                    //прибавляем единицу, чтобы id начинались с 1
                    answerId = "request00" + std::to_string(i + 1);
                else
                    answerId = "request0" + std::to_string(i + 1);
            } else
                answerId = "request" + std::to_string(i + 1);
            answersJSON["answers"][answerId] = answerJSON;
        }
        answersFile << answersJSON;
    }
    else {
        throw std::runtime_error("Error: cannot write in \"answers.json\" file");
    }
}
