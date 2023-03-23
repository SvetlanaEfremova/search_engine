#include "../include/Entry.h"
#include "../include/InvertedIndex.h"
#include <thread>
#include <vector>

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

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    auto threads = new std::thread *[docs.size()];   //потоки для работы с текстовыми файлами
    for (int i = 0; i < docs.size(); i++) {
        threads[i] = new std::thread([this, i]() {
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
    for (int i = 0; i < docs.size(); i++) {
        threads[i]->join();
    }
    delete[] threads;
};

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    return freq_dictionary[word];
}



