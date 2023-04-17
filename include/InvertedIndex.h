#pragma once
#include "Entry.h"
#include <vector>
#include <string>
#include <map>
/**
 * Метод для получения слов из строки - для разбиения документов на отдельные слова при выполнении
 * метода UpdateDocumentBase класса InvertedIndex
 * @param inString - строка с поисковым запросом
 * @return возвращает список слов из inString
 */
std::vector<std::string> GetWordsFromString (std::string inString);
/**
 * Метод для получения уникальных слов из строки - для разбиения поисковых запросов на слова
 * @param inString - строка с поисковым запросом
 * @return возвращает список слов из inString
 */
std::vector<std::string> GetUniqueWordsFromString (std::string inString);

/**
 * Метод добавляет слово в частотный словарь
 * @param freq_dictionary - частотный словарь
 * @param word - слово
 * @param docIndex - номер документа, в котором встретилось это слово
 */
void addWordToDocumentBase(std::map<std::string, std::vector<Entry>>& freq_dictionary, std::string word,
                           int docIndex);

/**
 * Метод для сортировки записей в словаре по возрастанию значения docid
 * @param entries - список документов для отдельного слова
 */
void sortEntriesForWord (std::vector<Entry>& entries);

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    /**
    * Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word);
private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};
