#ifndef CDCOMMAND_H
#define CDCOMMAND_H

#include "BuiltInCommand.h"

class CdCommand : public BuiltInCommand{
public:
    CdCommand(std::string cmd_args[], int num_of_args, SmallShell& other) :
                                                                    BuiltInCommand(cmd_args, num_of_args, other){};

    virtual ~CdCommand() = default;
    
    void execute() override;
};

#endif //CDCOMMAND_H
