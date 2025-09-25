#ifndef SMALLSHELL_H
#define SMALLSHELL_H

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "Commands/Command.h"
#include "JobsList.h"


class SmallShell {
    std::string m_prompt_name = "smash";
    std::string m_last_working_dir = "\n"; // Initialized value

    // Alias related fields
    std::map<std::string, std::string> m_name_to_cmd;
    std::vector<std::string> m_order;
    //----------//
    JobsList m_jobs_list;

    int m_saved_std_out;
    int m_saved_std_in;
    int m_saved_std_err;
    int m_redirected_out;
    int m_redirected_in;
    int m_redirected_err;

    // Initial value of the foreground process pid
    pid_t m_fg_proc_pid = 0;

    SmallShell();

public:
    SmallShell(SmallShell const &) = delete; // disable copy ctor

    void operator=(SmallShell const &) = delete; // disable = operator

    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    ~SmallShell() = default;

    void executeCommand(const char *cmd_line);

    std::string getPromptName() const;

    void setPromptName(const std::string& new_prompt_name);

    std::string getlastWorkingDir() const;

    void setlastWorkingDir(const std::string& dir);

    void addAlias(std::string& cmd, std::string& alias);

    std::map<std::string, std::string>& getMap();

    std::vector<std::string>& getOrder();

    JobsList& getSmashJobsList();

    int getSavedStdOut() const;

    int getSavedStdIn() const;

    int getSavedStdErr() const;

    void setRedirectionOut(int new_fd);

    void setRedirectionIn(int new_fd);

    void setRedirectionErr(int new_fd);

    int getRedirectedOut() const;

    int getRedirectedIn() const;

    int getRedirectedErr() const;

    void restoreStdOut();

    void restoreStdIn();

    void restoreStdErr();

    pid_t getFgProcPID() const;

    void setFgProcPID(pid_t pid);

    std::unique_ptr<Command>CreateCommand(const char *cmd_line);

    std::unique_ptr<Command> createCommandHandler(std::string args_cmd[], const char* og_cmd, int numOfArgs,
                                                                                    bool is_bg_cmd, int is_pipe_cmd);

    std::unique_ptr<Command> buildAliasCommand(std::string args_cmd[], int numOfArgs);

    std::vector<std::string> CreateCommandVectorAlias(std::string args_cmd[], size_t numOfArgs);

    std::unique_ptr<Command> CreateCommandAndRedirect(std::string args_cmd[], size_t numOfArgs,
                                                                                    const char* cmd_line, int start);
};

#endif //SMALLSHELL_H
