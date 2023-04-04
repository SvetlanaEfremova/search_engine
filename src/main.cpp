#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    try {
        ConverterJSON converter;
        InvertedIndex index;
        index.UpdateDocumentBase(converter.GetTextDocuments());
        SearchServer server(index);
        auto answers = server.search(converter.GetRequests());
        converter.putAnswers(answers);
        std::cout << "The search query is complete, the answers are written to \"answers.json\" file" << std::endl;
    }
    catch (const std::exception &ex) {
        std::cerr << ex.what();
    }
    return 0;
}