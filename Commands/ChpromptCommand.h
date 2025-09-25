#ifndef CHPROMPTCOMMAND_H
#define CHPROMPTCOMMAND_H

#include "BuiltInCommand.h"


class ChpromptCommand : public BuiltInCommand{
    std::string m_name = "chprompt";
public:
    ChpromptCommand(std::string cmd_args[], int num_of_args, SmallShell& other) :
                                                                    BuiltInCommand(cmd_args, num_of_args, other){};
    void execute() override;
};



#endif //CHPROMPT_H
