#include "JobsCommand.h"

void JobsCommand::execute() {
    this->m_small_shell.getSmashJobsList().printJobsList();
}