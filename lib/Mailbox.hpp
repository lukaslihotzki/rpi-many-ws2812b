#ifndef MAILBOX_HPP
#define MAILBOX_HPP

#include <array>

class Mailbox
{
public:
	Mailbox();
	~Mailbox();
	unsigned mem_alloc(unsigned size, unsigned align, unsigned flags);
	unsigned mem_free(unsigned handle);
	unsigned mem_lock(unsigned handle);
	unsigned mem_unlock(unsigned handle);
	unsigned get_firmware_revision();
	unsigned get_board_model();
	unsigned get_board_revision();
	unsigned get_dma_channels();
private:
	unsigned ctl(std::array<unsigned, 9> buf);
	int fd;
};

#endif
