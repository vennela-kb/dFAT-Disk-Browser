

// this file is just for reference, you don't need to use it in your implementation.
// it's just meant to formally describe how the data structures are laid out.

// probably start reading this from the bottom, where `block` is defined.
// everything else builds on top of a block.  :)

// if you're unfamiliar with C syntax, you might find it helpful to Google the following first...
//   - C union
//   - C struct
//   - C enum


// we will assume a block is 512 bytes
#define BLOCK_SIZE 512


// the two-byte values which may be found in the FAT, include
enum fat_entry_type {
	DISK_LABEL		= 0x0000,		// this block contains the disk label
	FAT_BLOCK		= 0x0001,		// this block contains the FAT itself
	IN_USE			= 0xfefe,		// this block is either a directory entry, or a file chunk
	UNALLOCATED		= 0xffff,		// this block is free to be used
};


// within a directory entry, these are the valid listing types
enum dir_entry_type {
	UNUSED_ENTRY	= 0x0000,		// unused slot.  put new file or directory listings here
	DIR_ENTRY		= 0x0002,		// this entry points to a child directory node
	FILE_CHUNK		= 0x0003,		// this entry points to the first block of a file
};


// a directory listing, containing information about one file or directory
// total size = 32   (2 + 2 + 4 + 24 = 32)
struct dir_listing {
	unsigned short type;			// a dir_entry_type.  either DIR, FILE, or unused
	unsigned short block;			// the block where this file or directory can be found
	unsigned int metadata;			// information about the item (currently just used for file size)
	char name[24];					// the name of the entry
};


// this block just holds the disk label, and any other info we might
// want to add later on
struct disk_label {
	char label[BLOCK_SIZE];			// just let the label use the whole thing
};


// the FAT block
// an array of 2-byte values, mapping the state of all blocks on the disk
struct fat {
	unsigned short table[BLOCK_SIZE / 2];		// each on holds a fat_entry_type
};


// a directory entry block, containing a list of directory listings
struct dir_entry {
	struct dir_listing list[BLOCK_SIZE / sizeof(dir_listing)];  // each holds a directory listing
};


// a file chunk block.  just holds file data.  a file may span many of these.
struct file_chunk {
	char data[BLOCK_SIZE];
};


// the most basic component!  everything else builds on this.
// a block may be any one of these, all of
// which would occupy the full 512 bytes
union block {
	struct disk_label;		// a block containing the disk label
	struct fat;				// a block containing the FAT
	struct dir_entry;		// a block containing a directory entry
	struct file_chunk;		// a block containing a file chunk
};


