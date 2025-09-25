#ifndef PIPECOMMAND_H_
#define PIPECOMMAND_H_

#include "../Commands/Command.h"
#include "../SmallShell.h"

class PipeCommand : public Command {
    SmallShell& m_small_shell;
    int m_pipe_place;
    bool m_is_err;

    void executePipeHandler(int fd);

public:
    PipeCommand(std::string cmd_args[], int numOfArgs, SmallShell& other, int pipe_place, bool is_err) :
            Command(cmd_args, numOfArgs), m_small_shell(other), m_pipe_place(pipe_place), m_is_err(is_err) {};

    virtual ~PipeCommand() {}

    bool getIsErr() const{
        return m_is_err;
    }
    
    int getPipePlace() {
        return this->m_pipe_place;
    }

    void execute() override;
};

#endif /* PIPECOMMAND_H_ */