#ifndef LOCKED_MBOX_ALLOC_HPP
#define LOCKED_MBOX_ALLOC_HPP

#include "MboxAlloc.hpp"

class LockedMboxAlloc : public MboxAlloc
{
public:
	LockedMboxAlloc(Mailbox& mbox, int size, int align, int flags);
	~LockedMboxAlloc();

	const unsigned bus_addr;
};

#endif
