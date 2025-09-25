#include <unistd.h>
#include <iostream>
#include "PwdCommand.h"

void PwdCommand::execute() {
    char* cwd = getcwd(nullptr, 0); // Dynamically allocates space for cwd
    std::cout << cwd << std::endl;
}
