# Smash – Small Shell

Smash (small shell) is a lightweight command-line interpreter for Linux, implemented in C++ as part of an Operating Systems course.  
It mimics the behavior of a real Linux shell, supporting both built-in and external commands, job control, and basic signal handling.

The goal of this project is to gain hands-on experience with:
- Process creation and management (`fork`, `exec`, `waitpid`)
- Signal handling (`SIGINT`, `SIGTSTP`, `SIGKILL`, etc.)
- I/O redirection and pipes
- Implementing built-in shell commands

## Assumptions & Limitations

To simplify the implementation, the following assumptions are made:

- Each command appears on a separate line and cannot exceed **200 characters**.  
- Each command supports up to **20 arguments**.  
- Any number of spaces (defined by the `WHITESPACE` macro in `Commands.h`) may appear between words or at the beginning of the line.  
- The shell supports up to **100 processes** running simultaneously.  
- File and folder names do not include special characters and are assumed to be lowercase.  
- I/O redirection (`>`, `>>`) and pipes (`|`, `|&`) are assumed to be correctly formatted.  
- Memory allocation (`new` / `malloc`) will not fail, and memory leaks are not checked.

## Built-in Commands

Smash implements several built-in commands, which are executed directly inside the shell process (without forking).  

### 🔹 `chprompt <new-prompt>`
- Changes the shell prompt.  
- If no parameter is given, the prompt resets to `smash>`.  
- If multiple parameters are given, only the first is used.  

**Example:**
```bash
smash> chprompt custom
custom> chprompt
smash>
```

### 🔹 `showpid`
- showpid command prints the smash PID.
- If any number of arguments were provided with this command, they will be ignored.

**Example:**
```bash
smash> showpid
smash pid is 30903
smash> 
```

### 🔹 `pwd`
- pwd command has no arguments.
- The pwd command prints the full path of the current working directory.
- If any number of arguments were provided with this command, they will be ignored.

**Example:**
```bash
smash> pwd
/home/smash/build
smash>
```

### 🔹 `cd <new-path>`
- The cd (Change Directory) command takes a single argument <path> that specifies either a relative or full path to change the current working directory to.
- There's a special argument, `-`, that cd can accept.
- When `-` is the only argument provided to the cd command, it instructs the shell to change the current working directory to the last working directory.
- Can accept only one argument.
- If the last working directory is empty and `cd -` was called (before calling cd with some path to change current working directory to it) then it will be an error.

**Example:**
```bash
smash> cd -
smash error: cd: OLDPWD not set
smash> cd dir1 dir2
smash error: cd: too many arguments
smash> cd /home/superMario
smash> pwd
/home/superMario
smash> cd ..
smash> pwd
/home
smash> cd -
smash> pwd
/home/superMario
smash>
```

### 🔹 `jobs`
- The jobs command prints the jobs list which contains the unfinished jobs (Those running in the background).
- The list is printed in thhe following format: [<job-id>] <command>, where <command> is the original command provided by the user (including aliases).
- If any number of arguments were provided, they will be ignored.

**Example:**
```bash
smash> sleep 100&
smash> sleep 200&
smash> jobs
[1] sleep 100&
[2] sleep 200&
smash>
```

### 🔹 `fg`
- The fg command brings a process that runs in the background to the foreground.
- fg command prints the command line of that job along with its PID (as can be seen in the example) and then waits for it, which in effect will bring the requested process to run in the foreground.
- The job-id argument is an optional argument. If it is specified, then the specific job which its job id (as printed in jobs command) should be brought to the foreground.
- If the job-id argument is not specified, then the job with the maximal job id in the jobs list should be selected to be brought to the foreground.
- Syntax has to be valid (number of arguments or the format of the arguments).
- job-id has to exist.

**Example:**
```bash
smash> sleep 100&
smash> sleep 200&
smash> sleep 500&
smash> jobs
[1] sleep 100&
[2] sleep 200&
[3] sleep 500&
smash> fg 3
sleep 500& 901
```

