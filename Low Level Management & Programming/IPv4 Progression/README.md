# *IPv4 Directory Overview*

## *Directory Description*  
This directory features a series of progressively advanced programs designed to manage IPv4 addresses and associated usernames. Each file demonstrates increasingly sophisticated techniques in data management and user interaction, providing a clear pathway for skill development in systems programming.

## *Overview*  
The programs focus on mutation, sorting, and storage of IPv4 addresses and usernames. Their modular design supports easy extension, including additional data fields and functionality.

## *Shared Logic and Features*  

- **Variables and Methods:**  
  - IPv4 addresses are stored as four integers (0–255), separated by periods, following standard conventions.
  - Aliases represent usernames (such as employees or guest WiFi users) and are stored as character strings.
  - Core functions offer consistent capabilities across implementations, with varied approaches for learning purposes.

- **Input/Output and Storage:**  
  - Users can specify files for storage or import.
  - Save options include creating new files or overwriting existing ones.
  - Changes can be discarded with a graceful exit.

- **Error Handling:**  
  - Duplicate entries (by alias or IPv4 address) are ignored, with case-insensitive matching for aliases.
  - IPv4 addresses are validated for format and value range; invalid lines are skipped.
  - Aliases must be unique, 1–10 characters, and free of forbidden characters.

- **Data Format:**  
  - Aliases shorter than 10 characters are padded for consistency.
  - All functions use the format `<IPv4, alias>` for storing and retrieving information.

## *Project Modules*

- **IPv4 List:**  
  Demonstrates program flow, typedef usage, and basic I/O. Includes a simple terminal UI for importing data from text files, error checking, and options for sorting and saving lists.

- **IPv4 BST:**  
  Reads text files to create a doubly linked binary search tree (BST) of IPv4 and alias pairs. Provides menu-driven options to modify, display, and save data, with memory management protections.

- **IPv4 MySQL:**  
  Connects to MySQL via Linux, enabling table creation and management. The global list can be merged with imported text data, and changes can be synced with the database. Users can save data to files or any SQL table.

## *Summary*  
This directory showcases foundational and advanced data management techniques, including file handling, validation, sorting, and database integration. It demonstrates core competencies in systems programming and backend development.

#### NOTE: Functionality of IPv4 MySql program can be layered for a UI experience* (  Coming soon: Spring 2026 :)  )* ####

