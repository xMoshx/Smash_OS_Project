#ifndef KILLCOMMAND_H
#define KILLCOMMAND_H

#include "BuiltInCommand.h"

class KillCommand : public BuiltInCommand{
public:
    KillCommand(std::string cmd_line[], int num_of_args, SmallShell& other) :
                                                                BuiltInCommand(cmd_line, num_of_args, other) {};

    ~KillCommand() override = default;

    void execute() override;
};



#endif //KILLCOMMAND_H
