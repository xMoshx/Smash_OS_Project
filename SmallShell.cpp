#include <iostream>
#include <unistd.h>
#include <regex>
#include <cstring>
#include <memory>
#include "SmallShell.h"
#include "ExternalCommand.h"
#include "Utilities.h"
#include "SpecialCommands/PipeCommand.h"
#include "SpecialCommands/RedirectionCommand.h"
#include "SpecialCommands/WhoAmICommand.h"
#include "SpecialCommands/DiskUsageCommand.h"
#include "Commands/WatchprocCommand.h"
#include "Commands/ChpromptCommand.h"
#include "Commands/ShowpidCommand.h"
#include "Commands/CdCommand.h"
#include "Commands/PwdCommand.h"
#include "Commands/UnAliasCommand.h"
#include "Commands/UnsetenvCommand.h"
#include "Commands/JobsCommand.h"
#include "Commands/ForegroundCommand.h"
#include "Commands/QuitCommand.h"
#include "Commands/KillCommand.h"
#include "Commands/AliasCommand.h"

const std::vector<std::string> RESERVED_COMMANDS = {"chprompt", "showpid", "pwd", "cd", "jobs", "fg",
                                        "quit", "kill", "alias", "unalias", "unsetenv", "watchproc"};

SmallShell::SmallShell() {
    this->m_saved_std_out = dup(STDOUT_FILENO);
    if (this->m_saved_std_out < 0) {
        std::perror("smash error: dup failed");
    }

    this->m_saved_std_in = dup(STDIN_FILENO);
    if (this->m_saved_std_in < 0) {
        std::perror("smash error: dup failed");
    }

    this->m_saved_std_err = dup(STDERR_FILENO);
    if (this->m_saved_std_err < 0) {
        std::perror("smash error: dup failed");
    }
}

std::string SmallShell::getPromptName() const {
    return this->m_prompt_name;
}

void SmallShell::setPromptName(const std::string& new_prompt_name) {
    this->m_prompt_name = new_prompt_name;
}

std::string SmallShell::getlastWorkingDir() const{
    return this->m_last_working_dir;
}

void SmallShell::setlastWorkingDir(const std::string& dir) {
    this->m_last_working_dir = dir;
}

void SmallShell::addAlias(std::string& cmd, std::string& alias) {
    this->m_name_to_cmd.insert({alias, cmd});
    m_order.push_back(alias);
}

std::map<std::string, std::string>& SmallShell::getMap() {
    return this->m_name_to_cmd;
}

std::vector<std::string>& SmallShell::getOrder() {
    return this->m_order;
}

JobsList& SmallShell::getSmashJobsList() {
    return this->m_jobs_list;
}

int SmallShell::getSavedStdOut() const {
    return this->m_saved_std_out;
}

int SmallShell::getSavedStdIn() const {
    return this->m_saved_std_in;
}

int SmallShell::getSavedStdErr() const {
    return this->m_saved_std_err;
}

void SmallShell::setRedirectionOut(int new_fd) {
    this->m_redirected_out = new_fd;
}

void SmallShell::setRedirectionIn(int new_fd) {
    this->m_redirected_in = new_fd;
}

void SmallShell::setRedirectionErr(int new_fd) {
    this->m_redirected_err = new_fd;
}

int SmallShell::getRedirectedOut() const{
    return this->m_redirected_out;
}

int SmallShell::getRedirectedIn() const {
    return this->m_redirected_in;
}

int SmallShell::getRedirectedErr() const{
    return this->m_redirected_err;
}

void SmallShell::restoreStdOut() {
    fflush(stdout);
    if (dup2(this->getSavedStdOut(), STDOUT_FILENO) < 0) {
        std::perror("smash error: dup failed");
    }
}

void SmallShell::restoreStdIn() {
    fflush(stdin);
    if (dup2(this->getSavedStdIn(), STDIN_FILENO) < 0) {
        std::perror("smash error: dup failed");
    }
}

