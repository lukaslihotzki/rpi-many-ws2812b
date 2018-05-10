#include "UserMemory.hpp"

#include <cstdint>

template<typename T>
UserMemory<T>::UserMemory(Mailbox& mbox, MemoryFile& mem, Model& model, int size, int align)
	: LockedMboxAlloc(mbox, size, align, model.mem_flag)
	, MemoryMapping<T>(mem, bus_addr &~ 0xc0000000, bus_addr, size)
{
}
