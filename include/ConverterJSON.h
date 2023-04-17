#pragma once
#include "RelativeIndex.h"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>

/**
 * Функция для создания файла "config.json"
 */
void MakeConfigFile();

/**
 * Функция для создания файла "requests.json"
 */
void MakeRequestsFile();

/**
 * Метод записывает содержимое текстовых файлов в вектор со строками
 * @param textsList - список текстовых файлов
 * @param texts - вектор строк с содержимым текстовых файлов, который возвращает метод GetTextDocuments
 * класса ConverterJSON
 */
void ReadTextFiles(std::vector<std::string> textsList, std::vector<std::string>& texts);

/**
    * Метод, который записывает ответ на запрос в формате json
    * @param answer - ответ на отдельный запрос, элемент вектора, возвращаемого в результате
    * работы метода search класса SearchServer
    * @return - ответ в формате json
    */
nlohmann::json makeJSONAnswer(std::vector<RelativeIndex> answer);

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
public:
    ConverterJSON() = default;

    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    std::vector<std::string> GetTextDocuments();

    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    */
    int GetResponsesLimit();

    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();

    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);
};