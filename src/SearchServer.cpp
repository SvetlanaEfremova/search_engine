#include "../include/Entry.h"
#include "../include/SearchServer.h"
#include "../include/ConverterJSON.h"
#include <vector>

int GetTotalCountForWord(std::vector<Entry> entriesForWord) {
    int count = 0;
    if (!entriesForWord.empty()) {
        for (auto it = entriesForWord.begin(); it != entriesForWord.end(); it++) {
            count += it->count;
        }
    }
    return count;
}

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

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
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

