#include <unistd.h>
#include <iostream>
#include <string>

#include "WhoAmICommand.h"
#include "../Utilities.h"

void WhoAmICommand::execute() {
    uid_t user_id = getuid();
    std::string content;
    
    try {
        readFileContent("/etc/passwd", content);
    } catch (const std::exception& ex) {
        std::perror("readFileContent failed");
        return;
    }

    std::string user, path;
    for (auto& line : splitLines(content)) {
        size_t field_start = 0, field_end;

        // username
        field_end = line.find(':', field_start);
        if (field_end == std::string::npos) continue;
        std::string name = line.substr(field_start, field_end - field_start);

        // skip password
        field_start = field_end + 1;
        field_end = line.find(':', field_start);
        if (field_end == std::string::npos) continue;

        // uid field
        field_start = field_end + 1;
        field_end = line.find(':', field_start);
        if (field_end == std::string::npos) continue;
        std::string uid_str = line.substr(field_start, field_end - field_start);

        // match against current UID
        if (static_cast<uid_t>(std::stoi(uid_str)) != user_id)
            continue;

        // skip gid, full name fields
        for (int i = 0; i < 3; ++i) {
            field_start = field_end + 1;
            field_end = line.find(':', field_start);
            if (field_end == std::string::npos) break;
        }

        // home directory
        if (field_start < line.size()) {
            field_end = line.find(':', field_start);
            if (field_end == std::string::npos)
                path = line.substr(field_start);
            else
                path = line.substr(field_start, field_end - field_start);
        }

        user = name;
        break;
    }

    if (!user.empty()) {
        std::cout << user << " " << path << "\n";
    }
}