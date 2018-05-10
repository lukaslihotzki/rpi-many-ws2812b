#ifndef USER_MEMORY_HPP
#define USER_MEMORY_HPP

#include "LockedMboxAlloc.hpp"
#include "MemoryMapping.hpp"

template <typename T>
class UserMemory : public LockedMboxAlloc, public MemoryMapping<T>
{
public:
	UserMemory(Mailbox& mbox, MemoryFile& mem, Model& model, int size, int align);
};

#include "UserMemory.impl.hpp"

#endif
