#include <iostream>
#include "AliasCommand.h"

std::string& AliasCommand::getAlias(){
    return this->m_alias;
}

std::string& AliasCommand::getCmd(){
    return m_cmd;
}

void AliasCommand::execute() {
    if(this->getCmdArgs()[1].empty()){ // printing all the alias's the user created
        for (const std::string& alias : this->m_small_shell.getOrder()) {
            std::cout << alias << "='" << this->m_small_shell.getMap()[alias] << "'" << std::endl;
        }
        return;
    }
    this->m_small_shell.addAlias(this->getCmd(), this->getAlias());
}
