#ifndef DISKUSAGECOMMAND_H_
#define DISKUSAGECOMMAND_H_

#include <experimental/filesystem>

#include "../Commands/Command.h"
#include "../SmallShell.h"

class DiskUsageCommand : public Command {
    SmallShell& m_small_shell;
public:
    DiskUsageCommand(std::string cmd_args[], int numOfArgs, SmallShell& other): 
                                                Command(cmd_args, numOfArgs), m_small_shell(other) {}

    virtual ~DiskUsageCommand() {}

    void execute() override;

    uintmax_t diskUsageWrapper(const std::experimental::filesystem::path& path);
};

#endif /* DISKUSAGECOMMAND_H_ */