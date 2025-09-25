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

### 🔹 `chprompt [new-prompt]`
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


