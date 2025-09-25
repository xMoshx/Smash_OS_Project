#include <unistd.h>
#include <sys/wait.h>

#include "ExternalCommand.h"
#include "JobsList.h"
#include "Utilities.h"

void ExternalCommand::execute(){
    int num_of_args = this->getNumOfArgs();
    bool bg_cmd = this->getIsBgCmd();
    bool complex = isComplexCommand(this->getCmdArgs(), num_of_args);

    pid_t pid = fork();
    if(pid < 0){
        std::perror("smash error: fork failed");
        return;
    }

    // Child process
    if(pid == 0){
        setpgrp(); // Cannot fail (based on man page)
        if(!complex) {

            // Simple command - execute directly
            std::vector<char*> args;
            args.reserve(num_of_args + 1);
            for (int i = 0; i < num_of_args; ++i) {
                args.push_back(const_cast<char*>(this->getCmdArgs()[i].c_str()));
            }
            args.push_back(nullptr);
            execvp(args[0], args.data());
            std::perror("smash error: execvp failed");
        }
        else{

            // Complex command - execute using bash -c "cmd arg1 arg2 ..."
            std::string cmd_line;
            for(int i = 0; i < num_of_args - 1; i++){
                cmd_line += this->getCmdArgs()[i] + " ";
            }
            cmd_line += this->getCmdArgs()[num_of_args - 1];

            const char* bash = "/bin/bash";
            const char* args[] = {"bash", "-c", cmd_line.c_str(), nullptr};

            execvp(bash, const_cast<char* const*>((args)));

            // Should not reach here unless execvp failed
            std::perror("smash error: execvp failed");
        }
    }

    // Parent process
    if(pid > 0){
        this->m_shell.setFgProcPID(pid);
        if (!bg_cmd) {

            // Foreground command - wait for it to finish
            if(waitpid(pid, nullptr, 0 ) < 0){
                std::perror("smash error: waitpid failed");
            }
        }
        else{

            // Background command - do not wait and add to jobs list
            this->m_shell.getSmashJobsList().removeFinishedJobs();
            this->m_shell.getSmashJobsList().addJob(m_og_cmd, pid);
        }

        this->m_shell.setFgProcPID(0);
    }
}
