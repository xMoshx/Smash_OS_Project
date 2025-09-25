#ifndef WHOAMI_COMMAND_H_
#define WHOAMI_COMMAND_H_

#include "../Commands/Command.h"
#include "../SmallShell.h"

class WhoAmICommand : public Command {
    SmallShell& m_small_shell;
public:
    WhoAmICommand(std::string cmd_args[], int numOfArgs, SmallShell& other): 
                                                        Command(cmd_args, numOfArgs), m_small_shell(other){};

    virtual ~WhoAmICommand() {}

    void execute() override;
};

#endif /* WHOAMI_COMMAND_H_ */