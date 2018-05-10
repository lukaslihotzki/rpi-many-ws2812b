#ifndef MODEL_HPP
#define MODEL_HPP

class Mailbox;

class Model
{
public:
	Model(Mailbox& mailbox);

	const unsigned virt_base;
	const unsigned phys_base;
	const unsigned mem_flag;

private:
	static bool isNew(unsigned revision);
};

#endif
