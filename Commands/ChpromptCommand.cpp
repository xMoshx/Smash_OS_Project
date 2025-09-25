#include "ChpromptCommand.h"

void ChpromptCommand::execute() {
    std::string cmd_args[COMMAND_MAX_ARGS];
    for(int i = 0; i < COMMAND_MAX_ARGS; ++i){
        cmd_args[i] = this->getCmdArgs()[i];
    }
    if(cmd_args[1].empty()){ //if it's nullptr
        m_small_shell.setPromptName("smash");
    }
    else {
        this->m_small_shell.setPromptName(cmd_args[1]);
    }
}
