The program performs a search query. Requests texts are taken from the _requests.json_ file.
The search is performed through the text documents in _resources_ folder, passing through those files that are specified in the configuration file _config.json_.

Responses are written to the file named _answers.json_. Field names "request001...003" are request identifiers that correspond to the query order. Answers for the requests that have more than 1 result contain the relevance ("rank") calculated for each relevant document.
"docid" are documents IDs that are formed automatically while performing the search query.

_config.json_ file has 2 fields: "config" and "files". "config" contains:
"name" - name of the program;
"version" - the program version;
"max_responses" - the maximum number of responses for each request - if not specified, it is set to 5.
"files" contains the paths to the resource documents for the search query.


To run this project, one needs a C++ compiler (for example, MinGW), CMake and IDE (JetBrains CLion is recommended).
When you open the project in Clion, choose _SearchEngine|Debug_ to perform a search query. Then press _Build_ (Ctrl+F9) and _Run_ (Shift+F10).
In case _config.json_ and/or _requests.json_ are missing, choose _MakeConfigFiles_ instead of _SearchEngine_ to create the configuration files.