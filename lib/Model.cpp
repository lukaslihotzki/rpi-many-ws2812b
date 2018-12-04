#include "Model.hpp"

#include "Mailbox.hpp"

enum MemFlag {
	discardable = 1 << 0,
	normal = 0 << 2,
	direct = 1 << 2,
	coherent = 2 << 2,
	l1_nonallocating = direct | coherent,
	zero = 1 << 4,
	no_init = 1 << 5,
	hint_permalock = 1 << 6,
};

enum {
	board_revision_new_scheme = 1 << 23,
	board_revision_type_mask = 0xff << 4,
	board_revision_type_offs = 4,
};

enum BoardRevisionType {
	pi1_a = 0,
	pi1_b = 1,
	pi1_a_plus = 2,
	pi1_b_plus = 3,
	pi2_b = 4,
	alpha = 5,
	cm = 6,
	pi3_b = 8,
	cm3 = 10,
	pi3_b_plus = 13
};

Model::Model(Mailbox& mailbox)
	: virt_base(isNew(mailbox.get_board_revision()) ? 0x3f000000 : 0x20000000)
	, phys_base(0x7e000000)
	, mem_flag(l1_nonallocating | no_init)
{
}

bool Model::isNew(unsigned revision)
{
	if (revision & board_revision_new_scheme) {
		BoardRevisionType type = BoardRevisionType((revision & board_revision_type_mask) >> board_revision_type_offs);
		return type == pi2_b || type == pi3_b || type == cm3 || type == pi3_b_plus;
	} else {
		return false;
	}
}
