*Program Structure & Design*:

    - A simplified CPU scheduling simulator written in C for educational purposes, developed as part of CS367: Operating Systems at George Mason University (Fall 2024). This project 
    models a basic process scheduling system using linked queues and bit masks to represent process states and transitions.

*Features Implemented*:

    - Dynamic allocation and deallocation of scheduler and queues
    - Creation and insertion of processes into ready queues
    - Process aging and starvation mitigation
    - Selection of next process using criticality, priority, and age
    - Suspension and resumption of processes
    - Exit and termination handling, including setting and retrieving exit codes
    - Bitmask-based state management for ready, running, suspended, and terminated states

*Key Concepts*:

    - Process State Encoding: Each process embeds state info using a bitmask for efficient transitions
    - Linked Queue Management: Ready, suspended, and terminated queues modeled as singly linked lists
    - Priority Scheduling: Selection favors critical, starving, or higher-priority processes
    - Safe Memory Practices: Defensive deallocation ensures no memory leaks or dangling pointers

*Files*:

    - This project centers on egul_sched.c and its associated headers and modules:

        * egul_sched.c: Core implementation of the Egul scheduling library API
        * egul_sched.h, strawhat_scheduler.h, strawhat_support.h, strawhat_process.h: Interfaces and support files provided to scaffold the project

*Functions*:

    Egul_schedule_s *egul_initialize();
        • Creates the Egul Schedule struct, which has pointers to all three Linked List Queues.

    Egul_process_s *egul_create(Egul_create_data_s *data);
        • Create a new Process node from the data argument and return a pointer to it.

    int egul_insert(Egul_schedule_s *schedule, Egul_process_s *process);
        • Insert the Process node to the Ready Queue Linked List at the end.

    int egul_count(Egul_queue_s *queue);
        • Return the number of Process nodes in the given Queue's Linked List.

    Egul_process_s *egul_select(Egul_schedule_s *schedule);
        • Remove and return a pointer to the best Process in your Ready Queue Linked List. (Details in Section 4)

    int egul_suspend(Egul_schedule_s *schedule, pid_t pid);
        • Remove the process with pid from Ready Queue and insert in Suspended Queue at the end.

    int egul_resume(Egul_schedule_s *schedule, pid_t pid);
        • Remove the process with pid from Suspended Queue and insert in Ready Queue at the end.

    int egul_exited(Egul_schedule_s *schedule, Egul_process_s *process, int exit_code);
        • Inserts the Process node in the Terminated Queue at the end.

    int egul_killed(Egul_schedule_s *schedule, pid_t pid, int exit_code);
        • Move a Process from either Ready or Suspended Queue to Terminated Queue at the end.

    int egul_get_ec(Egul_process_s *process);
        • Returns the exit code from the status member for the given process.

    void egul_deallocate(Egul_schedule_s *schedule);
        • Free the Egul Schedule, the three Linked List Queues, and all of their Process Nodes


# Licensing & Attribution: #
        - This README pertains only to the student-created portions within egul_sched.c. Starter code, documentation, and ASCII art assets remain under the original copyright: Prof. Kevin Andrea at GMU
        - Upon request, for interviews, I am permitted to use the starter code and share it with said recruiter in a private repo
        - Additionally, all of the documentation for the shell (Strawhat) being used as well as all projects specs for my code


