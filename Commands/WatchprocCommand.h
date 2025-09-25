#ifndef WATCHPROC_H__
#define WATCHPROC_H__

#include "BuiltInCommand.h"

class WatchProcCommand : public BuiltInCommand {
public:
    WatchProcCommand(std::string cmd_args[], int num_of_args, SmallShell& other)
            : BuiltInCommand(cmd_args, num_of_args, other){};

    virtual ~WatchProcCommand() {
    }

    void execute() override;

    double calculateCpuUsege(int pid);
};

#endif // WATCHPROC_H__