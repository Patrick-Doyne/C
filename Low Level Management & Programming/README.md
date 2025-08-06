# Low Level Management & Programming

This directory contains a collection of C projects and modules demonstrating a wide range of low-level systems programming concepts, including process scheduling, task management, and IPv4 address manipulation. Each subdirectory contains specialized functionality with source code, documentation, and build scripts.

---

## Directory Structure

### Scheduler

- **Purpose:** A simplified CPU scheduling simulator for educational use (CS367: Operating Systems, GMU).
- **Features:**
  - Bitmask-based process state management (ready, running, suspended, terminated)
  - Linked queue management for process states
  - Priority and criticality-based process selection
  - Safe dynamic memory handling
- **Key Files:**
  - `egul_sched.c` — Egul scheduling library implementation
  - `egul_sched.h` — Scheduler API definitions
  - `strawhat_scheduler.h`, `strawhat_support.h`, `strawhat_process.h` — Shell integration and process utilities
- **Docs:** [Scheduler README](Scheduler/README.md)

---

### Task Manager

- **Purpose:** Task management with foreground/background process control, signal handling, and state tracking.
- **Key File:**
  - `zaku.c` — Implements task list management, signal handlers for process control (SIGINT, SIGTSTP), and status logging.
- **Features:**
  - Dynamic linked list for tasks, storing metadata like PID, task number, exit code, state, foreground/background mode, command, and arguments.
  - Foreground/background process distinction, with commands to start, suspend, resume, and kill processes.
  - Custom signal handlers for interactive shell control:  
    - `SIGINT` (Ctrl+C) to kill foreground tasks  
    - `SIGTSTP` (Ctrl+Z) to suspend foreground tasks
  - Logging and status change notifications for process lifecycle events.
  - Instruction set for shell-like commands: `quit`, `help`, `list`, `delete`, `start_fg`, `start_bg`, `kill`, `suspend`, `resume_fg`, `resume_bg`, `fg`, `pipe`, and `chain`.
  - Helper functions for task lookup by PID and running foreground task.
  - Defensive error checking for task operations and signal handling.
- **Docs:** [Task Manager README](TaskManager/README.md)
---

### IPv4 Progression

This area contains two major projects for working with IPv4 addresses and aliases:

#### IPv4 List

- **Description:**  
  A linked-list-based utility to add, look up, display, sort, and save IPv4 addresses and their aliases.
- **Features:**
  - Add IPv4 address and alias pairs interactively.
  - Validate IPv4 address format and value ranges.
  - Look up addresses by alias.
  - Display the full list of addresses and aliases.
  - Show all aliases for a specific location.
  - Sort addresses.
  - Save the address list to a text file with a timestamp and author info.
  - Simple menu-driven interface for ease of use.
- **How to Use:**
  1. Compile:  
     ```sh
     gcc IPv4.c -o IPv4List
     ```
  2. Run:  
     ```sh
     ./IPv4List
     ```
  3. Follow the menu prompts to add, look up, display, sort, or save addresses.

---

#### IPv4 BST

- **Description:**  
  A binary search tree (BST) implementation storing IPv4 address-alias pairs for efficient search, update, and deletion.
- **Features:**
  - Read address-alias pairs from a text file (with error handling for edge cases).
  - Add new addresses and aliases.
  - Lookup addresses by alias.
  - Update and delete entries.
  - Display all entries in alphabetical order.
  - Display aliases for a given location.
  - Save the BST to a file in sorted order.
  - Robust menu interface with input validation.
  - Edge case handling for invalid or duplicate entries.
- **How to Build:**
  1. Navigate to the source directory:  
     ```sh
     cd IPv4 Progression/IPv4 BST/src
     ```
  2. Run the makefile:  
     ```sh
     make create
     ```
  3. Execute the program:  
     ```sh
     ./BSTtest
     ```
  4. When prompted, provide a `.txt` file with address-alias pairs (see `edgecases.txt` for format examples).
- **File Reference:**
  - `menu.c` — Main menu and input handler.
  - `add.c`, `delete.c`, `update.c`, `list.c`, `lookup.c`, `aliasLocation.c`, `quit.c`, `save.c` — Core BST operations.
  - `edgecases.txt` — Sample and edge-case input for testing.
  - `makefile` — Build instructions.
- **Documentation and Examples:**
  - **IPv4 Address Format:**  
    `x.x.x.x [alias]` where `x` is between 0–255 and alias is ≤10 characters.
  - **Sample Input (`edgecases.txt`):**
    ```
    131.250.95.21 jet
    172.66.7.88 wabash
    111.22.5.66 green
    ```
  - **Example Usage:**  
    - Add: Enter new IPv4 address and alias when prompted.
    - Lookup: Enter alias to retrieve associated address.
    - Save: Specify a filename to save current list/tree.
- **Docs:** [IPv4 BST README](IPv4Progression/IPv4BST/README.md)
---

## How to Build

Each subdirectory may contain its own `Makefile` or build instructions. For example, to build the IPv4 BST utility:

```sh
cd "IPv4 Progression/IPv4 BST/src"
make create
```

---

## Contribution

Feel free to explore, fork, and submit pull requests or issues to improve these modules or contribute new ones!

---

## License

Portions of code are © Patrick Doyne and respective instructors. See individual files for details.

---
