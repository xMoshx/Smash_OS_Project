#ifndef JOBSLIST_H
#define JOBSLIST_H

#include <string>
#include <vector>


class JobsList {
    class JobEntry {
        int m_jobId;
        std::string m_command;
        pid_t m_pid;

    public:
        JobEntry(const int& jobId, pid_t pid, const char* command){
            this->m_jobId = jobId;
            this->m_command = command;
            this->m_pid = pid;
        };
        ~JobEntry() = default;

        int getJobId() const {
            return this->m_jobId;
        }

        int getPid() const {
            return this->m_pid;
        }

        std::string getCommand() const {
            return this->m_command;
        }
    };

    std::vector<JobEntry> m_jobs;
    int max_id = 0;

public:
    JobsList() = default;

    ~JobsList() = default;

    int getMaxId() const;

    void setMaxId(int max_id);

    std::vector<JobEntry>& getJobsList();

    void addJob(const char* cmd, pid_t pid);

    void printJobsList();

    void killAllJobs();

    void removeFinishedJobs();

    JobEntry *getJobById(int jobId);

    void removeJobById(int jobId);

    JobEntry *getLastJob();

    JobEntry *getLastStoppedJob(int *jobId);

    void updateMaxJobId();
};

#endif //JOBSLIST_H
