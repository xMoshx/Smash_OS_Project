#ifndef ALIASCOMMAND_H
#define ALIASCOMMAND_H

#include "BuiltInCommand.h"

class AliasCommand : public BuiltInCommand{
    std::string m_alias;
    std::string m_cmd;
public:
    AliasCommand(const std::string& cmd, const std::string& alias, std::string cmd_args[], int num_of_args, SmallShell& other)
                                                            : BuiltInCommand(cmd_args, num_of_args, other){
        m_alias = alias;
        m_cmd= cmd;
    };

    ~AliasCommand() override = default;

    std::string& getAlias();

    std::string& getCmd();

    void execute() override;
};



#endif //ALIASCOMMAND_H
