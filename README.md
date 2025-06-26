# dFAT Disk Browser

This program implements a simple disk browser for the dFAT filesystem format. It allows you to navigate through directories and read files from a disk image that uses the dFAT format.

## Requirements

- C++ compiler (g++) with C++11 support
- Windows, Linux, or macOS
- Make (optional, but recommended)

## Compilation


### Without Make

If you don't have Make, compile manually with these commands:
```bash
g++ -Wall -std=c++11  dbrowse.cpp Disk.cpp dFAT_data_structures.cpp -o dbrowse

./dbrowse
```

## Running the Program

1. Make sure you have a disk image file named `disk.img` in the same directory as the program
2. Run the program:
   - `./dbrowse`

## Available Commands

- `dir` - List contents of current directory
  - Shows type (FILE/DIR), size, and name of each entry
  
- `cd <dir>` - Change directory
  - Use `cd ..` to go to parent directory
  - Use `cd dirname` to enter a directory

- `read <file>` - Read and display contents of a file
  - Shows the contents of the specified file

- `pwd` - Print current working directory
  - Shows your current location in the filesystem

- `help` - Display available commands

- `exit` or `quit` - Exit the program

## Program Structure

The program uses the following disk structure:
- Block 0: Disk Label
- Block 1: FAT (File Allocation Table)
- Block 2: Root Directory
- Block Size: 512 bytes

## Example Usage

```bash
> dir
Directory listing:
Type    Size        Name
------------------------------
DIR     -          documents
FILE    1024       hello.txt

> cd documents
> dir
Directory listing:
Type    Size        Name
------------------------------
FILE    512        notes.txt
DIR     -          subfolder

> read notes.txt
Contents of notes.txt:
-----------------
This is the content of notes.txt

> cd ..
> pwd
/

> exit
```

## Error Handling

The program handles various error conditions:
- Invalid commands
- Non-existent files or directories
- Attempting to read directories as files
- Attempting to change into files as if they were directories

## Implementation Details

- Uses the provided Disk library for block read/write operations
- Implements proper directory hierarchy navigation
- Handles text encoding and displays only printable characters
- Maintains directory stack for proper parent directory navigation
- Uses byte-aligned structs for correct disk structure interpretation


