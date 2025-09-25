#include <iostream>
#include <signal.h>
#include "KillCommand.h"

void KillCommand::execute() {
    if(this->getNumOfArgs() != 3){
        std::cerr << "smash error: kill: invalid arguments" << std::endl;
        return;
    }

    if(this->getCmdArgs()[1][0] == '-'){
        try{
            int signum = atoi(const_cast<char*>(&this->getCmdArgs()[1][1]));
            int jobId = stoi(this->getCmdArgs()[2]);
            if(jobId <= 0){
                std::cerr << "smash error: kill: invalid arguments" << std::endl;
                return;
            }
            auto jobEntry = this->m_small_shell.getSmashJobsList().getJobById(jobId);
            if(jobEntry == nullptr){
                std::cout << "smash error: kill: job-id " << jobId << " does not exist" << std::endl;
                return;
            }
            int jobPid = jobEntry->getPid();
            if(!kill(jobPid, signum)){
                std::cout << "signal number " << signum << " was sent to pid " << jobPid << std::endl;
                return;
            }
            else{
                perror("smash error: kill failed");
            }

        } catch(...){
            std::cerr << "smash error: kill: invalid arguments" << std::endl;
            return;
        }
    }
    else{
        std::cerr << "smash error: kill: invalid arguments" << std::endl;
        return;
    }
}