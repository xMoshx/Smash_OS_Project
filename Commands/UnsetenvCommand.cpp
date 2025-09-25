#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "UnsetenvCommand.h"
#include "../Utilities.h"

void UnSetEnvCommand::execute() {
    std::vector<char> environ_vector = environ_file_to_vector();


    std::vector<std::string> env_names = extract_env_var_names(environ_vector);
    // Now we have an array of the names of the vars

    if (this->getNumOfArgs() > 1) {
        for (int i = 1; i < this->getNumOfArgs(); i++) {
            bool exists = std::find(env_names.begin(), env_names.end(), this->getCmdArgs()[i]) != env_names.end();
            if (exists) {
                remove_env_var(this->getCmdArgs()[i]);
            }
            else {
                std::cerr << "smash error: unsetenv: " << this->getCmdArgs()[i] << " does not exist" << std::endl;
                break;
            }
        }
    }
    else {
        std::cerr << "smash error: unsetenv: not enough arguments" << std::endl;
    }
}