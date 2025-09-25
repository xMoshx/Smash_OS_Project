#ifndef FOREGROUNDCOMMAND_H
#define FOREGROUNDCOMMAND_H

#include "BuiltInCommand.h"

class ForegroundCommand : public BuiltInCommand{
public:
    ForegroundCommand(std::string cmd_line[], int num_of_args, SmallShell& other) : BuiltInCommand(cmd_line, num_of_args, other) {};

    ~ForegroundCommand() override = default;

    void execute() override;

    void executeFgByID();

    void executeLastId();
};



#endif //FOREGROUNDCOMMAND_H
