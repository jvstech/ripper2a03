#if !defined(JVS_RIPPER2A03_INTERNAL_RAM_H_)
#define JVS_RIPPER2A03_INTERNAL_RAM_H_

#include <cstdint>
#include <vector>

#include "internal_ram_address_line.h"

namespace jvs
{

class InternalRAM
{
public:

  InternalRAM();

  const InternalRAMAddressLine& operator[](std::uint16_t address) const
  {
    return mapping_[address];
  }

  InternalRAMAddressLine& operator[](std::uint16_t address)
  {
    return mapping_[address];
  }

private:
  std::vector<std::uint8_t> buffer_;
  std::vector<InternalRAMAddressLine> mapping_;
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_INTERNAL_RAM_H_
