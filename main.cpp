#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <thread>
#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

struct Entry {
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

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
    files = { "resources/file001.txt",
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
    std::vector<std::string> GetTextDocuments() {
        std::vector<std::string> textsList; //список названий текстовых файлов в файле config
        std::vector<std::string> texts; //возвращаемое содержимое текстовых файлов
        nlohmann::json configJSON;  //переменная, в которую запишем содержимое файла config.json
        std::ifstream configFile("config.json");    //открываем файл config.json для чтения
        if (configFile.is_open()) {
            configFile >> configJSON;
            if (configJSON["config"] != nullptr) {
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
                throw std::runtime_error("Error: config file is empty");
            }
        }
        else {
            throw std::runtime_error("Error: config file is missing");
        }
        return texts;
    }

    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    */
    int GetResponsesLimit() {
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

    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests() {
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

    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
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
};

/**
 * Метод для получения слов из строки - для разбиения документов на отдельные слова при выполнении
 * метода UpdateDocumentBase класса InvertedIndex
 * @param inString - строка с поисковым запросом
 * @return возвращает список слов из inString
 */
std::vector<std::string> GetWordsFromString (std::string inString) {
    std::vector<std::string> words = {};    //возвращаемый список слов
    //убираем лишние пробелы:
    while (inString.find("  ") != std::string::npos) { //пока в строке есть 2 пробела подряд
        inString.erase(inString.begin() + inString.find("  ")); //убираем эти пробелы
    }
    if (inString[0] == ' ') inString.erase(inString.begin());   //если в начале строки пробел, удаляем его
    if (inString[inString.size()-1] != ' ') inString = inString + " ";  //добавляем пробел в конец строки
    std::string word;
    while (!inString.empty()) {
        word = inString.substr(0,inString.find(" "));
        words.push_back(word);
        inString = inString.substr(inString.find(" ")+1);
    }
    return words;
}

/**
 * Метод для получения уникальных слов из строки - для разбиения поисковых запросов на слова
 * @param inString - строка с поисковым запросом
 * @return возвращает список слов из inString
 */
std::vector<std::string> GetUniqueWordsFromString (std::string inString) {
    std::vector<std::string> words = {};
    //убираем лишние пробелы:
    while (inString.find("  ") != std::string::npos) {  //пока есть 2 пробела подряд
        inString.erase(inString.begin() + inString.find("  "));
    }
    if (inString[0] == ' ') inString.erase(inString.begin());   //если в начале строки пробел, удаляем его
    if (inString[inString.size()-1] != ' ') inString = inString + " ";  //добавляем пробел в конец строки
    std::string word;
    while (!inString.empty()) {
        word = inString.substr(0,inString.find(" "));
        //проверяем, есть ли слово в списке
        bool wordIsInList = false;
        for (auto it = words.begin(); it != words.end(); it++) {
            if (*it == word) {
                wordIsInList = true;
                break;
            }
        }
        if (!wordIsInList)
            words.push_back(word);
        inString = inString.substr(inString.find(" ")+1);
    }
    return words;
}

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs) {
        docs = input_docs;
        auto threads = new std::thread*[docs.size()];   //потоки для работы с текстовыми файлами
        for (int i = 0; i < docs.size(); i++) {
            threads[i] = new std::thread([this,i](){
                std::vector<std::string> wordsFromDoc = GetWordsFromString(docs[i]);
                for (auto word : wordsFromDoc) {
                    bool wordFoundInDictionary = false;
                    //проходим по словарю и проверяем, есть ли такое слово
                    for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); it++) {
                        if (it->first == word) {
                            wordFoundInDictionary = true;
                            bool wordFoundInDocument = false; //встречалось ли уже вслово в этом документе
                            for (auto &entry : it->second) {
                                if (entry.doc_id == i) {
                                    entry.count++;
                                    wordFoundInDocument = true;
                                }
                            }
                            /** если слово не встречалось в этом документе, добавляем документ в список
                            для данного слова */
                            if (!wordFoundInDocument) {
                                Entry newEntry;
                                newEntry.doc_id = i;
                                newEntry.count = 1;
                                it->second.push_back(newEntry);
                            }
                        }
                    }
                    //если слова нет в словаре, добавляем его в словарь
                    if (!wordFoundInDictionary) {
                        Entry newEntry;
                        newEntry.doc_id = i;
                        newEntry.count = 1;
                        std::vector<Entry> entries;
                        entries.push_back(newEntry);
                        freq_dictionary.insert(std::make_pair(word, entries));
                    }
                }
            });
        }
        for (int i = 0; i < docs.size(); i++){
            threads[i]->join();
        }
        delete[] threads;
    };

    /**
    * Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word) {
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};

/**
 * Метод подсчитывает, сколько раз слово суммарно встречается в базе данных
 * @param entriesForWord - данные из словаря для конкретного слова - результат работы функции
 * GetWordCount из класса InvertedIndex
 * @return возвращает сумму всех count в списке entriesForWord
 */
