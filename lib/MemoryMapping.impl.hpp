#include "MemoryMapping.hpp"

#include "MemoryFile.hpp"

#include <sys/mman.h>

template<typename T>
MemoryMapping<T>::MemoryMapping(MemoryFile& mem, unsigned virt_base, unsigned phys_base, unsigned len)
	: virt_base(virt_base)
	, phys_base(phys_base)
	, len(len)
	, ptr(reinterpret_cast<T*>(mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED, mem.fd, virt_base)))
{
	if (ptr == MAP_FAILED) {
		throw std::runtime_error("map failed");
	}
}

template<typename T>
MemoryMapping<T>::~MemoryMapping()
{
	munmap(ptr, len);
}

template<typename T>
T* MemoryMapping<T>::operator->()
{
	return ptr;
}

template<typename T>
MemoryMapping<T>::operator T*()
{
	return ptr;
}

template<typename T>
unsigned MemoryMapping<T>::to_virtual(void* data)
{
	return reinterpret_cast<uint8_t*>(data) - reinterpret_cast<uint8_t*>(this->ptr) + virt_base;
}

template<typename T>
unsigned MemoryMapping<T>::to_virtual(volatile void* data)
{
	return reinterpret_cast<volatile uint8_t*>(data) - reinterpret_cast<volatile uint8_t*>(this->ptr) + virt_base;
}

template<typename T>
unsigned MemoryMapping<T>::to_physical(void* data)
{
	return reinterpret_cast<uint8_t*>(data) - reinterpret_cast<uint8_t*>(this->ptr) + phys_base;
}

template<typename T>
unsigned MemoryMapping<T>::to_physical(volatile void* data)
{
	return reinterpret_cast<volatile uint8_t*>(data) - reinterpret_cast<volatile uint8_t*>(this->ptr) + phys_base;
}
