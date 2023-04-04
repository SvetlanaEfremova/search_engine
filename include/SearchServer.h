#pragma once
#include "Entry.h"
#include "InvertedIndex.h"
#include "RelativeIndex.h"
#include <vector>
#include <string>

/**
 * Метод подсчитывает, сколько раз слово суммарно встречается в базе данных
 * @param entriesForWord - данные из словаря для конкретного слова - результат работы функции
 * GetWordCount из класса InvertedIndex
 * @return возвращает сумму всех count в списке entriesForWord
 */
int GetTotalCountForWord(std::vector<Entry> entriesForWord);

/**
 * Метод сортирует список слов из запроса по возрастанию встречаемости в базе данных
 * @param wordList - список слов из запроса, _index - объект класса InvertedIndex, из которого
 * получаем данные о частоте встречаемости слов в базе данных
 */
void ArrangeWordsFromRequest(std::vector<std::string>& wordList, InvertedIndex _index);

/**
 * Метод добавляет ответ для отдельного слова в ответ для отдельного запроса
 * @param answer - ответ на запрос
 * @param entryForWord - данные из словаря для данного слова
 * @param maxAbsRelevance - макс. абс. релевантность для всего поискового запроса
 */
void addAnswerForWord(std::vector<RelativeIndex>& answer, Entry entryForWord, float& maxAbsRelevance);

/**
 * Метод формирует ответ на поисковый запрос
 * @param wordListFromRequest - список уникальных слов из запроса
 * @param _index
 * @param maxAbsRelevance - макс. абс. релевантность для всего поискового запроса
 * @return answer - ответ на отдельный запрос
 */
std::vector<RelativeIndex> getAnswerForRequest(std::vector<std::string>wordListFromRequest, InvertedIndex _index,
                                               float& maxAbsRelevance);

/**
 * Метод сортирует данные в ответе для запроса по убыванию релевантности
 * @param answer - ответ на отдельный запрос
 */
void ArrangeAnswerElements(std::vector<RelativeIndex>& answer);

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
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:
    InvertedIndex _index;
};