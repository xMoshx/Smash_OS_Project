#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <cmath>

#include "DiskUsageCommand.h"

uintmax_t DiskUsageCommand::diskUsageWrapper(const std::experimental::filesystem::path& path){
    struct stat sb;

    // Checks for unreadable folder
    if (lstat(path.c_str(), &sb) != 0) {
        return 0;
    }

    // Checks if the path is a symlink - if so, ignore
    if (S_ISLNK(sb.st_mode)) {
        return 0;
    }

    uintmax_t disk_usage = sb.st_blocks;

    if (S_ISDIR(sb.st_mode)) {

        // Only for this part we use recursive directory iterator
        // as we want to go through all sub-directories
        // and add their sizes as well
        for (auto const& entry : std::experimental::filesystem::directory_iterator(path)) {
            disk_usage += diskUsageWrapper(entry.path());
        }
    }
    return disk_usage;
}

void DiskUsageCommand::execute() {
    std::string path;
    struct stat sb;

    if(this->getNumOfArgs() == 1){
        path = getcwd(nullptr, 0);
    }
    else {
        path = this->getCmdArgs()[1];
    }
    if (stat(path.c_str(), &sb) != 0){
        std::cerr << "smash error: du: directory " << path << " does not exist" << std::endl;
        return;
    }
    if(this->getNumOfArgs() > 2){
        std::cerr << "smash error: du: too many arguments" << std::endl;
        return;
    }
    std::cout << "Total disk usage: " << std::ceil(diskUsageWrapper(path) / 2) <<" KB" << std::endl;
}