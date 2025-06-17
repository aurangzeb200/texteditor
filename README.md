# 📝 Vim-Inspired Text Editor

A lightweight, console-based text editor written in C++ that emulates Vim-like functionality. It supports text insertion, navigation, copy-paste, search/replace, and file operations using a doubly linked list for efficient line and character management. Ideal for learning about data structures and text editor design.

## Features
- **Vim-like Modes**: Toggle between insert and normal modes, with command mode for file operations (e.g., `:w`, `:q`).
- **Efficient Text Management**: Uses a doubly linked list to store and manipulate characters in each line.
- **Command History**: Tracks and navigates through executed commands using a deque.
- **Search and Replace**: Supports pattern searching (`/pattern`) and substitution (`:s/old/new`).
- **File Operations**: Load and save files with error handling for common issues (e.g., permissions, disk full).
- **Line Operations**: Copy, paste, delete, indent, and join lines with numeric prefixes (e.g., `2dd` deletes two lines).

## Interesting Techniques Used
Let’s dive into a few cool techniques this project uses, explained step-by-step like we’re sketching it out on a whiteboard.

### 1. Doubly Linked List for Text Lines
Each line in the editor is a `Line` object, and each character in a line is a `Node` in a [doubly linked list](https://en.wikipedia.org/wiki/Doubly_linked_list). Why? It makes insertions and deletions super efficient, especially when you’re moving the cursor around or editing in the middle of a line.

- **How it works**: A `Node` has a `char data`, a `next` pointer, and a `prev` pointer. The `Line` class tracks the `head` (first node) and `cursor` (current position).
- **Example**: To insert a character, we create a new `Node`, adjust the `next` and `prev` pointers of adjacent nodes, and update the `cursor`. This is O(1) for insertion at the cursor position.
- **Why it’s cool**: Compared to a string or array, a linked list avoids shifting elements during edits, which is critical for real-time text editing.

### 2. Command History with Deque
The editor keeps track of commands (e.g., “Move Right”, “Delete Line”) in a `deque` (double-ended queue). This lets you navigate through your command history using up/down arrows, similar to a shell.

- **How it works**: The `EditorStatus` class stores commands in a `deque<string> commandHistory`. The `index` tracks your position in the history, and methods like `moveToNextCommand` update it.
- **Example**: When you run a command (e.g., `:w file.txt`), it’s pushed to the `deque`. Pressing up/down adjusts the `index` to show previous/next commands.
- **Why it’s cool**: A [deque](https://en.cppreference.com/w/cpp/container/deque) is perfect here because it supports fast insertions at both ends, and we only need sequential access for history navigation.

### 3. Pattern Search and Replace
The search feature (`/pattern`) and replace command (`:s/old/new`) use a character-by-character matching algorithm. It’s not regex, but it’s efficient for simple text patterns.

- **How it works**: The `search` method scans each line’s linked list, comparing characters to the pattern. If a match is found, it updates the cursor and tracks the line/column. Replace builds on this by deleting the matched text and inserting the new string.
- **Example**: For `/hello`, it checks each `Node` for ‘h’, then verifies ‘e’, ‘l’, ‘l’, ‘o’. If found, it sets the cursor before the match and reports the line/column.
- **Why it’s cool**: This approach leverages the linked list’s structure for traversal, avoiding string copying, and supports global replace with the `/g` flag.

## Non-Obvious Libraries/Tools Used
- **[conio.h](https://en.wikipedia.org/wiki/Conio.h)**: Handles low-level console input (e.g., `_getch` for non-blocking keypress detection). It’s Windows-specific but critical for real-time input without requiring Enter.
- **[deque](https://en.cppreference.com/w/cpp/container/deque)**: Used for command history. Unlike `vector`, it’s optimized for insertions at both ends, making it ideal for history navigation.
- **[fstream](https://en.cppreference.com/w/cpp/header/fstream)**: Manages file I/O for loading and saving text files, with error handling for permissions and disk issues.

## Project Folder Structure

- **Text_editor_project.cpp**: The main source file containing all classes (`Node`, `Line`, `TextEditor`, `FileManager`) and logic.
- **README.md**: This documentation file.

## Build and Run
1. **Prerequisites**: A C++ compiler (e.g., g++ on Windows with MinGW or MSVC).
