#include <iostream>
#include <unistd.h>
#include <iomanip>

#include "../Utilities.h"
#include "WatchprocCommand.h"


double WatchProcCommand::calculateCpuUsege(int pid){
    long hz = sysconf(_SC_CLK_TCK);
    uint64_t total_cpu_time_0 = get_total_cpu_time();
    uint64_t process_cpu_time_0;

    try {
        process_cpu_time_0 = get_process_cpu_time(pid);
    }
    catch (...) {
        std::cerr << "smash error: watchproc: pid " << pid << " does not exist" << std::endl;
        return -1;
    }

    struct timespec duration{};
    duration.tv_sec = 1;
    duration.tv_nsec = 0;
    if(nanosleep(&duration, nullptr) == -1){
        std::perror("smash error: nanosleep failed");
    }

    uint64_t total_cpu_time_1 = get_total_cpu_time();
    uint64_t process_cpu_time_1 = get_process_cpu_time(pid);

    uint64_t delta_total = total_cpu_time_1 - total_cpu_time_0;
    uint64_t delta_process = process_cpu_time_1 - process_cpu_time_0;

    return ((double(delta_process) / hz) / (double (delta_total) / hz)) * 100.0;
}

void WatchProcCommand::execute() {
    if (this->getNumOfArgs() == 2) {
        int pid;
        try {
            pid = atoi(this->getCmdArgs()[1].c_str());
        }
        catch(...){
            std::cerr << "smash error: watchproc: invalid arguments" << std::endl;
        }

        double cpu_usage = calculateCpuUsege(pid);

        if(cpu_usage == -1){
            return;
        }

        double mem_mb = get_mem_mb(pid);
        std::cout << "PID: " << pid
                << " | CPU Usage: " << std::fixed << std::setprecision(1)
                << cpu_usage << "% | Memory Usage: "
                << std::setprecision(1) << mem_mb <<" MB"
                << std::endl;
    }
    else {
        std::cerr << "smash error: watchproc: invalid arguments" << std::endl;
    }
}