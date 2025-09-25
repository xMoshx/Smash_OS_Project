#ifndef __UNSETENV_H__
#define __UNSETENV_H__

#include "BuiltInCommand.h"

class UnSetEnvCommand : public BuiltInCommand {
public:
    UnSetEnvCommand(std::string cmd_args[], int num_of_args, SmallShell& other)
                                        : BuiltInCommand(cmd_args, num_of_args, other){};

    virtual ~UnSetEnvCommand() {
    }

    void execute() override;
};

#endif //__UNSETENV_H__
