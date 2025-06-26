
#ifndef DISK_H
#define DISK_H

#include <fstream>
#include <string>

class Disk {

private:

	int blocksize;
	std::fstream diskfile;
	int blockreads, blockwrites;

public:

	Disk(std::string diskfile, int blocksize);
	~Disk();

	void readBlock(int n, char *dstptr);
	void writeBlock(int n, char *srcptr);

	void printStats();

};

#endif
