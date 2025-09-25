#ifndef UNALIASCOMMAND_H
#define UNALIASCOMMAND_H

#include "BuiltInCommand.h"

class UnAliasCommand : public BuiltInCommand {
public:
    UnAliasCommand(std::string cmd_args[], int num_of_args, SmallShell& other)
                                    : BuiltInCommand(cmd_args, num_of_args, other){};

    ~UnAliasCommand() override = default;

    void removeAllAlias(int num_of_args);

    void execute() override;
};




#endif //UNALIASCOMMAND_H
