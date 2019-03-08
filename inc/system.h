#if !defined(JVS_RIPPER2A03_SYSTEM_H_)
#define JVS_RIPPER2A03_SYSTEM_H_

#include <functional>

#include "intenral_ram.h"
#include "nes_rom.h"

namespace jvs
{

class CPU;

class System
{
  friend class CPU;

public:
  System(NesRom& rom);

  const InternalRAM& internal_ram() const
  {
    return internal_ram_;
  }

  InternalRAM& internal_ram()
  {
    return internal_ram_;
  }

private:
  InternalRAM internal_ram_{};
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_SYSTEM_H_