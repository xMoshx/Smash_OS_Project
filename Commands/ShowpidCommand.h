#ifndef SHOWPIDCOMMAND_H
#define SHOWPIDCOMMAND_H

#include "BuiltInCommand.h"

class ShowpidCommand : public BuiltInCommand{
public:
    ShowpidCommand(std::string cmd_args[], int num_of_args, SmallShell& other) :
                                                                    BuiltInCommand(cmd_args, num_of_args, other){};
    void execute() override;
};



#endif //SHOWPID_H
