#include "internal_ram.h"

jvs::InternalRAM::InternalRAM()
  : buffer_(InternalRAMAddressLine::InternalRAMSize, 
    static_cast<std::uint8_t>(0)),
  mapping_(jvs::CreateInternalRAMMapping(buffer_))
{
}
