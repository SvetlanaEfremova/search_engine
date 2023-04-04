#include "MakeRequestsFile.h"
#include "nlohmann/json.hpp"
#include <fstream>

void MakeRequestsFile() {
    nlohmann::json requests;
    requests["requests"] = {"the","it was going to rain","sometimes","had","at that moment"};
    std::ofstream requestsFile("../requests.json");    //открываем файл для записи
    requestsFile << requests;   //записываем запросы в файл
}