void SmallShell::restoreStdErr() {
    fflush((stderr));
    if (dup2(this->getSavedStdErr(), STDERR_FILENO) < 0) {
        std::perror("smash error: dup failed");
    }
}

pid_t SmallShell::getFgProcPID() const {
    return this->m_fg_proc_pid;
}

void SmallShell::setFgProcPID(pid_t pid) {
    this->m_fg_proc_pid = pid;
}

std::unique_ptr<Command> SmallShell::CreateCommand(const char *cmd_line) {
    std::size_t len = std::strlen(cmd_line); //get len of the cmd line
    char* copy_cmd_line = new char[len + 1]; //copy the cmd line for usege
    std::strcpy(copy_cmd_line, cmd_line);

    char* char_args_cmd[COMMAND_MAX_ARGS];
    std::string args_cmd[COMMAND_MAX_ARGS]; // Full command for non-alias command

    _removeBackgroundSign(copy_cmd_line); // Working without the & incase it's internal command
    std::size_t numOfArgs = _parseCommandLine(copy_cmd_line, char_args_cmd);

    _charPtrArrayToStringArray(char_args_cmd, args_cmd, numOfArgs); // Working with strings - easier

    auto mapped_cmd = this->m_name_to_cmd.find(args_cmd[0]);

    if(mapped_cmd != this->m_name_to_cmd.end()){ // Found it's an aliased command
        std::vector<std::string> alias_args_cmd_v = CreateCommandVectorAlias(args_cmd, numOfArgs);
        std::string alias_args_cmd[COMMAND_MAX_ARGS];
        std::string* tmp = _vectorToStringArray(alias_args_cmd_v);
        for (size_t i = 0; i < COMMAND_MAX_ARGS && i < alias_args_cmd_v.size(); ++i){
            alias_args_cmd[i] = tmp[i];
        }
        delete[] tmp;
        int start = _parseCommandLine(cmd_line, char_args_cmd);
        return CreateCommandAndRedirect(alias_args_cmd, numOfArgs, cmd_line, start - 1);
    }
    else{
        return CreateCommandAndRedirect(args_cmd, numOfArgs, cmd_line, 0); // Non-alias command
    }
}

std::unique_ptr<Command> SmallShell::createCommandHandler(std::string args_cmd[], const char* og_cmd, int numOfArgs,
                                                                                    bool is_bg_cmd, int is_pipe_cmd) {
    if (is_pipe_cmd){
        bool is_err = is_pipe_cmd != 1;
        int pipe_place = _findPipePlacement(args_cmd, numOfArgs);
        return std::make_unique<PipeCommand>(args_cmd, numOfArgs, *this, pipe_place, is_err);
    }
    else if(args_cmd[0] == "chprompt"){
        return std::make_unique<ChpromptCommand>(args_cmd, numOfArgs, *this);
    }
    else if(args_cmd[0] == "showpid"){
        return std::make_unique<ShowpidCommand>(args_cmd, numOfArgs, *this);
    }
    else if(args_cmd[0] == "cd"){
        return std::make_unique<CdCommand>(args_cmd, numOfArgs, *this);
    }
    else if(args_cmd[0] == "pwd"){
        return std::make_unique<PwdCommand>(args_cmd, numOfArgs, *this);
    }
    else if(args_cmd[0] == "alias"){
        return buildAliasCommand(args_cmd, numOfArgs);
    }
    else if(args_cmd[0] == "unalias"){
        return std::make_unique<UnAliasCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "unsetenv") {
        return std::make_unique<UnSetEnvCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "watchproc") {
        return std::make_unique<WatchProcCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "jobs") {
        return std::make_unique<JobsCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "fg") {
        return std::make_unique<ForegroundCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "quit") {
        return std::make_unique<QuitCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "kill") {
        return std::make_unique<KillCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "du") {
        return std::make_unique<DiskUsageCommand>(args_cmd, numOfArgs, *this);
    }
    else if (args_cmd[0] == "whoami") {
        return std::make_unique<WhoAmICommand>(args_cmd, numOfArgs, *this);
    }
    else {
        return std::make_unique<ExternalCommand>(args_cmd, og_cmd, numOfArgs, *this, is_bg_cmd);
    }
    return nullptr; // Just in case
}

