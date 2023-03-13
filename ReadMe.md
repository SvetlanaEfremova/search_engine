The program performs a search query. Requests texts are taken from the "requests.json" file.
The search is performed through the documents in the "resources" folder, passing through those files that are specified in the configuration file "config.json".
Responses are written to the file named "answers.json". The answer for each request contains the relevance ("rank") of each document relevant for this request.
The maximum number of results is specified in "config.json" file, if not, this number is set equal to 5.

To run this project, one needs a C++ compiler (for example, MinGW), CMake and IDE (JetBrains CLion is recommended).
When opening the project in CLion, you need to press "Build" (Ctrl+F9) and "Run" (Shift+F10).
In case "config.json" and/or "requests.json" are missing, un-comment "MakeConfigFile" and "MakeRequestsFile" functions in "main" section and press "Run".