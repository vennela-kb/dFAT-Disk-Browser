
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Disk.h"

/*
 * Disk constructor
 *
 * @param diskfile	Name of the disk file to be opened. Should
 * 					be a multiple of `blocksize` in size.
 * @param blocksize	Block size to use.
 *
 * @return none
 */
Disk::Disk(std::string diskfile, int blocksize) {

	this->diskfile.open(diskfile, std::ios::out | std::ios::in | std::ios::binary);

	if (!this->diskfile.is_open()) {
		throw std::runtime_error("Failed to open disk file");
	}

	this->blocksize = blocksize;
	this->blockreads = 0;
	this->blockwrites = 0;

}

/*
 * Disk destructor
 *
 * @return none
 */
Disk::~Disk() {

	this->diskfile.close();

}


/*
 * Read a block from the disk
 *
 * @param n			Block number to read
 * @param dstptr	Memory location where the data should be read in to.
 * 					Should be exactly `blocksize` in size.
 *
 * @return void
 */
void Disk::readBlock(int n, char *dstptr) {

	this->diskfile.seekg(this->blocksize * n);
	this->diskfile.read(dstptr, this->blocksize);

	this->blockreads++;

}

/*
 * Write a block to the disk
 *
 * @param n			Block number to write to
 * @param srcptr	Memory location of data to be written.
 * 					Should be exactly `blocksize` in size.
 *
 * @return void
 */
void Disk::writeBlock(int n, char *srcptr) {

	this->diskfile.seekp(this->blocksize * n);
	this->diskfile.write(srcptr, this->blocksize);

	this->blockwrites++;

}

/*
 * Print statistics showing number of block read/writes.
 *
 * @return void
 */
void Disk::printStats() {

	std::cout << std::endl;
	std::cout << "===== Disk usage statistics =====" << std::endl;
	std::cout << " Total block reads:  " << this->blockreads << std::endl;
	std::cout << " Total block writes: " << this->blockwrites << std::endl;
	std::cout << "=================================" << std::endl;
	std::cout << std::endl;

}