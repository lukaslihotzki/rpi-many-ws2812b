#ifndef MEMORY_FILE_HPP
#define MEMORY_FILE_HPP

class MemoryFile
{
public:
	MemoryFile();
	~MemoryFile();
private:
	template <typename T>
	friend class MemoryMapping;
	int fd;
};

#endif
