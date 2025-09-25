#include <iostream>
#include <sys/wait.h>

#include "ForegroundCommand.h"

void ForegroundCommand::execute() {
    if(this->getNumOfArgs() > 2){
        std::cerr << "smash error: fg: invalid arguments" << std::endl;
        return;
    }
    else if (this->getNumOfArgs() < 2) {
        this->executeLastId();
        return;
    }
    else {
        this->executeFgByID();
        return;
    }
}

void ForegroundCommand::executeFgByID(){
    std::string job_id_s = this->getCmdArgs()[1];
    try {
        int job_id = stoi(job_id_s);
        if (this->m_small_shell.getSmashJobsList().getJobById(job_id) == nullptr) {
            std::cerr << "smash error: fg: job-id " << job_id << " does not exist" << std::endl;
            return;
        }
        auto job = this->m_small_shell.getSmashJobsList().getJobById(job_id);
        std::cout << job->getCommand() << " " << job->getPid() << std::endl;
        this->m_small_shell.setFgProcPID(job->getPid());

        waitpid(job->getPid(), nullptr, 0);

        this->m_small_shell.getSmashJobsList().removeJobById(job->getJobId());
        if(job_id == this->m_small_shell.getSmashJobsList().getMaxId()){
            this->m_small_shell.getSmashJobsList().updateMaxJobId();
        }
        this->m_small_shell.setFgProcPID(0);
    }
    catch(...){
        std::cerr << "smash error: fg: invalid arguments" << std::endl;
        return;
    }
}

void ForegroundCommand::executeLastId(){
    if(this->m_small_shell.getSmashJobsList().getJobsList().empty()){
        std::cerr << "smash error: fg: jobs list is empty" << std::endl;
        return;
    }
    auto job = this->m_small_shell.getSmashJobsList().getLastJob();
    this->m_small_shell.setFgProcPID(job->getPid());

    std::cout << job->getCommand() << " " << job->getPid() << std::endl;
    waitpid(job->getPid(), nullptr,0);
    this->m_small_shell.getSmashJobsList().removeJobById(job->getJobId());
    this->m_small_shell.getSmashJobsList().updateMaxJobId();

    this->m_small_shell.setFgProcPID(0);
}