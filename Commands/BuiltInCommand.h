#ifndef BUILTINCOMMAND_H
#define BUILTINCOMMAND_H

#include "Command.h"
#include "../SmallShell.h"

class BuiltInCommand : public Command {
protected:
    SmallShell& m_small_shell;
public:
    BuiltInCommand(std::string cmd_args[], int num_of_args, SmallShell& other) :
                                                Command(cmd_args, num_of_args), m_small_shell(other){};

    virtual void execute() = 0;

    virtual ~BuiltInCommand() = default;
};

#endif //BUILTINCOMMAND_H
