#ifndef EXTERNALCOMMAND_H_
#define EXTERNALCOMMAND_H_

#include "Commands/Command.h"
#include "SmallShell.h"

class ExternalCommand : public Command {
    bool m_is_bg_cmd;
    SmallShell& m_shell;
    const char* m_og_cmd;
    
public:
    ExternalCommand(std::string cmd_args[], const char* og_cmd, int num_of_args, SmallShell& shell, bool is_bg_cmd) : Command(cmd_args, num_of_args), m_shell(shell) {
        m_is_bg_cmd = is_bg_cmd;
        m_og_cmd = og_cmd;
    };

    virtual ~ExternalCommand() {
    }

    bool getIsBgCmd() const {
        return m_is_bg_cmd;
    }

    void execute() override;
};

#endif /* EXTERNALCOMMAND_H_ */