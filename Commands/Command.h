#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#define COMMAND_MAX_ARGS (20)
#define COMMAND_MAX_LENGTH (200)

class Command {
    std::string m_cmd_args[COMMAND_MAX_ARGS];
    int m_num_of_args;
public:
    Command(std::string cmd_args[COMMAND_MAX_ARGS], int num_of_args);

    virtual ~Command() = default;

    virtual void execute() = 0;

    std::string* getCmdArgs();

    int getNumOfArgs() const;

};
#endif //COMMAND_H
