#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H

#include "BuiltInCommand.h"

class QuitCommand : public BuiltInCommand{
public:
    QuitCommand(std::string cmd_args[], int num_of_args, SmallShell& other) :
                                                                    BuiltInCommand(cmd_args, num_of_args, other){};

    ~QuitCommand() override = default;

    void execute() override;
};

#endif //QUITCOMMAND_H
