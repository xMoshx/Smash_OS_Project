#include <fcntl.h>
#include <unistd.h>

#include "RedirectionCommand.h"

void RedirectionCommand::execute() {

    // flag O_APPEND for ">>", O_TRUNC for ">"
    int flags = O_CREAT | O_WRONLY | (this->getMode() == ">>" ? O_APPEND : O_TRUNC);
    int new_fd = open(this->getPath().c_str(), flags, 0666);

    if (new_fd < 0) {
        std::perror("smash error: open failed");
    }

    if (dup2(new_fd, STDOUT_FILENO) < 0) {

        // should not reach here
        std::perror("smash error: dup2 failed");
        close(new_fd);
    }
}
