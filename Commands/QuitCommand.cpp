#include <iostream>
#include <signal.h>
#include "QuitCommand.h"

void QuitCommand::execute() {
    if(this->getCmdArgs()[1] == "kill") {
        this->m_small_shell.getSmashJobsList().killAllJobs();
    }
    exit(SIGKILL);
}