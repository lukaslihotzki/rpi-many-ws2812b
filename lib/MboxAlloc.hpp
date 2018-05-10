#ifndef MBOX_ALLOC_HPP
#define MBOX_ALLOC_HPP

#include "Mailbox.hpp"

class MboxAlloc
{
public:
	MboxAlloc(Mailbox& mbox, int size, int align, int flags);
	~MboxAlloc();
	Mailbox& mbox;
	unsigned const ref;
};

#endif
