#include <unistd.h>
#include <sys/wait.h>

#include "PipeCommand.h"

void PipeCommand::execute() {
    if (!this->getIsErr()) {
        this->executePipeHandler(STDOUT_FILENO);
    }
    else {
        this->executePipeHandler(STDERR_FILENO);
    }
}

void PipeCommand::executePipeHandler(int fd){
    int my_pipe[2];
    pipe(my_pipe);

    pid_t pid = fork();
    if (pid == 0) {
        close(my_pipe[0]);
        std::string cmd_1_args;
        for (int i = 0; i < getPipePlace(); i++) {
            cmd_1_args += this->getCmdArgs()[i] + " ";
        }

        if (dup2(my_pipe[1], fd) < 0) {
            std::perror("smash error: dup2 failed");
        }
        std::unique_ptr<Command> cmd_1 = this->m_small_shell.CreateCommand(cmd_1_args.c_str());
        if (cmd_1 != nullptr) {
            cmd_1->execute();
        }

        this->m_small_shell.restoreStdOut();
        this->m_small_shell.restoreStdErr();
        _exit(1);
    }
    else {
        if (waitpid(pid, nullptr, 0 ) < 0) {
            perror("smash error: waitpid failed");
        }
        close(my_pipe[1]);
        std::string cmd_2_s;
        for (int i = this->getPipePlace() + 1; i < this->getNumOfArgs(); i++) {
            cmd_2_s += this->getCmdArgs()[i] + " ";
        }
        if (dup2(my_pipe[0], STDIN_FILENO) < 0) {
            perror("smash error: dup2 failed");
        }

        std::unique_ptr<Command> cmd_2 = this->m_small_shell.CreateCommand(cmd_2_s.c_str());
        cmd_2->execute();

        this->m_small_shell.restoreStdIn();
    }
}