### 🔹 `quit [kill]`
- The quit command exits the smash.
- Only if the kill argument was specified (which is optional)
then smash should kill all of its unfinished jobs and print the number of processes/jobs that were killed, their PIDs and command-lines.
- We assume that the kill argument, if present, will appear first.
- If any number of arguments (other than kill) were provided with this command, they will be ignored.

**Example:**
```bash
smash> quit kill
smash: sending SIGKILL signal to 3 jobs:
30959: sleep 100&
30960: sleep 200&
30961: sleep 10&
Linux-shell:
```

### 🔹 `kill -<signum> <jobid>`
- Kill command sends a signal whose number is specified by <signum> to a job whose sequence ID in jobs list is <job-id> (same as job-id in jobs command) and prints a message reporting that the specified signal was sent to the specified job.
- Syntax has to be valid (number of arguments or the format of the arguments).
- job-id has to exist.

**Example:**
```bash
smash> kill -9 1
signal number 9 was sent to pid 30985
smash>
```

### 🔹 `alias <name>=’<command>’`
- The alias command is used to create an alias, which is a shortcut that allows a string to be substituted for a command.
- This can include the command itself and its parameters, but it cannot replace parameters alone.
- <name> is a case-sensitive, user-defined string that can include only letters (a-z, A-Z), numbers (0-9), and underscores (_). It must not be an internal reserved keyword of the shell (eg., `quit`, `lisdir` etc…).
- If no arguments are provided, list all current aliases.
- If the alias name conflicts with an existing alias or a reserved keyword, then an error message would be printed.
- Syntax has to be valid (number of arguments, the format of the arguments or <name>).
- We assume no recursive aliases (to make it simple).

**Example:**
```bash
smash> alias ll='ls -l'
smash> ll
total 0
-rw-r--r-- 1 user user 0 Jan 1 00:00 file.txt
smash> alias s100=’sleep 100 ’
smash> alias
ll='ls -l'
s100=’sleep 100 ’
smash> alias s=’sleep’
smash> s 3
```
- After 3 seconds:
```bash
smash>
```

### 🔹 `unalias <name_1> <name_2> …`
- The unalias command removes the specified aliases (<name_1>, <name_2>, ...) separated by spaces.
- If one or more of the provided alias names do not exist, the deletion process stops at the first invalid occurrence.

**Example:**
```bash
smash> alias ll='ls -l'
smash> unalias ll
smash>
```

### 🔹 `unsetenv <variable_1> <variable_2> …`
- The unsetenv command removes the specified environment variables separated by spaces.
- After successful execution, the specified variables will no longer exist in the environment.

**Example:**
```bash
smash> unsetenv TMPDIR
smash>
```

### 🔹 `watchproc <pid>`
- The watchproc command displays a snapshot of CPU and memory usage for the specified process.
- The <pid> argument specifies the process ID of the process whose resource usage will be reported.
- Syntax has to be valid (number of arguments or the format of the arguments).
- PID has to exist.

**Example:**
```bash
smash> watchproc 5678
PID: 5678 | CPU Usage: 1.8% | Memory Usage: 23.7 MB
smash>
```

## External Commands

Besides built-in commands, **smash** supports executing external commands.  
An external command is any command that is not built-in or a special command.

### 🔹 Command format
```bash
<command> [arguments]
```
- command – name of the external command or executable.
- arguments – optional (up to 20).

### 🔹 Types of external command

#### 1. Simple external command
- Run directly with an **exec** system call.

**Example:**
```bash
ls -l
sleep 3
./a.out arg1 arg2
```
#### 2. Complex external commands (contain * or ?)
- Must be executed via **bash -c "<command>"**.

**Example:**
```bash
bash -c "rm *.txt"
```

## Background execution (built-in or external)
- Adding & at the end of the command runs the command in the background.
- Smash does not wait for background commands to finish.
- Background processes are added to the Jobs list.

## Special Commands

### IO redirection
- 

