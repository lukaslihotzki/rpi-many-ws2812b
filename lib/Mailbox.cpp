#include "Mailbox.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdexcept>

Mailbox::Mailbox()
	: fd(open("/dev/vcio", O_RDWR))
{
	if (fd == -1) {
		throw std::runtime_error("/dev/vcio error");
	}
}

Mailbox::~Mailbox()
{
	close(fd);
}

unsigned Mailbox::mem_alloc(unsigned size, unsigned align, unsigned flags)
{
	return ctl({4 * 9, 0, 0x3000c, 12, 12, size, align, flags, 0});
}

unsigned Mailbox::mem_free(unsigned handle)
{
	return ctl({4 * 7, 0, 0x3000f, 4, 4, handle, 0});
}

unsigned Mailbox::mem_lock(unsigned handle)
{
	return ctl({4 * 7, 0, 0x3000d, 4, 4, handle, 0});
}

unsigned Mailbox::mem_unlock(unsigned handle)
{
	return ctl({4 * 7, 0, 0x3000e, 4, 4, handle, 0});
}

unsigned Mailbox::get_firmware_revision()
{
	return ctl({4 * 7, 0, 0x10000, 4, 0, 0, 0});
}

unsigned Mailbox::get_board_model()
{
	return ctl({4 * 7, 0, 0x10001, 4, 0, 0, 0});
}

unsigned Mailbox::get_board_revision()
{
	return ctl({4 * 7, 0, 0x10002, 4, 0, 0, 0});
}

unsigned Mailbox::get_dma_channels()
{
	return ctl({4 * 7, 0, 0x60001, 4, 0, 0, 0});
}

unsigned Mailbox::ctl(std::array<unsigned, 9> buf)
{
	int ret_val = ioctl(fd, _IOWR(100, 0, char *), buf.data());

	if (ret_val < 0) {
		throw std::runtime_error("MBOX_PROPERTY ioctl error");
	}

	if (buf[1] != 0x80000000) {
		throw std::runtime_error("MBOX_PROPERTY error");
	}

	return buf[5];
}
