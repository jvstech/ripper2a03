#include "internal_ram_address_line.h"

#include <vector>

auto jvs::CreateInternalRAMMapping(std::vector<std::uint8_t>& buffer)
  -> std::vector<jvs::InternalRAMAddressLine>
{
  std::vector<InternalRAMAddressLine> mapping{};
  mapping.reserve(InternalRAMAddressLine::EffectiveRAMSize);
  for (std::uint16_t addr = 0; addr < InternalRAMAddressLine::EffectiveRAMSize; 
    addr++)
  {
    mapping.emplace_back(buffer, addr);
  }

  return mapping;
}
