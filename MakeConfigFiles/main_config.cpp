#include "MakeConfigFile.h"
#include "MakeRequestsFile.h"
#include <iostream>

int main() {
    MakeConfigFile();
    MakeRequestsFile();
    std::cout << "\"Config.json\" and \"Requests.json\" files are created" << std::endl;
    return 0;
}