int GetTotalCountForWord(std::vector<Entry> entriesForWord) {
    int count = 0;
    if (!entriesForWord.empty()) {
        for (auto it = entriesForWord.begin(); it != entriesForWord.end(); it++) {
            count += it->count;
        }
    }
    return count;
}

/**
 * Метод сортирует список слов из запроса по возрастанию встречаемости в базе данных
 * @param wordList - список слов из запроса, _index - объект класса InvertedIndex, из которого
 * получаем данные о частоте встречаемости слов в базе данных
 */
void ArrangeWordsFromRequest(std::vector<std::string> &wordList, InvertedIndex _index) {
    for (auto it1 = wordList.begin(); it1 != wordList.end(); it1++) {
        int minCount = GetTotalCountForWord(_index.GetWordCount(*it1));
        auto minIt = it1;
        for (auto it2 = it1; it2 != wordList.end(); it2++) {
            int count = GetTotalCountForWord(_index.GetWordCount(*it2));
            if (count < minCount) {
                minCount = count;
                minIt = it2;
            }
        }
        std::string temp = *it1;
        *it1 = *minIt;
        *minIt = temp;
    }
}

/**
 * Метод сортирует данные в ответе для запроса по убыванию релевантности
 * @param answer - ответ на отдельный запрос
 */
void ArrangeAnswerElements(std::vector<RelativeIndex> &answer) {
    for (auto it1 = answer.begin(); it1 != answer.end(); it1++) {
        float maxRank = it1->rank;
        auto maxIt = it1;
        for (auto it2 = it1; it2 != answer.end(); it2++) {
            if (it2->rank > maxRank) {
                maxRank = it2->rank;
                maxIt = it2;
            }
        }
        auto temp = *it1;
        *it1 = *maxIt;
        *maxIt = temp;
    }
}

class SearchServer {
public:

    /**
    * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */
    SearchServer(InvertedIndex& idx) : _index(idx){ };

    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла
    requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
        std::vector<std::vector<RelativeIndex>> answers; //возвращаемый вектор с ответами для каждого запроса
        /** создаём объект класса ConverterJSON для получения информации
        * о максимальном числе ответов на запрос
        */
        auto converterJson = new ConverterJSON();
        int responsesLimit = converterJson->GetResponsesLimit();
        delete converterJson;
        for (int i = 0; i < queries_input.size(); i++) {    //проходим по запросам
            std::vector<RelativeIndex> answer = {}; //ответ для i-го запроса, который добавим в answers
            std::string request = queries_input[i]; //строка с текстом запроса
            //список уникальных слов из запроса:
            std::vector<std::string> wordListFromRequest = GetUniqueWordsFromString(request);
            //упорядочить слова в соответствии с частотой встречаемости в документах
            ArrangeWordsFromRequest(wordListFromRequest, _index);
            float maxAbsRelevance = 0;
            for (auto word : wordListFromRequest) {    //проходим по списку слов из запроса
                //проходим по данным из словаря для данного слова
                for (auto entryForWord : _index.GetWordCount(word)) {
                    //проверяем, есть ли данный документ в ответе для данного запроса
                    bool docFoundInAnswer = false;
                    for (auto &answerElement : answer) {
                        if (answerElement.doc_id == entryForWord.doc_id) {
                            docFoundInAnswer = true;
                            //если документ уже есть в ответе, увеличиваем его rank
                            answerElement.rank += (float)entryForWord.count;
                            if (answerElement.rank > maxAbsRelevance) {
                                maxAbsRelevance = answerElement.rank;
                            }
                            break;
                        }
                    }
                    //если данного документа нет, добавляем его в ответ
                    if (!docFoundInAnswer && answer.size() < responsesLimit) {
                        RelativeIndex newIndex = {entryForWord.doc_id,(float)entryForWord.count};
                        answer.push_back(newIndex);
                        if ((float)entryForWord.count > maxAbsRelevance) {
                            maxAbsRelevance = (float)entryForWord.count;
                        }
                    }
                }
            }
            //считаем относительную релевантость
            if (maxAbsRelevance != 0) {
                for (auto &answerElement : answer) {
                    answerElement.rank = answerElement.rank / maxAbsRelevance;
                }
            }
            ArrangeAnswerElements(answer);
            answers.push_back(answer);
        }
        return answers;
    }
private:
    InvertedIndex _index;
};

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