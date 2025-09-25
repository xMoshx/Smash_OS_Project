#include "Command.h"

Command::Command(std::string cmd_args[COMMAND_MAX_ARGS], int num_of_args) {
    for(int i = 0; i < COMMAND_MAX_ARGS; ++i){
        this->m_cmd_args[i] = cmd_args[i];
    }
    this->m_num_of_args = num_of_args;
}

std::string* Command::getCmdArgs() {
    return this->m_cmd_args;
}

int Command::getNumOfArgs() const {
    return this->m_num_of_args;
}