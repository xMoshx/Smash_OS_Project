#include <iostream>
#include <algorithm>
#include "UnAliasCommand.h"

void UnAliasCommand::execute() {
    int num_of_args = this->getNumOfArgs();
    if (num_of_args < 2) {
        std::cerr << "smash error: unalias: not enough arguments" << std::endl;
    }
    else {
        removeAllAlias(num_of_args);
    }
}

void UnAliasCommand::removeAllAlias(int num_of_args) {
    std::map<std::string,std::string>& alias_map = this->m_small_shell.getMap();
    std::vector<std::string>& ordered_vector = this->m_small_shell.getOrder();

    for(int i = 1; i < num_of_args; ++i){
        bool exists = alias_map.find(this->getCmdArgs()[i]) != alias_map.end();
        std::string alias_to_remove = this->getCmdArgs()[i];
        if(exists){
            alias_map.erase(alias_to_remove);
            auto alias_to_remove_ord = std::find(ordered_vector.begin(),
                                                 ordered_vector.end(), this->getCmdArgs()[i]);
            ordered_vector.erase(alias_to_remove_ord);
        }
        else{
            std::cerr << "smash error: " + alias_to_remove + " alias does not exist" << std::endl;
            break;
        }
    }
}