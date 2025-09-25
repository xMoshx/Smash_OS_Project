#include <iostream>
#include "sys/wait.h"
#include "JobsList.h"

int JobsList::getMaxId() const {
    return this->max_id;
}

void JobsList::setMaxId(int max_id) {
    this->max_id = max_id;
}

std::vector<JobsList::JobEntry>& JobsList::getJobsList() {
    return this->m_jobs;
}

void JobsList::addJob(const char* cmd, pid_t pid) {
    int new_id = this->getMaxId() > 0 ? this->getMaxId() + 1 : 1;
    JobEntry new_job = JobEntry(new_id, pid, cmd);
    this->getJobsList().push_back(new_job);
    this->setMaxId(new_id);
}

void JobsList::printJobsList() {
    this->removeFinishedJobs();
    for (const JobEntry& job : this->getJobsList()) {
        std::cout <<"[" << job.getJobId() << "] " << job.getCommand() << std::endl;
    }
}

void JobsList::killAllJobs() {
    std::cout << "smash: sending SIGKILL signal to " << this->getJobsList().size() << " jobs:" << std::endl;
    for(const auto& job : this->getJobsList()){
        std::cout << job.getPid() <<": " << job.getCommand() << std::endl;
        if(kill(job.getPid(), SIGKILL)){
            std::perror("smash error: kill failed");
        }
    }
}

void JobsList::removeFinishedJobs() {
    std::vector<JobEntry>& jobs_list = this->getJobsList();
    for (auto job_it = jobs_list.begin(); job_it < jobs_list.end();) {
        int ret = waitpid(job_it->getPid(), nullptr, WNOHANG);
        if (ret > 0) {
            jobs_list.erase(job_it);
            if(job_it->getJobId() == this->getMaxId()){
                updateMaxJobId();
            }
        }
        else {
            ++job_it;
        }
    }
}

JobsList::JobEntry* JobsList::getJobById(int jobId) {
    for (JobEntry& job : this->getJobsList()) {
        if (job.getJobId() == jobId) {
            return &job;
        }
    }
    return nullptr;
}

void JobsList::removeJobById(int jobId) {
    for (auto it = this->getJobsList().begin(); it < this->getJobsList().end(); ++it) {
        if (it->getJobId() == jobId) {
            this->getJobsList().erase(it);
        }
    }
}

JobsList::JobEntry* JobsList::getLastJob() {
    return &*(this->getJobsList().end() - 1);
}

//TODO:: implement?
JobsList::JobEntry* JobsList::getLastStoppedJob(int *jobId){};

void JobsList::updateMaxJobId(){
    this->setMaxId(this->getLastJob()->getJobId());
}