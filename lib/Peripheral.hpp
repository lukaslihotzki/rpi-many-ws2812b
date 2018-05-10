#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include "MemoryMapping.hpp"
#include "Model.hpp"

template <typename T = unsigned>
class Peripheral : public MemoryMapping<T>
{
public:
	Peripheral(MemoryFile& mem, Model& model, unsigned addr, unsigned len = sizeof(T))
		: MemoryMapping<T>(mem, model.virt_base + addr, model.phys_base + addr, len)
	{
	}
};

#endif
