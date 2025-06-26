#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Disk.h"

using namespace std;

#define BLOCK_SIZE 512
#define DISK_FILE "disk.img"

// Data structure definitions
enum fat_entry_type {
    DISK_LABEL = 0x0000,
    FAT_BLOCK = 0x0001,
    IN_USE = 0xfefe,
    UNALLOCATED = 0xffff,
};

enum dir_entry_type {
    UNUSED_ENTRY = 0x0000,
    DIR_ENTRY = 0x0002,
    FILE_CHUNK = 0x0003,
};

#pragma pack(push, 1)  // Ensure struct packing is byte-aligned
struct dir_listing {
    unsigned short type;
    unsigned short block;
    unsigned int metadata;
    char name[24];
};

struct fat {
    unsigned short table[BLOCK_SIZE / 2];
};

struct dir_entry {
    dir_listing list[BLOCK_SIZE / sizeof(dir_listing)];
};
#pragma pack(pop)

// Global variables
Disk* disk;
vector<int> dir_stack;  // Stack of directory block numbers for ".."
int current_block;
fat* FAT;  // Global FAT table

// Function declarations
void printHelp();
void listDirectory();
void changeDirectory(const string& dirname);
void readFile(const string& filename);
void printWorkingDirectory();
int findEntry(const string& name, const dir_entry& dir);
void readDiskLabel();
void readFAT();

int main() {
    // Initialize disk
    disk = new Disk(DISK_FILE, BLOCK_SIZE);
    
    // Read FAT from block 1
    readFAT();
    
    // Root directory starts at block 2
    current_block = 2;

    // Read disk label from block 0
    readDiskLabel();

    string command;
    cout << "\nEnter 'help' for available commands.\n";

    while (true) {
        cout << "\n> ";
        getline(cin, command);

        if (command.empty()) continue;

        istringstream iss(command);
        string cmd;
        iss >> cmd;

        if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "dir") {
            listDirectory();
        }
        else if (cmd == "cd") {
            string dirname;
            iss >> dirname;
            if (dirname.empty()) {
                cout << "Usage: cd <directory>\n";
            } else {
                changeDirectory(dirname);
            }
        }
        else if (cmd == "read") {
            string filename;
            iss >> filename;
            if (filename.empty()) {
                cout << "Usage: read <filename>\n";
            } else {
                readFile(filename);
            }
        }
        else if (cmd == "pwd") {
            printWorkingDirectory();
        }
        else if (cmd == "exit" || cmd == "quit") {
            break;
        }
        else {
            cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    disk->printStats();
    delete disk;
    delete FAT;
    return 0;
}

void readFAT() {
    char block[BLOCK_SIZE];
    disk->readBlock(1, block);  // FAT is in block 1
    FAT = new fat;
    memcpy(FAT, block, BLOCK_SIZE);
}

void printHelp() {
    cout << "\nAvailable commands:\n";
    cout << "  dir           - List contents of current directory\n";
    cout << "  cd <dir>      - Change directory (use '..' for parent)\n";
    cout << "  read <file>   - Read and display contents of a file\n";
    cout << "  pwd           - Print current working directory\n";
    cout << "  help          - Display this help message\n";
    cout << "  exit/quit     - Exit the program\n";
}

void readDiskLabel() {
    char block[BLOCK_SIZE];
    disk->readBlock(0, block);
    cout << "Disk Label: ";
    // Only print valid ASCII characters
    for (int i = 0; i < BLOCK_SIZE && block[i] != '\0'; i++) {
        if (isprint(block[i])) {
            cout << block[i];
        }
    }
    cout << endl;
}

void listDirectory() {
    char block[BLOCK_SIZE];
    disk->readBlock(current_block, block);
    dir_entry* dir = (dir_entry*)block;

    cout << "\nDirectory listing:\n";
    cout << setw(8) << left << "Type" << setw(12) << "Size" << "Name\n";
    cout << string(30, '-') << endl;

    int num_entries = BLOCK_SIZE / sizeof(dir_listing);
    for (int i = 0; i < num_entries; i++) {
        const dir_listing& entry = dir->list[i];
        
        // Skip unused entries
        if (entry.type == UNUSED_ENTRY) continue;
        
        // Only show valid directory entries and file chunks
        if (entry.type != DIR_ENTRY && entry.type != FILE_CHUNK) continue;

        string type = (entry.type == DIR_ENTRY) ? "DIR" : "FILE";
        string size = (entry.type == DIR_ENTRY) ? "" : to_string(entry.metadata);
        
        // Clean up the name - only print valid ASCII characters
        string name;
        for (int j = 0; j < 24 && entry.name[j] != '\0'; j++) {
            if (isprint(entry.name[j])) {
                name += entry.name[j];
            }
        }
        
        if (!name.empty()) {
            cout << setw(8) << left << type;
            cout << setw(12) << size;
            cout << name << endl;
        }
    }
}

int findEntry(const string& name, const dir_entry& dir) {
    int num_entries = BLOCK_SIZE / sizeof(dir_listing);
    for (int i = 0; i < num_entries; i++) {
        const dir_listing& entry = dir.list[i];
        if (entry.type != UNUSED_ENTRY) {
            string entry_name;
            for (int j = 0; j < 24 && entry.name[j] != '\0'; j++) {
                if (isprint(entry.name[j])) {
                    entry_name += entry.name[j];
                }
            }
            if (entry_name == name) {
                return entry.block;
            }
        }
    }
    return -1;
}

void changeDirectory(const string& dirname) {
    if (dirname == "..") {
        if (!dir_stack.empty()) {
            current_block = dir_stack.back();
            dir_stack.pop_back();
        }
        return;
    }

    char block[BLOCK_SIZE];
    disk->readBlock(current_block, block);
    dir_entry* dir = (dir_entry*)block;

    int new_block = findEntry(dirname, *dir);
    if (new_block != -1) {
        // Verify it's actually a directory
        disk->readBlock(new_block, block);
        dir_entry* new_dir = (dir_entry*)block;
        if (new_dir->list[0].type == DIR_ENTRY || new_dir->list[0].type == UNUSED_ENTRY) {
            dir_stack.push_back(current_block);
            current_block = new_block;
        } else {
            cout << dirname << " is not a directory" << endl;
        }
    } else {
        cout << "Directory not found: " << dirname << endl;
    }
}

void readFile(const string& filename) {
    char block[BLOCK_SIZE];
    disk->readBlock(current_block, block);
    dir_entry* dir = (dir_entry*)block;

    int file_block = findEntry(filename, *dir);
    if (file_block == -1) {
        cout << "File not found: " << filename << endl;
        return;
    }

    // Read and display file contents
    disk->readBlock(file_block, block);
    cout << "\nContents of " << filename << ":\n";
    cout << string(40, '-') << endl;
    
    // Print only valid ASCII characters
    for (int i = 0; i < BLOCK_SIZE && block[i] != '\0'; i++) {
        if (isprint(block[i]) || block[i] == '\n' || block[i] == '\t' || block[i] == '\r') {
            cout << block[i];
        }
    }
    cout << endl;
}

void printWorkingDirectory() {
    cout << "/";
    // We would need to store directory names to implement this properly
    cout << endl;
} 
