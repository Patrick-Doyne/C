# Zaku – Task Manager Shell in C

## Program Overview & Purpose

Implements an interactive command-line shell (“Zaku”) that manages foreground and background jobs, UNIX signals, piping, I/O redirection, and built-in commands. Designed for GMU CS 367 to demonstrate process control, signal handling, and linked-list task management in C.

---

## Language, Compiler & System Interfaces

- Language: C (C99 standard)

- Compiler: GCC with flags `-std=c99 -Wall -Wextra`

- Build Automation: `make` (Makefile orchestrates compilation of `zaku.c`, `parse.c`, `util.c`)

- Process Control: `fork()`, `execv()`, `wait()`, `waitpid()`, `setpgid()`

- Signals: `sigaction()`, `SIGINT`, `SIGTSTP`, `SIGCHLD`, `SIGCONT`

- I/O Redirection: `open()`, `dup2()`, `< infile`, `> outfile`

- Piping: `pipe()`, file descriptors

- Data Structures: Linked list of Task nodes (`pid_t`, state flags, command text)

---

## Development Environment

Unix/Linux (GMU Horizon Hall lab machines or any POSIX shell)

Shell: Bash for interactive use and script testing

Editors: Vim, Emacs, VS Code, or any C-aware IDE

---

## Provided Headers & Modules

- `zaku.h`  
  Defines the `Task` struct and function prototypes for shell operations

- `parse.h`  
  Tokenizes user input into commands, arguments, and I/O files

- `util.h`  
  String-copy helpers and memory utilities for command data

- `logging.h`  
  Logging primitives (intro, prompt, status changes, errors)

---

## Constraints

- No external shell libraries—parsing via provided `parse.h` only

- No dynamic shell-framework calls; implement built-ins directly

- Maintain your own linked list of `Task` entries

- All output through `logging.h` calls; no `printf` in command logic

---

## File Structure

- `zaku.c`  
  Full implementation of the shell loop, built-ins, task list, and signal handler

- Supporting source files:  
  - `parse.c`, `util.c` (provided)  
  - Headers in `include/`

---

## Features Implemented

- Built-in instructions:  
  `quit`, `help`, `list`, `delete`, `start_fg`, `start_bg`, `kill`, `suspend`,  
  `resume_fg`, `resume_bg`, `fg`, `pipe`, `chain`

- Task lifecycle management: ready → running → suspended → finished/terminated

- Foreground & background job control with custom signal handler

- I/O redirection for `< infile` and `> outfile` in `start_fg`/`start_bg`

- Unidirectional piping between two user tasks

- Structured logging of all events (status changes, errors, I/O redirections)

---

## Key Concepts

- UNIX job control and process groups (`setpgid`, foreground vs. background)

- Robust signal handling: ignore/reinstall handlers to avoid race conditions

- Linked-list management: insertion, deletion, lookup by PID or task number

- Shell parsing integration via `parse.h` API

- Logging abstraction for consistent output without direct `printf`

---

## Functions & Their Roles

| Function                                 | Role & Description                                                                 |
|------------------------------------------|------------------------------------------------------------------------------------|
| `getTaskFromPid(taskList, PID)`          | Return `Task*` matching `PID`, or NULL                                              |
| `getRunningFG(taskList)`                 | Return currently running foreground `Task*`, or NULL                                |
| `handler(sig)`                           | Custom `SIGINT`/`SIGTSTP`/`SIGCHLD` handler updating task states and logging        |
| `containsInstruction(cmd, instructions)` | Test if `cmd` matches any built-in instruction                                      |
| `biggestTaskNum(taskList)`               | Return highest assigned task number                                                |
| `getTask(taskList, taskNum)`             | Return `Task*` with `taskNumber == taskNum`, or NULL                                |
| `deleteTask(taskList, taskNum)`          | Remove a non-running/non-suspended task by number and log deletion or error         |
| `createBasicTask(taskList, cmd, argv)`   | Allocate and insert new `Task` node in “ready” state, copying `cmd` and `argv`     |
| `listTasks(taskList)`                    | Traverse and log info for every task in the list                                   |
| `start(taskList, taskNum, fg_bg, in, out)` | Fork/exec a task in foreground or background, set up I/O redirection, log events   |
| `chain(taskList, n1, n2)`                | Execute two tasks sequentially: wait on first (fg), then start second (bg)         |
| `killTask(taskList, taskNum)`            | Send `SIGINT` to a background task                                                  |
| `fg(taskList, taskNum)`                  | Bring a background task to foreground, wait for exit, and log termination           |
| `suspend(taskList, taskNum)`             | Send `SIGTSTP` to a background task and log suspension                              |
| `resume(taskList, taskNum, fg_bg)`       | Send `SIGCONT` to a suspended task; optionally wait if moved to foreground         |
| `pipeTasks(taskList, n1, n2)`            | Set up a UNIX pipe between two tasks, fork both, redirect fds, and log execution    |

---

## Usage Notes

After parsing, built-in commands dispatch directly; unknown commands spawn new tasks.

Use `help` to list available built-ins and syntax.

Always call `free_command()` at loop end to avoid leaks.

Test piping, chaining, and signal interactions in Horizon Hall lab environment.

---

## Debugging & Testing

Use GDB to step through `handler()` and job-control logic.

Valgrind (optional) to detect leaks in `createBasicTask` and list operations.

Write simple shell scripts or Makefile targets to automate tests of each built-in.

---

## Author & Credits

Student Implementation: Patrick

Starter Code & Course: Prof. Yutao Zhong, GMU CS 367 (Fall 2024)

Course Text: Computer Systems: A Programmer’s Perspective by Bryant & O’Hallaron

All rights reserved by Prof. Zhong; student additions licensed for academic use only  
