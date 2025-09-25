#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <cstring>
#include <memory>

int _findPipePlacement(std::string args[], int numOfArgs);

std::pair<std::string, std::size_t>* _findAndSplitSpecial(std::string args[], std::size_t &numArgs);

std::pair<std::string, std::size_t>* findSpecial(const char* cmdLine);

bool isComplexCommand(std::string args_cmd[], const int numOfArgs);

bool isExternalCommand(const std::string& command);

double get_mem_mb(pid_t pid);

uint64_t get_process_cpu_time(pid_t pid);

uint64_t get_process_cpu_time(pid_t pid);

void remove_env_var(const std::string& varname);

std::vector<char> environ_file_to_vector();

std::vector<std::string> extract_env_var_names(const std::vector<char>& env_file);

void _charPtrArrayToStringArray(char* const src[], std::string dest[],std::size_t n);

void _removeBackgroundSign(char *cmd_line);

bool _isBackgroundComamnd(const char *cmd_line);

std::size_t _parseCommandLine(const char *cmd_line, char **args);

std::string _trim(const std::string &s);

std::string _rtrim(const std::string &s);

std::string _ltrim(const std::string &s);

uint64_t get_total_cpu_time();

std::vector<std::string> _charArrayToVector(char* arr[], std::size_t length);

std::string* _vectorToStringArray(const std::vector<std::string>& vec);

std::vector<std::string> splitLines(const std::string& s);

void readFileContent(const std::string& path, std::string& content);

std::vector<std::string> splitTokens(const std::string& s);



#endif //UTILITIES_H
