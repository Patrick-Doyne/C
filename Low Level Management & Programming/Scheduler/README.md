## *Program Structure & Design*:

  - A simplified CPU scheduling simulator written in C for educational purposes, developed as part of CS367: Operating Systems at George Mason University (Fall 2024).  
  - Models a basic process scheduling system using linked queues and bit masks to represent process states and transitions.

## *Tech Stack*:

  - Language: C (C99 standard)  
  - Compiler Toolchain: GCC (>= 9.x) or Clang  
  - Build System: Make (via custom `Makefile`)  
  - Runtime Environment: POSIX-compliant OS (Linux/macOS)  
  - Memory Debugging: Valgrind for leak detection and profiling  
  - Shell Integration: Strawhat shell headers and support modules

## *Features Implemented*:

  - Dynamic allocation and deallocation of scheduler and queues  
  - Creation and insertion of processes into ready queues  
  - Process aging and starvation mitigation  
  - Selection of next process using criticality, priority, and age  
  - Suspension and resumption of processes  
  - Exit and termination handling, including setting and retrieving exit codes  
  - Bitmask-based state management for ready, running, suspended, and terminated states

## *Key Concepts*:

  - Process State Encoding: Each process embeds state info using a bitmask for efficient transitions  
  - Linked Queue Management: Ready, suspended, and terminated queues modeled as singly linked lists  
  - Priority Scheduling: Selection favors critical, starving, or higher-priority processes  
  - Safe Memory Practices: Defensive deallocation ensures no memory leaks or dangling pointers

## *Files*:

  - `egul_sched.c` – Core implementation of the Egul scheduling library API  
  - `egul_sched.h` – Public interface and definitions for scheduler initialization and management  
  - `strawhat_scheduler.h` – Scaffold for scheduler interactions with the Strawhat shell  
  - `strawhat_support.h` – Utility functions and support routines for Strawhat processes  
  - `strawhat_process.h` – Definitions and helpers for Strawhat process control

## *Functions & Classes*:

  **egul_sched.c**

  - `Egul_schedule_s *egul_initialize();` — Creates the Egul Schedule struct, which holds pointers to all three linked-list queues.  
  - `Egul_process_s *egul_create(Egul_create_data_s *data);` — Allocates and returns a new process node from the provided data.  
  - `int egul_insert(Egul_schedule_s *schedule, Egul_process_s *process);` — Inserts a process node at the end of the Ready Queue.  
  - `int egul_count(Egul_queue_s *queue);` — Returns the number of process nodes in the specified queue.  
  - `Egul_process_s *egul_select(Egul_schedule_s *schedule);` — Removes and returns the highest-priority process from the Ready Queue.  
  - `int egul_suspend(Egul_schedule_s *schedule, pid_t pid);` — Moves a process (by pid) from the Ready Queue to the end of the Suspended Queue.  
  - `int egul_resume(Egul_schedule_s *schedule, pid_t pid);` — Moves a process (by pid) from the Suspended Queue back to the Ready Queue.  
  - `int egul_exited(Egul_schedule_s *schedule, Egul_process_s *process, int exit_code);` — Appends a terminated process node to the Terminated Queue.  
  - `int egul_killed(Egul_schedule_s *schedule, pid_t pid, int exit_code);` — Relocates a process (by pid) from Ready or Suspended Queue into the Terminated Queue.  
  - `int egul_get_ec(Egul_process_s *process);` — Retrieves the exit code from a terminated process’s status field.  
  - `void egul_deallocate(Egul_schedule_s *schedule);` — Frees the schedule struct, all three queues, and their contained process nodes.

## *Licensing & Attribution*:

  - This README covers only the student-authored portions within `egul_sched.c`. Starter code, documentation, and ASCII art remain under Prof. Kevin Andrea’s original copyright.  
  - Upon request, starter code may be shared privately with recruiters for interview purposes.  
  - All Strawhat shell documentation and project specifications remain under their original licenses.
