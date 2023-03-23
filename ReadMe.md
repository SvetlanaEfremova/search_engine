The program performs a search query. Requests texts are taken from the _requests.json_ file in the _cmake-build-debug/src_ folder.
The search is performed through the text documents in _cmake-build-debug/src/resources_, passing through those files that are specified in the configuration file _cmake-build-debug/config.json_.
Responses are written to the file named _answers.json_ located in the same folder. The answer for each request contains the relevance ("rank") of each document relevant for this request.
The maximum number of results is specified in "config.json" file, if not, this number is set equal to 5.

To run this project, one needs a C++ compiler (for example, MinGW), CMake and IDE (JetBrains CLion is recommended).
When opening the project in CLion, you need to press _Build_ (Ctrl+F9) and _Run_ (Shift+F10).
In case _config.json_ and/or _requests.json_ are missing, un-comment _MakeConfigFile_ and _MakeRequestsFile_ functions in _main.cpp_ section and press _Run_.