std::unique_ptr<Command> SmallShell::CreateCommandAndRedirect(std::string args_cmd[], std::size_t numOfArgs,
                                                                                    const char* cmd_line, int start) {
    int is_pipe_cmd = false;
    bool is_redirect = false;
    bool is_bg_cmd = _isBackgroundComamnd(cmd_line);
    auto is_special(_findAndSplitSpecial(args_cmd, numOfArgs));

    if(is_special){
        if(is_special->first == "|"){
            is_pipe_cmd = 1;
        }
        else if(is_special->first == "|&"){
            is_pipe_cmd = 2;
        }
        else if(is_special->first == ">" || is_special->first == ">>"){
            is_redirect = true;
        }
    }

    if (is_redirect) {
        auto redirect_cmd = std::make_unique<RedirectionCommand>(args_cmd, numOfArgs, args_cmd[numOfArgs - 2],
                                                                                    args_cmd[numOfArgs - 1], *this);
        redirect_cmd->execute();
        numOfArgs -= 2;
        return createCommandHandler(args_cmd, cmd_line, numOfArgs + start, is_bg_cmd, is_pipe_cmd);
    }

    return createCommandHandler(args_cmd, cmd_line, numOfArgs + start, is_bg_cmd, is_pipe_cmd);
}

std::unique_ptr<Command> SmallShell::buildAliasCommand(std::string args_cmd[], int numOfArgs) {
    std::string full_alias_cmd;
    for (int i = 0; i < numOfArgs - 1; i++) {
        full_alias_cmd += args_cmd[i] + " ";
    }
    full_alias_cmd += args_cmd[numOfArgs - 1];

    std::regex pat(R"(^alias ([A-Za-z0-9_]+)='([^']*)'$)");
    std::smatch result;

    if(numOfArgs == 1){
        return std::make_unique<AliasCommand>("", "", args_cmd, numOfArgs, *this);
    }

    if(std::regex_match(full_alias_cmd, result, pat)){
        std::string alias = result[1];
        std::string cmd = result[2];
        bool exists = std::find(RESERVED_COMMANDS.begin(), RESERVED_COMMANDS.end(), alias) !=
                                                                                                RESERVED_COMMANDS.end();

        if (this->m_name_to_cmd.find(alias) == this->m_name_to_cmd.end() && !exists) {
            return std::make_unique<AliasCommand>(cmd, alias, args_cmd, numOfArgs, *this);
        }
        else {
            std::cerr << "smash error: " << alias << " already exists or is a reserved word" << std::endl;
        }
    }
    else{
        std::cerr<< "smash error: alias: invalid alias format" << std::endl;
    }
    return nullptr;
}

std::vector<std::string> SmallShell::CreateCommandVectorAlias(std::string args_cmd[], size_t numOfArgs) {
    char* char_alias_args_cmd[COMMAND_MAX_ARGS];
    int start = _parseCommandLine(this->m_name_to_cmd[std::string(args_cmd[0])].c_str(), char_alias_args_cmd);

    std::vector<std::string> alias_args_cmd = _charArrayToVector(char_alias_args_cmd, numOfArgs);
    alias_args_cmd.resize(COMMAND_MAX_ARGS);

    for(std::size_t i = 0; i < numOfArgs; ++i){ // Copy the arguments of the rest of the command to a string format
        alias_args_cmd[i + start] = args_cmd[i + 1];
    }
    return alias_args_cmd;
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here
    if(*cmd_line == '\0'){
        return;
    }
    std::unique_ptr<Command> cmd = CreateCommand((cmd_line));
    if (cmd != nullptr) {
        this->getSmashJobsList().removeFinishedJobs();
        cmd->execute();
    }

    this->restoreStdOut();
}

