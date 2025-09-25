#include <unistd.h>
#include <iostream>
#include "CdCommand.h"

void CdCommand::execute() {
    std::string specialCommand = "-";
    int numOfArgs = this->getNumOfArgs();

    if(numOfArgs > 2){
        std::cerr << "smash error: cd: too many arguments" << std::endl;
    }
    else if(numOfArgs == 2){
        if(this->getCmdArgs()[1] == specialCommand){
            if(this->m_small_shell.getlastWorkingDir() == "\n"){
                std::cerr << "smash error: cd: OLDPWD not set" << std::endl;
            }
            else {
                std::string currDir = getcwd(nullptr, 0); // Dynamically allocate buffer for cwd
                if (chdir(this->m_small_shell.getlastWorkingDir().c_str()) >= 0){ // Make sure the syscall did not fail
                    this->m_small_shell.setlastWorkingDir(currDir);
                }
                else {
                    std::perror("smash error: chdir failed");
                }
            }
        }
        else{
            std::string currDir = getcwd(nullptr, 0); // Dynamically allocate buffer for cwd
            this->m_small_shell.setlastWorkingDir(currDir);
            if (chdir(this->getCmdArgs()[1].c_str()) < 0) {
                std::perror("smash error: chdir failed");
            }
        }
    }
}