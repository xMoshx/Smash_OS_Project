#ifndef JOBSCOMMAND_H
#define JOBSCOMMAND_H

#include "BuiltInCommand.h"

class JobsCommand : public BuiltInCommand{
public:
    JobsCommand(std::string cmd_line[], int num_of_args, SmallShell& other) :
                                                                    BuiltInCommand(cmd_line, num_of_args, other) {};

    ~JobsCommand() override = default;

    void execute() override;
};



#endif //JOBSCOMMAND_H
