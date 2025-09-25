#include <iostream>
#include <unistd.h>
#include "ShowpidCommand.h"

void ShowpidCommand::execute() {
    std::cout << "smash pid is " << getpid() << std::endl;
}