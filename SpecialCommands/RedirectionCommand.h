#ifndef REDIRECTIONCOMMAND_H_
#define REDIRECTIONCOMMAND_H_

#include "../Commands/Command.h"
#include "../SmallShell.h"

class RedirectionCommand : public Command {
    SmallShell& m_small_shell;
    std::string m_path;
    std::string m_mode;

public:
    explicit RedirectionCommand(std::string cmd_args[], size_t numOfArgs, std::string& mode, std::string& path,  SmallShell& other) : Command(cmd_args, numOfArgs), m_small_shell(other) {
        m_path = path;
        m_mode = mode;
    };

    virtual ~RedirectionCommand() {
    }

    std::string getPath() const{
        return this->m_path;
    }

    std::string getMode() const {
        return this->m_mode;
    }

    void execute() override;
};

#endif /* REDIRECTIONCOMMAND_H_ */