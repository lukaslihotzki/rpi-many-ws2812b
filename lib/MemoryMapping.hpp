#ifndef MEMORY_MAPPING_HPP
#define MEMORY_MAPPING_HPP

class MemoryFile;

template <typename T = unsigned>
class MemoryMapping
{
public:
	MemoryMapping(MemoryFile& mem, unsigned virt_base, unsigned phys_base, unsigned len = sizeof(T));
	~MemoryMapping();
	operator T*();
	T* operator->();
	unsigned virt_base;
	unsigned phys_base;
	unsigned const len;
	T* const ptr;
	unsigned to_virtual(void* data);
	unsigned to_virtual(volatile void* data);
	unsigned to_physical(void* data);
	unsigned to_physical(volatile void* data);
};

#include "MemoryMapping.impl.hpp"

#endif
