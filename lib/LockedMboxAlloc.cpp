#include "LockedMboxAlloc.hpp"

#include "Mailbox.hpp"

LockedMboxAlloc::LockedMboxAlloc(Mailbox& mbox, int size, int align, int flags)
	: MboxAlloc(mbox, size, align, flags)
	, bus_addr(mbox.mem_lock(ref))
{
}

LockedMboxAlloc::~LockedMboxAlloc()
{
	mbox.mem_unlock(ref);
}
