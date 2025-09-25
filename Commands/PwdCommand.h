#ifndef PWDCOMMAND_H
#define PWDCOMMAND_H

#include "BuiltInCommand.h"

class PwdCommand : public BuiltInCommand{
public:
    PwdCommand(std::string cmd_args[], int num_of_args, SmallShell& other) :
                                                                    BuiltInCommand(cmd_args, num_of_args, other){};

    void execute() override;
};



#endif //PWD_H
