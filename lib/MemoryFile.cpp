#include "MemoryFile.hpp"

#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

MemoryFile::MemoryFile()
	: fd(open("/dev/mem", O_RDWR | O_SYNC))
{
	if (fd == -1) {
		throw std::runtime_error("/dev/mem error");
	}
}

MemoryFile::~MemoryFile()
{
	close(fd);
}
