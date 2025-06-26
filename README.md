# dFAT Disk Browser

A command-line utility written in C++ that allows you to explore and read files from a disk image formatted with the dFAT filesystem.

---

## 🚀 Project Overview

The **dFAT Disk Browser** enables:
- **Directory Navigation**: List and traverse directory structures.
- **File Reading**: Display contents of files stored on the disk image.
- **Disk Statistics**: Track block read/write operations.

Built on a simple File Allocation Table (FAT) design, this tool demonstrates how filesystems organize and manage data at the block level.

---

## 🧩 Repository Structure

```
.
├── dbrowse.cpp               # Main application: parses commands, interacts with disk
├── Disk.cpp                  # Implements Disk class for block-level I/O
├── Disk.h                    # Disk class header
├── dFAT_data_structures.cpp  # Reference definitions of FAT, directory, and block layouts
├── disk.img                  # Sample disk image (512 B blocks) [not in repo]
├── Makefile                  # (Optional) Build instructions
└── README.md                 # This file
```

---

## 🔧 Prerequisites

- **C++ Compiler** with C++11 support (e.g., g++, clang++)
- **Make** (optional, for using the provided Makefile)
- **disk.img**: A binary disk image formatted with dFAT (must be present in working directory)

---

## 🔨 Build Instructions

### Using Makefile (if provided)

```bash
make
```

This compiles and links all sources into the `dbrowse` executable.

### Manual Compilation

```bash
g++ -std=c++11 -Wall     dbrowse.cpp Disk.cpp dFAT_data_structures.cpp     -o dbrowse
```

---

## 📈 Running the Disk Browser

1. Ensure `disk.img` is in the same directory.
2. Run the executable:
   ```bash
   ./dbrowse
   ```
3. You will see the disk label and prompt:
   ```
   Disk Label: MyDisk
   Enter 'help' for available commands.
   > 
   ```

---

## 💻 Available Commands

- `help`  
  Display a list of supported commands and their usage.
- `dir`  
  List contents of the current directory (type, size, name).
- `cd <directory>`  
  Change into a child directory (`..` for parent).
- `read <filename>`  
  Read and display contents of a file (prints only printable ASCII characters).
- `pwd`  
  Print the current working directory path.
- `exit` / `quit`  
  Exit the program and display disk usage statistics.

---

## 🛠️ Implementation Details

- **Block Size**: 512 bytes  
- **Disk Label**: Stored in block 0; printed at startup.  
- **FAT Table**: Stored in block 1; tracks block statuses (in use, unallocated).  
- **Root Directory**: Begins at block 2; directory entries point to files or subdirectories.  
- **Disk I/O**: `Disk` class provides `readBlock` and `writeBlock` methods with I/O counters.  
- **Memory Structures**:  
  - `fat` and `dir_entry` structs interpret raw block data.  
  - `dir_listing` entries include type, block pointer, metadata (e.g., size), and name.

On exit, the tool prints total block reads and writes to help analyze I/O performance.

---

## 📚 Example Session

```bash
$ ./dbrowse
Disk Label: SampleDisk
Enter 'help' for available commands.

> dir
Type    Size        Name
------------------------------
DIR              documents
FILE    2048      report.txt

> cd documents
> dir
Type    Size        Name
------------------------------
FILE    512       notes.txt
DIR              images

> read notes.txt
Contents of notes.txt:
--------------------
Meeting at 10 AM on Monday.

> pwd
/

> exit
===== Disk usage statistics =====
 Total block reads:  15
 Total block writes: 0
=================================
```

---

## 🤝 Contributing

Contributions and improvements are welcome! Feel free to:

- Add support for writing files or creating directories.
- Implement file deletion and new file creation.
- Extend the filesystem to handle larger disk images or variable block sizes.

Please open issues or submit pull requests.

---

## 📬 Contact

For questions or feedback, contact **kothakonda.baby@gmail.com**.
