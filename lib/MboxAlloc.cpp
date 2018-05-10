#include "MboxAlloc.hpp"

MboxAlloc::MboxAlloc(Mailbox& mbox, int size, int align, int flags)
	: mbox(mbox)
	, ref(mbox.mem_alloc(size, align, flags))
{
}

MboxAlloc::~MboxAlloc()
{
	mbox.mem_free(ref);
}
